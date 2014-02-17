// 15-745 S14 Assignment 2: reaching-definitions.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "reaching-definitions.h"


using std::cout;
using std::endl;


namespace llvm {


ReachingDefinitions::ReachingDefinitions() :
    DataFlowPass(ID, NONE, UNION, FORWARDS) { };


//
// Override generate function of DataFlowPass to use defines().
//
Assignments ReachingDefinitions::generate(const BasicBlock& block) {
  return DataFlowUtil::defines(block);
}


//
// Override generate function of DataFlowPass to use kills().
// Notation and naming may change later.
//
Assignments ReachingDefinitions::kill(const BasicBlock& block) {
  return DataFlowUtil::kills(block);
}


//
// Subclasses override the transfer function.
// More transparent way to provide function pointers.
//
void ReachingDefinitions::transferFn(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  meetFunction(generate, output);
}


//
// Do the following to meet the FunctionPass API
//
char ReachingDefinitions::ID = 0;
RegisterPass<ReachingDefinitions> X("reach", "15745 ReachingDefinitions");


}
