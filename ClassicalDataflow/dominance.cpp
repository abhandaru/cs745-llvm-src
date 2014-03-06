// 15-745 S14 Assignment 3: dominance.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "dominance.h"


using std::cerr;
using std::cout;
using std::endl;


namespace llvm {


DominancePass::DominancePass() : DataFlowPass(ID, INTERSECTION, FORWARDS) { };


//
// Override the function for generating the top set for the pass.
//
Assignments DominancePass::top(const BasicBlock& block) {
  	return Assignments();
}


//
// Override the init function for BlockStates
//
Assignments DominancePass::init(const BasicBlock& block) {
  const Function& fn = *(&block)->getParent();
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
  Assignments genSet;
  genSet.insert(Assignment(&block));
  return genSet;
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
  DataFlowUtil::setUnion(output, generate);
}


const BasicBlock* DominancePass::getIdom(BlockStates& states,
    const BasicBlock* node) {
  std::queue<const BasicBlock*> worklist;
  std::set<const BasicBlock*> visited;
  // Set initial conditions.
  Assignments stops = states[node].out;
  worklist.push(node);
  while (!worklist.empty()) {
    const BasicBlock* current = worklist.front();
    worklist.pop();
    // Skip if visited.
    if (visited.count(current)) {
      continue;
    }
    // Did we find the idom?
    if (node != current && stops.count(Assignment(current)) > 0) {
      return current;
    }
    // Mark visited and add all predecessors to the worklist.
    visited.insert(current);
    for (const_pred_iterator I = pred_begin(current), IE = pred_end(current);
        I != IE; ++I) {
      worklist.push(*I);
    }
  }
  // Return null if there is no idom node (possible for 1st block).
  return NULL;
}


//
// Do the following to meet the FunctionPass API
//
char DominancePass::ID = 0;
RegisterPass<DominancePass> X("dominance", "15745 DominancePass");


}
