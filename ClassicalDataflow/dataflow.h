// 15-745 S14 Assignment 2: dataflow.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_DATAFLOW_H__

#include <iostream>
#include <queue>
#include <set>

#include "llvm/IR/Instructions.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/ValueMap.h"
#include "llvm/Support/CFG.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"

#include "util.h"



namespace llvm {


enum Meet {
  INTERSECTION,
  UNION
};

enum Direction {
  FORWARDS,
  BACKWARDS
};

enum Top {
  ALL,
  NONE
};

class DataFlowPass : public FunctionPass {
 public:
  DataFlowPass(char id, Top top, Meet meet, Direction direction);
  void computeGenKill(const BlockList& blocks, BlockStates& states);
  void traverseForwards(const BlockList& blocks, BlockStates& states);
  void traverseBackwards(const BlockList& blocks, BlockStates& states);
  void meetFunction(const Assignments& in, Assignments& out);
  void display(const BlockList& blocks, BlockStates& states);
  BlockStates runOnBlocks(const BlockList& blocks);

  // data flow API
  virtual Assignments top(const Function& fn) = 0;
  virtual Assignments generate(const BasicBlock& block) = 0;
  virtual Assignments kill(const BasicBlock& block) = 0;
  virtual void transferFn(const Assignments& generate, const Assignments& kill,
      const Assignments& input, Assignments& output) = 0;

  // pass API
  virtual bool runOnFunction(Function& F);
  virtual void getAnalysisUsage(AnalysisUsage& AU) const;

 protected:
  const Top _top;
  const Meet _meet;
  const Direction _direction;
};


}

#endif
