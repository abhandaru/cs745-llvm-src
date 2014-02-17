// 15-745 S14 Assignment 2: reaching-definitions.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef __REACHING_DEFINITIONS_H__
#define __REACHING_DEFINITIONS_H__

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"
#include "util.h"


namespace llvm {


class ReachingDefinitions : public DataFlowPass {
 public:
  static char ID;
  ReachingDefinitions();
  Assignments generate(const BasicBlock& block);
  Assignments kill(const BasicBlock& block);
  void transferFn(const Assignments& generate, const Assignments& kill,
    const Assignments& input, Assignments& output);
};


}

#endif
