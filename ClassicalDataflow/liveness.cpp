// 15-745 S14 Assignment 2: reaching-definitions.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "liveness.h"

namespace llvm {


Liveness::Liveness() : DataFlowPass(ID) {
  meet = INTERSECTION;
  direction = FORWARDS;
};


bool Liveness::runOnFunction(Function& F) {
  ExampleFunctionPrinter(errs(), F);
  // Did not modify the incoming Function.
  return false;
}


void Liveness::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
}


char Liveness::ID = 0;
RegisterPass<Liveness> X("live", "15745 Liveness");


}
