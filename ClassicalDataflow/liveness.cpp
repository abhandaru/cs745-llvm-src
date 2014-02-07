#include "liveness.h"

namespace llvm {


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
