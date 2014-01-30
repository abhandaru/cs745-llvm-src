#ifndef __LOCAL_OPTS_H__
#define __LOCAL_OPTS_H__

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/User.h"

#include "llvm/ADT/ValueMap.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include <ostream>
#include <fstream>
#include <iostream>

namespace local {

class LocalOpts : public llvm::ModulePass {
 public:
  static char ID;
  LocalOpts() : ModulePass(ID) { };
  ~LocalOpts() { };
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
  virtual bool runOnModule(llvm::Module& module);
  void eachFunction(llvm::Function& function);
  void constantFolding(llvm::BasicBlock& block);
  void strengthReduction(llvm::BasicBlock& block);
  void algebraicIdentities(llvm::BasicBlock& block);

 private:
  uint64_t log2(uint64_t x);
  size_t strength;
  size_t fold;
  size_t algebra;
};

}

#endif