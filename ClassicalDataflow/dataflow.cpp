// 15-745 S14 Assignment 2: dataflow.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

using std::cout;
using std::endl;

namespace llvm {


// Add code for your dataflow abstraction here (if necessary).

SmallSet<Value*, 10, std::less<Value*> > Gen(BasicBlock& block) {
  SmallSet<Value*, 10, std::less<Value*> > genSet;
  for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
    Instruction* instr = &(*it);
    if (StoreInst* store = dyn_cast<StoreInst>(instr)) {
      Value* pointer = store->getPointerOperand();
      if (!genSet.count(pointer)) {
        genSet.insert(pointer);
        cout << "haha" << endl;
      }
    }
  }
  return genSet;
}

void Kill(BasicBlock& B) {

}

SmallSet<Value*, 10, std::less<Value*> > Use(BasicBlock& block) {
  SmallSet<Value*, 10, std::less<Value*> > loadSet;
  for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
    Instruction* instr = &(*it);
    if (LoadInst* load = dyn_cast<LoadInst>(instr)) {
      Value* pointer = load->getPointerOperand();
      if (!loadSet.count(pointer)) {
        loadSet.insert(pointer);
        cout << "pupu" << endl;
      }
    }
  }
  return loadSet;
}

void PrintInstructionOps(raw_ostream& O, const Instruction* I) {
  O << "\nOps: {";
  if (I != NULL) {
    for (Instruction::const_op_iterator OI = I->op_begin(), OE = I->op_end();
        OI != OE; ++OI) {
      const Value* v = OI->get();
      v->print(O);
      O << ";";
    }
  }
  O << "}\n";
}

void DataFlowPass::ExampleFunctionPrinter(raw_ostream& O, const Function& F) {
  for (Function::const_iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
    const BasicBlock* block = FI;
    O << block->getName() << ":\n";
    const Value* blockValue = block;
    PrintInstructionOps(O, NULL);
    for (BasicBlock::const_iterator BI = block->begin(), BE = block->end();
        BI != BE; ++BI) {
      BI->print(O);
      PrintInstructionOps(O, &(*BI));
    }
  }
}

void DataFlowPass::PrintInstructionOps(raw_ostream& O, const Instruction* I) {
  O << "\nOps: {";
  if (I != NULL) {
    for (Instruction::const_op_iterator OI = I->op_begin(), OE = I->op_end();
        OI != OE; ++OI) {
      const Value* v = OI->get();
      v->print(O);
      O << ";";
    }
  }
  O << "}\n";
}

}
