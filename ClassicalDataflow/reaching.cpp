// 15-745 S14 Assignment 2=3: reaching.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "reaching.h"


using std::cerr;
using std::cout;
using std::endl;


namespace llvm {


ReachingPass::ReachingPass() :
    DataFlowPass(ID, UNION, FORWARDS) { };


//
// Override the function for generating the top set for the pass.
//
Assignments ReachingPass::top(const BasicBlock& block) {
    return Assignments();
}


//
// Override the init function for BlockStates
//
Assignments ReachingPass::init(const BasicBlock& block) {
  return Assignments();
}


//
// Override generate function of DataFlowPass to use defines().
//
Assignments ReachingPass::generate(const BasicBlock& block) {
  return DataFlowUtil::defines(block);
}


//
// Override generate function of DataFlowPass to use kills().
// Notation and naming may change later.
//
Assignments ReachingPass::kill(const BasicBlock& block) {
  return DataFlowUtil::kills(block);
}


//
// Subclasses override the transfer function.
// More transparent way to provide function pointers.
//
void ReachingPass::transferFn(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  DataFlowUtil::setUnion(output, generate);
}


//
// Methods for smaller granularities.
//
Assignments ReachingPass::forInstruction(const BlockState& state,
    const Instruction& instr) {
  instr.dump();
}


//
// Do the following to meet the FunctionPass API
//
char ReachingPass::ID = 0;
RegisterPass<ReachingPass> Z("reach", "15745 ReachingPass");


}
