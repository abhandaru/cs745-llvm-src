// 15-745 S14 Assignment 2: liveness.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "liveness.h"


using std::cout;
using std::endl;


namespace llvm {


Liveness::Liveness() : DataFlowPass(ID, NONE, INTERSECTION, FORWARDS) {
  cout << ">> Liveness() constructor" << endl;
};


Assignments Liveness::generate(const BasicBlock& block) {
  // TODO: This is just an example.
  return DataFlowUtil::uses(block);
}


Assignments Liveness::kill(const BasicBlock& block) {
  // TODO: This is just an example.
  return DataFlowUtil::defines(block);
}


bool Liveness::runOnFunction(Function& F) {
  // ExampleFunctionPrinter(errs(), F);
  // Does not modify the incoming Function.
  return false;
}


void Liveness::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
}


char Liveness::ID = 0;
RegisterPass<Liveness> X("live", "15745 Liveness");


}
