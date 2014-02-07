#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"

namespace llvm {

class Liveness : public DataFlowPass {
 public:
  static char ID;
  Liveness() : DataFlowPass(ID) { };
  virtual bool runOnFunction(Function& F);
  virtual void getAnalysisUsage(AnalysisUsage& AU) const;
};

}
