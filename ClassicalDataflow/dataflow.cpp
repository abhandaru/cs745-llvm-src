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


void DataFlowPass::transferFunction(const Assignments& generate,
    const Assignments& kill, Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  if (_meet == UNION) {
    DataFlowUtil::setUnion(output, generate);
  } else if (_meet == INTERSECTION) {
    DataFlowUtil::setIntersect(output, generate);
  }
}


bool DataFlowPass::runOnFunction(Function& fn) {
  // ExampleFunctionPrinter(errs(), fn);
  cout << "  Function: " << fn.getName().data() << endl;

  // First pass: precompute generate and kill sets.
  BlockStates states;
  for (Function::const_iterator FI = fn.begin(), FE = fn.end(); FI != FE; ++FI) {
    const BasicBlock& block = *FI;
    cout << "    Block: " << block.getName().data() << endl;
    BlockState state;
    state.generates = generate(block);
    state.kills = kill(block);
    // insert into map
    states.insert(BlockStatePair(&block, state));
    cout << endl;
  }

  // iterate for a forwards pass
  if (_direction == FORWARDS) {
    const BasicBlock& start = fn.front();

  // iterate for a backwards pass
  } else if (_direction == BACKWARDS) {

    // set up initial conditions.
    // TODO: Set start_state.out = to correct TOP.
    const BasicBlock& start = fn.back();
    BlockState& start_state = states[&start];

    // start at the back and traverse inverse graph
    std::queue<const BasicBlock*> worklist;
    worklist.push(&start);

    while (!worklist.empty()) {
      // inspect 1st element
      const BasicBlock* current = worklist.front();
      BlockState& state = states[current];
      cout << "    Block: " << current->getName().data() << endl;
      worklist.pop();



      for (const_pred_iterator I = pred_begin(current), IE = pred_end(current);
          I != IE; ++I) {
        const BasicBlock* predecessor = *I;



      }
    }
  }

  // Does not modify the incoming Function.
  cout << endl;
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
