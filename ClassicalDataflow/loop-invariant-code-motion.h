// 15-745 S14 Assignment 3: loop-invariant-code-motion.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef _LOOP_INVARIANT_CODE_MOTION_H_
#define _LOOP_INVARIANT_CODE_MOTION_H_

#include <iostream>
#include <set>
#include <vector>

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LegacyPassManagers.h"
#include "llvm/Pass.h"

#include "dominance.h"
#include "reaching.h"
#include "util.h"

namespace llvm {

class LicmPass : public LoopPass {
 public:
  static char ID;
  LicmPass();
  void showDominators(const BlockVector& blocks, const BasicBlock* preheader);

  // LoopPass API
  virtual bool runOnLoop(Loop *loop, LPPassManager &LPM);
  virtual void getAnalysisUsage(AnalysisUsage& AU) const;

 private:
  bool isInvariant(Loop* loop, std::set<const Instruction*>& invariants,
    const Instruction* instr);
  bool isInvariant(Loop* loop, std::set<const Instruction*>& invariants,
    const Value* operand);
  bool isTopLevel(const Loop* loop, const BasicBlock* block);

  // pass instances
  DominancePass dominance;
  ReachingPass reaching;

  // data from LLVM passes
  LoopInfo* info;
};

}

#endif
