// 15-745 S14 Assignment 2: util.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "util.h"


using std::cout;
using std::endl;


namespace llvm {


Assignments DataFlowUtil::uses(const BasicBlock& block) {
  cout << ">>     Uses:" << endl;
  Assignments loads;
  for (BasicBlock::const_iterator it = block.begin(); it != block.end(); ++it) {
    const Instruction& instr = *it;
    if (const LoadInst* load = dyn_cast<LoadInst>(&instr)) {
      const Value* pointer = load->getPointerOperand();
      Assignment assign(pointer);
      if (!loads.count(assign)) {
        loads.insert(assign);
        load->dump();
      }
    }
  }
  return loads;
}


Assignments DataFlowUtil::defines(const BasicBlock& block) {
  cout << ">>     Defines:" << endl;
  Assignments stores;
  for (BasicBlock::const_iterator it = block.begin(); it != block.end(); ++it) {
    const Instruction& instr = *it;
    if (const StoreInst* store = dyn_cast<StoreInst>(&instr)) {
      const Value* pointer = store->getPointerOperand();
      Assignment assign(pointer);
      if (!stores.count(assign)) {
        stores.insert(assign);
        store->dump();
      }
    }
  }
  return stores;
}


}
