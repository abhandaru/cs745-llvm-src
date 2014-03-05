// 15-745 S14 Assignment 3: loop-invariant-code-motion.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef _LOOP_INVARIANT_CODE_MOTION_H_
#define _LOOP_INVARIANT_CODE_MOTION_H_

#include <iostream>
#include <vector>

#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManagers.h"
#include "llvm/Pass.h"

#include "dominance.h"
#include "util.h"

namespace llvm {

class LicmPass : public LoopPass {
 public:
  static char ID;
  LicmPass();
  virtual bool runOnLoop(Loop *loop, LPPassManager &LPM);
  virtual void getAnalysisUsage(AnalysisUsage& AU) const;

 protected:
  DominancePass dominance;
};

}

#endif
