// 15-745 S14 Assignment 2: dataflow.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"


using std::cout;
using std::endl;


namespace llvm {


DataFlowPass::DataFlowPass(char id, Top top, Meet meet, Direction direction) :
    FunctionPass(id),
    _top(top),
    _meet(meet),
    _direction(direction) {
  cout << ">> DataFlowPass() constructor" << endl;
};


bool DataFlowPass::runOnFunction(Function& fn) {
  // ExampleFunctionPrinter(errs(), fn);
  cout << ">> Function: " << fn.getName().data() << endl;

  BlockStates states;
  for (Function::const_iterator FI = fn.begin(), FE = fn.end(); FI != FE; ++FI) {
    const BasicBlock& block = *FI;
    BlockState state;
    state.generates = generate(block);
    state.kills = kill(block);
    // insert into map
    states.insert(BlockStatePair(&block, state));
    cout << ">>   Block:" << endl;
  }

  // iterate in correct direction
  if (_direction == FORWARDS) {
    const BasicBlock& start = fn.front();
  } else if (_direction == BACKWARDS) {
    const BasicBlock& start = fn.back();
  }

  // Does not modify the incoming Function.
  return false;
}


void DataFlowPass::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
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
