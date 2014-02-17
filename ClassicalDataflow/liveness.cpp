// 15-745 S14 Assignment 2: liveness.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "liveness.h"


using std::cout;
using std::endl;


namespace llvm {


Liveness::Liveness() : DataFlowPass(ID, NONE, UNION, BACKWARDS) { };


//
// Override generate function of DataFlowPass to use uses().
//
Assignments Liveness::generate(const BasicBlock& block) {
  return DataFlowUtil::uses(block);
}


//
// Override generate function of DataFlowPass to use defines().
// Notation and naming may change later.
//
Assignments Liveness::kill(const BasicBlock& block) {
  return DataFlowUtil::defines(block);
}


//
// Subclasses override the transfer function.
// More transparent way to provide function pointers.
//
void Liveness::transferFn(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  meetFunction(generate, output);
}


//
// Do the following to meet the FunctionPass API
//
char Liveness::ID = 0;
RegisterPass<Liveness> X("live", "15745 Liveness");


}
