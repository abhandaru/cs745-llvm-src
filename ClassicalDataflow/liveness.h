// 15-745 S14 Assignment 2: liveness.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef __LIVENESS_H__
#define __LIVENESS_H__

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"
#include "util.h"

namespace llvm {


class Liveness : public DataFlowPass {
 public:
  static char ID;
  Liveness();
  Assignments generate(const BasicBlock& block);
  Assignments kill(const BasicBlock& block);

  // pass API
  virtual bool runOnFunction(Function& F);
  virtual void getAnalysisUsage(AnalysisUsage& AU) const;
};


}

#endif
