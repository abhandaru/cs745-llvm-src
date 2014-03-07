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


//
// This function derives the dominator tree for a given set of blocks and
// the computed block states resulting from this pass.
//
DominancePass::Node DominancePass::getDominatorTree(const BlockVector& blocks,
    BlockStates& states, BasicBlock* preheader) {
  // initial state
  DenseMap<BasicBlock*, Node*> visited;
  std::vector<BasicBlock*> stack;
  Node* root = new Node(NULL, preheader);
  visited[preheader] = root;

  // this loop runs until we have visited all nodes
  for (int i = 0; i < blocks.size(); i++) {
    BasicBlock* current = blocks[i];
    stack.clear();

    // create depth stack
    while (current && !visited.count(current)) {
      stack.push_back(current);
      BasicBlock* idom = getIdom(states, current);
      // edge case when we do not have an idom
      if (idom) current = idom;
      else current = preheader;
    }

    // pop off stack while creating nodes
    while (!stack.empty()) {
      BasicBlock* block = stack.back();
      stack.pop_back();
      // parent is the last node we visited
      Node* parent = visited[current];
      Node* node = new Node(parent, block);
      parent->children.push_back(node);
      // set up next iteration
      visited[block] = node;
      current = block;
    }
  }

  // Return a copy of the root object.
  // When the copy goes out of scope, the Node destructor will run and
  // free up all the memory we allocated.
  return *root;
}


//
// For a given node, find the immediate dominator in the tree using a reverse
// breadth first search. If the current node is not the initial node, and the
// current node is in the initial node's strict dominators list, return the
// current node.
//
BasicBlock* DominancePass::getIdom(BlockStates& states, BasicBlock* node) {
  std::queue<BasicBlock*> worklist;
  std::set<BasicBlock*> visited;
  // Set initial conditions.
  Assignments stops = states[node].out;
  worklist.push(node);
  while (!worklist.empty()) {
    BasicBlock* current = worklist.front();
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
    for (pred_iterator I = pred_begin(current), IE = pred_end(current);
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
