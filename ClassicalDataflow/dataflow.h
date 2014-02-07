// 15-745 S14 Assignment 2: dataflow.h
// Group: bovik, bovik2
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_DATAFLOW_H__

#include <stdio.h>

#include "llvm/IR/Instructions.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/ValueMap.h"
#include "llvm/Support/CFG.h"
#include "llvm/Pass.h"

#include <ostream>
#include <fstream>
#include <iostream>


namespace llvm {

enum MeetOperator {
  INTERSECTION,
  UNION
};

enum Direction {
  FORWARDS,
  BACKWARDS
};

class TranferFunction {

};

class DataFlowPass : public FunctionPass {
 public:
  DataFlowPass(char ID) : FunctionPass(ID) { };
  void ExampleFunctionPrinter(raw_ostream& O, const Function& F);
 private:
  void PrintInstructionOps(raw_ostream& O, const Instruction* I);
};

}

#endif
