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
    const BasicBlock* preheader) {
  BlockStates states = dominance.runOnBlocks(blocks);
  for (BlockVector::const_iterator I = blocks.begin(), IE = blocks.end();
      I != IE; ++I) {
    const BasicBlock* block = *I;
    const BasicBlock* idom = dominance.getIdom(states, block);
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


// iterate through operands and check to see if they are invariant.
// they must meet 1 of the following criteria
// - both constant
// - all reaching definitions for operands are outside the loop
// - both have exactly 1 reaching definition and it is invariant
bool LicmPass::isInvariant(Loop* loop, set<const Instruction*>& invariants,
    const Instruction* instr) {
    instr->dump();
    for (User::const_op_iterator OI = instr->op_begin(), OE = instr->op_end();
        OI != OE; ++OI) {
      const Value *val = *OI;
      if (!isInvariant(loop, invariants, val)) {
        return false;
      }
    }
    // Extra conditions to ensure safety of code movement.
    return (
      isSafeToSpeculativelyExecute(instr) &&
      !instr->mayReadFromMemory() &&
      !isa<LandingPadInst>(instr));
}


bool LicmPass::isInvariant(Loop* loop, set<const Instruction*>& invariants,
    const Value* operand) {
  // invariance check for instruction operands
  if (isa<const Instruction>(operand)) {
    cerr << "   - op<instr>";
    const Instruction* instr = dyn_cast<const Instruction>(operand);
    bool invariant = invariants.count(instr) || !loop->contains(instr);
    if (invariant) cerr << ": invariant";
    cerr << endl;
    return invariant;
  }

  // invariance check for constants
  else if (isa<const Constant>(operand)) {
    cerr << "   - op<Constant>: invariant" << endl;
    return true;
  }

  // invariance check for argument operand
  else if (isa<const Argument>(operand)) {
    cerr << "   - op<Argument>: invariant" << endl;
    return true;
  }

  // could not confirm operand as invariant
  cerr << "  - op<Other>" << endl;
  operand->dump();
  return false;
}


//
// Helper function to determine if the given block is in a sub-loop or not.
//
bool LicmPass::isTopLevel(const Loop* loop, const BasicBlock* block) {
  return info->getLoopFor(block) == loop;
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

  // run analysis passes on blocks
  const BlockVector& blocks = loop->getBlocks();
  showDominators(blocks, preheader);

  // compute reaching definitions for all blocks
  set<const Instruction*> invariants;
  BlockStates states = reaching.runOnBlocks(blocks);

  // go through all blocks in the loop
  for (BlockVector::const_iterator I = blocks.begin(), IE = blocks.end();
      I != IE; ++I) {
    const BasicBlock* block = *I;
    BlockState& state = states[block];
    // no need to iterate over subloops because they will already have their
    // instructions hoisted
    // can do a subloop check, then skip if we are in the subloop
    bool top_level = isTopLevel(loop, block);
    cerr << block->getName().data() << " (" << (int)top_level << "):" << endl;
    if (!top_level) {
      cerr << " - skipping ..." << endl;
      continue;
    }

    for (BasicBlock::const_iterator J = block->begin(), JE = block->end();
        J != JE; ++J) {
      const Instruction& instr = *J;
      bool invariant = isInvariant(loop, invariants, &instr);
      if (invariant) {
        cerr << "   - INVARIANT" << endl;
        invariants.insert(&instr);
      }
    }
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
