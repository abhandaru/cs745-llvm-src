// 15-745 S14 Assignment 2: reaching-definitions.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "reaching-definitions.h"


using std::cout;
using std::endl;


namespace llvm {


// TODO: Call DataFlowPass with the right arguments
ReachingDefinitions::ReachingDefinitions() :
    DataFlowPass(ID, NONE, INTERSECTION, FORWARDS) {
  cout << ">> Liveness() constructor" << endl;
};

Assignments ReachingDefinitions::generate(const BasicBlock& block) {
  return DataFlowUtil::uses(block);
}


Assignments ReachingDefinitions::kill(const BasicBlock& block) {
  return DataFlowUtil::uses(block);
}


bool ReachingDefinitions::runOnFunction(Function& F) {
  ExampleFunctionPrinter(errs(), F);

  // Did not modify the incoming Function.
  return false;
}


void ReachingDefinitions::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
}


char ReachingDefinitions::ID = 0;
RegisterPass<ReachingDefinitions> X("reach", "15745 ReachingDefinitions");


}
