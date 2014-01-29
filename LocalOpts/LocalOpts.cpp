#include "LocalOpts.h"

// useful headers
using namespace llvm;
using std::cout;
using std::endl;

namespace local {

// We don't modify the program, so we preserve all analyses
// TODO: Change so we can actually modify.
void LocalOpts::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

bool LocalOpts::runOnModule(Module& module) {
  std::cout << "Module " << module.getModuleIdentifier().c_str() << std::endl;

  for (Module::iterator it = module.begin(); it != module.end(); ++it) {
    cout << (*it).getName().data() << endl;
  }

  return false;
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
RegisterPass<LocalOpts> X("local-opts", "15745: Local Optimizations");

}
