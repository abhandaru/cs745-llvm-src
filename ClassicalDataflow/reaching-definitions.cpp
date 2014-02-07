// 15-745 S14 Assignment 2: reaching-definitions.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "reaching-definitions.h"

namespace llvm {


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
