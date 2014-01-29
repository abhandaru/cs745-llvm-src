// 15-745 S14 Assignment 1: FunctionInfo.cpp
// Group: bovik, bovik2
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"

#include <ostream>
#include <fstream>
#include <iostream>

// useful headers
using namespace llvm;
using std::cout;
using std::endl;

namespace {

class LocalOpts : public ModulePass {

//
// Private helper functions
//

 private:
  void getFunctionInfo(Module& module, Function& function) {
    // useful information
    iplist<BasicBlock>& blocks = function.getBasicBlockList();

    // determine all quantities we need.
    bool is_var_arg = function.isVarArg();
    size_t arg_count = function.arg_size();
    size_t callsite_count = getCallCount(module, function);
    size_t block_count = blocks.size();
    size_t instruction_count = 0;
    for (iplist<BasicBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
      BasicBlock& block = *it;
      instruction_count += block.getInstList().size();
    }

    // output in specified format
    cout << function.getName().data() << ",\t";
    if (is_var_arg) {
      cout << "*,\t";
    } else {
      cout << arg_count << ",\t";
    }
    cout << callsite_count << ",\t";
    cout << block_count << ",\t";
    cout << instruction_count << endl;
  }

  size_t getCallCount(Module& module, Function& function) {
    size_t count = 0;
    for (Module::iterator fn = module.begin(); fn != module.end(); ++fn) {
      iplist<BasicBlock>& blocks = (*fn).getBasicBlockList();

      for (iplist<BasicBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        iplist<Instruction>& instructions = (*it).getInstList();

        for (iplist<Instruction>::iterator itr = instructions.begin(); itr != instructions.end(); ++itr) {
          Instruction* instr = &(*itr);
          if (CallInst* call = dyn_cast<CallInst>(instr)) {
            if (call->getCalledFunction() == &function)
              count++;
          }
        }
      }
    }

    // clean up
    return count;
  }

  // Output the function information to standard out.
  // This function name makes no sense.
  void printFunctionInfo(Module& M) { }


public:

  //
  // Generic setup stuff
  //
  static char ID;
  LocalOpts() : ModulePass(ID) { }
  ~LocalOpts() { }

  // We don't modify the program, so we preserve all analyses
  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
  }


  //
  // Assignment code
  //
  virtual bool runOnFunction(Function &F) {

    // always return false
    return false;
  }

  virtual bool runOnModule(Module& M) {
    std::cout << "Module " << M.getModuleIdentifier().c_str() << std::endl;
    std::cout << "Name,\tArgs,\tCalls,\tBlocks,\tInsns\n";

    // iterate through all functions in the module
    for (Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI) {
      getFunctionInfo(M, *MI);
    }

    // always return false in this example
    return false;
  }

};

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
RegisterPass<LocalOpts> X("local-opts", "15745: Local Optimizations");

}
