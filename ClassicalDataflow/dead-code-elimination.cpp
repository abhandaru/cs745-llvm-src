// 15-745 S14 Assignment 3: dead-code-elimination.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "dead-code-elimination.h"


using std::cerr;
using std::cout;
using std::endl;


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
// Methods for smaller granularities.
//
// Assignments DcePass::forInstruction(const BlockState& state,
//     const Instruction& instr) {
//   instr.dump();
// }
bool DcePass::runOnFunction(Function& fn) {
  cout << "Function: " << fn.getName().data() << endl << endl;
  BlockList& blocks = fn.getBasicBlockList();
  BlockStates states = runOnBlocks(blocks);
  std::set<const Assignment*> lived;
  for (BlockList::const_iterator I = blocks.begin(), IE = blocks.end();
      I != IE; ++I) {
    const BasicBlock* block = &(*I);
    BlockState& state = states[block];

    Assignments blockOut = state.out;
    for(BasicBlock::const_reverse_iterator itrB = (*block).rbegin(); itrB != (*block).rend(); ++itrB) {
      const Instruction& instr = *itrB;
      
    }


  }
  
  return false;
}

//
// Do the following to meet the FunctionPass API
//
char DcePass::ID = 0;
RegisterPass<DcePass> W("cd-dce", "15745 DcePass");


}
