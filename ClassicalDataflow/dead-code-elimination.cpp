// 15-745 S14 Assignment 3: dead-code-elimination.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "dead-code-elimination.h"


using std::cerr;
using std::cout;
using std::endl;
using std::set;


namespace llvm {


DcePass::DcePass() :
    DataFlowPass(ID, UNION, BACKWARDS) { };


//
// Override the function for generating the top set for the pass.
//
Assignments DcePass::top(const BasicBlock& block) {
    return Assignments();
}


//
// Override the init function for BlockStates
//
Assignments DcePass::init(const BasicBlock& block) {
  return Assignments();
}


//
// Override generate function of DataFlowPass to use defines().
//
Assignments DcePass::generate(const BasicBlock& block) {
  return DataFlowUtil::uses(block);
}


//
// Override generate function of DataFlowPass to use kills().
// Notation and naming may change later.
//
Assignments DcePass::kill(const BasicBlock& block) {
  return DataFlowUtil::defines(block);
}


//
// Subclasses override the transfer function.
// More transparent way to provide function pointers.
//
void DcePass::transferFn(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  meetFn(generate, output);
}


//
// String of checks for a dead assignment.
//
bool DcePass::isDead(Instruction* instr) {
  if (!instr->use_empty())
    return false;
  if (isa<TerminatorInst>(instr))
    return false;
  if (isa<DbgInfoIntrinsic>(instr))
    return false;
  if (isa<LandingPadInst>(instr))
    return false;
  if (instr->mayHaveSideEffects())
    return false;
  return true;
}


//
// Methods for smaller granularities.
//
bool DcePass::runOnFunction(Function& fn) {
  cerr << "Function: " << fn.getName().data() << endl;
  BlockList& blocks = fn.getBasicBlockList();
  BlockStates states = runOnBlocks(blocks);
  set<Instruction*> dead;
  bool changed = false;

  cerr << "- Instructions to remove:" << endl;
  for (BlockList::iterator I = blocks.begin(), IE = blocks.end();
      I != IE; ++I) {
    BasicBlock* block = &(*I);
    dead.clear();
    // Iterate through instructions and inspect.
    for(BasicBlock::iterator J = block->begin(), JE = block->end();
        J != JE; ++J) {
      Instruction* instr = &(*J);
      if (isDead(instr)) {
        dead.insert(instr);
      }
    }
    // Remove all marked instructions.
    for(set<Instruction*>::iterator I = dead.begin(), IE = dead.end();
        I != IE; ++I) {
      Instruction* instr = *I;
      instr->dump();
      instr->eraseFromParent();
      changed = true;
    }
  }

  // Return whether or not we removed any instructions.
  cerr << endl;
  return changed;
}

//
// Do the following to meet the FunctionPass API
//
char DcePass::ID = 0;
RegisterPass<DcePass> W("cd-dce", "15745 DcePass");


}
