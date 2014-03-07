// 15-745 S14 Assignment 3: dominance.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef __DOMINANCE_PASS_H__
#define __DOMINANCE_PASS_H__

#include <set>
#include <queue>
#include <vector>

#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"

#include "dataflow.h"
#include "util.h"

namespace llvm {

class DominancePass : public DataFlowPass {
 public:
  // datatypes
  class Node;
  class Node {
   public:
    Node(const Node* _parent, const BasicBlock* _data)
        : data(_data), parent(_parent) { };
    ~Node() {
      for (int i = 0; i < children.size(); i++) {
        delete children[i];
      }
    };
    const BasicBlock* data;
    const Node* parent;
    std::vector<Node *> children;
  };

  // methods
  static char ID;
  DominancePass();
  Assignments top(const BasicBlock& block);
  Assignments init(const BasicBlock& block);
  Assignments generate(const BasicBlock& block);
  Assignments kill(const BasicBlock& block);
  void transferFn(const Assignments& generate, const Assignments& kill,
    const Assignments& input, Assignments& output);

  // deriving tree
  DominancePass::Node getDominatorTree(const BlockVector& blocks,
    BlockStates& states, const BasicBlock* preheader);
  const BasicBlock* getIdom(BlockStates& states, const BasicBlock* node);
};


}

#endif
