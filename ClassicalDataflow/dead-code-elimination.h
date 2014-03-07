// 15-745 S14 Assignment 3: dead-code-elimination.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef _DEAD_CODE_ELIMINATION_H_
#define _DEAD_CODE_ELIMINATION_H_

#include <iostream>
// #include <set>
// #include <vector>

#include "llvm/IR/Function.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constants.h"
#include "llvm/Pass.h"

#include "dataflow.h"
#include "util.h"

namespace llvm {

class DcePass : public DataFlowPass {
 public:
  static char ID;
  DcePass();
  Assignments top(const BasicBlock& block);
  Assignments init(const BasicBlock& block);
  Assignments generate(const BasicBlock& block);
  Assignments kill(const BasicBlock& block);
  void transferFn(const Assignments& generate, const Assignments& kill,
    const Assignments& input, Assignments& output);
  bool runOnFunction(Function& fn);
};

}

#endif
