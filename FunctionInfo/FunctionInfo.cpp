// 15-745 S14 Assignment 1: FunctionInfo.cpp
// Group: bovik, bovik2
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"

#include <ostream>
#include <fstream>
#include <iostream>

// useful headers
using namespace llvm;
using std::cout;
using std::endl;

namespace {

class FunctionInfo : public ModulePass {

  // Output the function information to standard out.
  // This function name makes no sense.
  void printFunctionInfo(Module& M) { }

public:

  //
  // Generic setup stuff
  //
  static char ID;
  FunctionInfo() : ModulePass(ID) { }
  ~FunctionInfo() { }

  // We don't modify the program, so we preserve all analyses
  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
  }


  //
  // Assignment code
  //
  virtual bool runOnFunction(Function &F) {
    std::cout << "Name,\tArgs,\tCalls,\tBlocks,\tInsns\n";

    // useful information
    iplist<BasicBlock>& blocks = F.getBasicBlockList();

    // determine all quantities we need.
    bool is_var_arg = F.isVarArg();
    size_t arg_count = F.getArgumentList().size();
    size_t callsite_count = 0;
    size_t block_count = blocks.size();
    size_t instruction_count = 0;
    for (iplist<BasicBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
      BasicBlock& block = *it;
      iplist<Instruction>& instList = block.getInstList();
      instruction_count += instList.size();
    }

    // output in specified format
    std::cout << "function_name" << ",\t";
    if (is_var_arg) {
      std::cout << "*,\t";
    } else {
      std::cout << arg_count << ",\t";
    }
    std::cout << callsite_count << ",\t" << block_count << ",\t"
        << instruction_count << std::endl;

    // always return false
    return false;
  }

  virtual bool runOnModule(Module& M) {
    std::cerr << "15745 Function Information Pass\n"; // TODO: remove this.
    std::cout << "Module " << M.getModuleIdentifier().c_str() << std::endl;

    // iterate through all functions in the module
    for (Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI) {
      runOnFunction(*MI);
    }

    // always return false in this example
    return false;
  }

};

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char FunctionInfo::ID = 0;
RegisterPass<FunctionInfo> X("function-info", "15745: Function Information");

}
