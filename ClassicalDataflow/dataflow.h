// 15-745 S14 Assignment 2: dataflow.h
// Group: bovik, bovik2
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_DATAFLOW_H__

#include <stdio.h>
#include <set>

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

class Assignment;
typedef std::set<Assignment> Assignments;

enum MeetOperator {
  INTERSECTION,
  UNION
};

enum Direction {
  FORWARDS,
  BACKWARDS
};

class Assignment {
 public:
  Value* pointer;
};

class GenKill {
 public:
  Assignments generate;
  Assignments kill;
};

class DataFlowPass : public FunctionPass {
 public:
  DataFlowPass(char ID) : FunctionPass(ID) { };
  void ExampleFunctionPrinter(raw_ostream& O, const Function& F);
  Assignments transferFunction(Assignments input);
  virtual Assignments generate(const BasicBlock& block);
  virtual Assignments kill(const BasicBlock& block);

  // data
  MeetOperator meet;
  Direction direction;
 private:
  void PrintInstructionOps(raw_ostream& O, const Instruction* I);
};

}

#endif
