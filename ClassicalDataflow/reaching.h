// 15-745 S14 Assignment 2: reaching-definitions.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef __REACHING_PASS_H__
#define __REACHING_PASS_H__

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"
#include "util.h"


namespace llvm {


class ReachingPass : public DataFlowPass {
 public:
  static char ID;
  ReachingPass();
  Assignments top(const BasicBlock& block);
  Assignments init(const BasicBlock& block);
  Assignments generate(const BasicBlock& block);
  Assignments kill(const BasicBlock& block);
  void transferFn(const Assignments& generate, const Assignments& kill,
    const Assignments& input, Assignments& output);
  // non-API methods
  Assignments forInstruction(const BlockState& state, const Instruction& instr);
};


}

#endif
