// 15-745 S14 Assignment 3: dominance.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "dominance.h"


using std::cout;
using std::endl;


namespace llvm {


DominancePass::DominancePass() : DataFlowPass(ID, ALL, INTERSECTION, FORWARDS) {
  cout << "DominancePass" << endl;
};


//
// Override the function for generating the top set for the pass.
//
Assignments DominancePass::top(const Function& fn) {
  Assignments all;
  for (Function::const_iterator I = fn.begin(), IE = fn.end(); I != IE; ++I) {
    const BasicBlock* block = I;
    Assignment assign(block);
    all.insert(assign);
  }
  return all;
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
  return Assignments();
}


//
// Subclasses override the transfer function.
// More transparent way to provide function pointers.
//
void DominancePass::transferFn(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  // DataFlowUtil::setSubtract(output, kill);
  DataFlowUtil::setUnion(output, generate);
}


//
// Do the following to meet the FunctionPass API
//
char DominancePass::ID = 0;
RegisterPass<DominancePass> X("dominance", "15745 DominancePass");


}
