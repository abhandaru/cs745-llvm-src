// 15-745 S14 Assignment 2: dominance.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "dominance.h"


using std::cout;
using std::endl;


namespace llvm {


DominancePass::DominancePass() : DataFlowPass(ID, UNION, BACKWARDS) { };


//
// Override the function for generating the top set for the pass.
//
Assignments DominancePass::top(const BasicBlock& block) {
  const Function* fn = block.getParent(); // should never be null
  return DataFlowUtil::all(*fn);          // or we will segfault here
}


//
// Override generate function of DataFlowPass to use uses().
//
Assignments DominancePass::generate(const BasicBlock& block) {
  return DataFlowUtil::uses(block);
}


//
// Override generate function of DataFlowPass to use defines().
// Notation and naming may change later.
//
Assignments DominancePass::kill(const BasicBlock& block) {
  return DataFlowUtil::defines(block);
}


//
// Subclasses override the transfer function.
// More transparent way to provide function pointers.
//
void DominancePass::transferFn(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  meetFunction(generate, output);
}


//
// Do the following to meet the FunctionPass API
//
char DominancePass::ID = 0;
RegisterPass<DominancePass> X("dominance", "15745 DominancePass");


}
