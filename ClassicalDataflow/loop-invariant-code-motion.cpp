// 15-745 S14 Assignment 3: loop-invariant-code-motion.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "loop-invariant-code-motion.h"

using std::cout;
using std::cerr;
using std::endl;
using std::set;
using std::vector;


namespace llvm {

//
// Call the constructor for the super class.
//
LicmPass::LicmPass() : LoopPass(ID) {
  // do nothing for now
}


//
// Helper function for printing out dominator information.
//
void LicmPass::showDominators(const BlockVector& blocks,
    BlockStates& states, BasicBlock* preheader) {
  for (BlockVector::const_iterator I = blocks.begin(), IE = blocks.end();
      I != IE; ++I) {
    BasicBlock* block = *I;
    BasicBlock* idom = dominance.getIdom(states, block);
    cerr << block->getName().data() << " idom ";
    if (idom) {
      cerr << idom->getName().data();
    } else {
      cerr << preheader->getName().data();
    }
    cerr << endl;
  }
  cerr << endl;
}


//
// Helper function to determine if the given block is in a sub-loop or not.
//
bool LicmPass::isTopLevel(const Loop* loop, const BasicBlock* block) {
  return info->getLoopFor(block) == loop;
}


//
// Check if instruction is invariant.
//
bool LicmPass::isInvariant(const Loop* loop,
   Invariants& invariants, Instruction* instr) {

    // Must also satisfy these conditions to ensure safety of movement.
    if (!isSafeToSpeculativelyExecute(instr) ||
        instr->mayReadFromMemory() ||
        isa<LandingPadInst>(instr)) {
      return false;
    }

    // See if all operands are invariant.
    for (User::op_iterator OI = instr->op_begin(), OE = instr->op_end();
        OI != OE; ++OI) {
      Value *operand = *OI;
      if (!isInvariant(loop, invariants, operand)) {
        return false;
      }
    }

    // Satisfies all conditions.
    return true;
}


//
// iterate through operands and check to see if they are invariant.
// An invariant is any non-instruction or must meet 1 of the following.
// - constant (TODO: try and constant fold here)
// - all reaching definitions for operand are outside the loop
// - both have exactly 1 reaching definition and it is invariant
//
bool LicmPass::isInvariant(const Loop* loop,
    Invariants& invariants, Value* operand) {
  // invariance check for instruction operands
  if (Instruction* instr = dyn_cast<Instruction>(operand)) {
    return invariants.count(instr) || !loop->contains(instr);
  }
  // All non-instructions are invariant.
  return true;
}


//
// Find loop invariant instructions in a given block.
// We can do this block by block because we are traversing the dominator
// tree in pre-order, which accumulating invariants in our set.
//
void LicmPass::inspectBlock(const Loop* loop, BasicBlock* block,
    Invariants& invariants) {
  // No need to iterate over subloops because they will already have their
  // instructions hoisted. Skip if we are in the subloop.
  bool top_level = isTopLevel(loop, block);
  if (!top_level) {
    return;
  }

  // Iterate through all the intructions.
  for (BasicBlock::iterator J = block->begin(), JE = block->end();
      J != JE; ++J) {
    Instruction& instr = *J;
    bool invariant = isInvariant(loop, invariants, &instr);
    if (invariant) {
      invariants.insert(&instr);
    }
  }
}


//
// For a given loop, find the loop invariant instructions and populate the
// set of invariant instructions found.
//
void LicmPass::findInvariants(
    const Loop* loop,
    const BlockVector& blocks,
    BlockStates& states,
    BasicBlock* preheader,
    Invariants& invariants) {
  // Compute the dominance tree.
  DominancePass::Node dom_tree = dominance.getDominatorTree(
      blocks, states, preheader);
  vector<DominancePass::Node*> stack;
  stack.push_back(&dom_tree);
  // Do a pre-order traversal over the Dominance Tree.
  while (!stack.empty()) {
    DominancePass::Node* node = stack.back();
    stack.pop_back();
    if (node->parent) {
      inspectBlock(loop, node->data, invariants);
    }
    for (int i = 0; i < node->children.size(); i++) {
      stack.push_back(node->children[i]);
    }
  }
}


//
// Override the runOnLoop function provided by LoopPass.
// Return true because we intend on modifying the control flow graph.
//
bool LicmPass::runOnLoop(Loop *loop, LPPassManager &LPM) {
  unsigned int depth = loop->getLoopDepth();
  info = &getAnalysis<LoopInfo>();
  cout << "Loop<depth = " << depth << ">" << endl;

  // check if there is a preheader
  BasicBlock* preheader = loop->getLoopPreheader();
  if (!preheader) {
    return false;
  }

  // Run analysis passes on blocks.
  const BlockVector& blocks = loop->getBlocks();
  BlockStates states = dominance.runOnBlocks(blocks);
  showDominators(blocks, states, preheader);

  // Cache all the invariants we have found so far.
  Invariants invariants;
  findInvariants(loop, blocks, states, preheader, invariants);

  // Finally, hoist the invariants into the preheader.
  // We want to delete this instruction and move it to the end of the
  // preheader (before the branch). We can do with with moveBefore.
  for (Invariants::iterator I = invariants.begin(), IE = invariants.end();
      I != IE; ++I) {
    Instruction* end = &(preheader->back());
    Instruction* instr = *I;
    instr->dump();
    instr->moveBefore(end);
  }

  // assume we modified the loop
  cerr << endl;
  return true;
};


//
// Set so that we can modify the control flow graph.
// TODO: Is this correct?
//
void LicmPass::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
  AU.addRequired<LoopInfo>();
}


//
// Do the following to meet the LoopPass API
//
char LicmPass::ID = 0;
RegisterPass<LicmPass> Y("cd-licm", "15745 Liveness");

}
