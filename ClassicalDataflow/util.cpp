// 15-745 S14 Assignment 2: util.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "util.h"


using std::cout;
using std::endl;


namespace llvm {


Assignments DataFlowUtil::uses(const BasicBlock& block) {
  Assignments loads;
  // for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
  //   Instruction* instr = &(*it);
  //   if (LoadInst* load = dyn_cast<LoadInst>(instr)) {
  //     Value* pointer = load->getPointerOperand();
  //     Assignment assign(pointer);
  //     if (!loadSet.count(assign)) {
  //       loadSet.insert(assign);
  //       cout << "pupu" << endl;
  //     }
  //   }
  // }
  return loads;
}


Assignments DataFlowUtil::defines(const BasicBlock& block) {
  Assignments stores;
  // for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
  //   Instruction* instr = &(*it);
  //   if (StoreInst* store = dyn_cast<StoreInst>(instr)) {
  //     Value* pointer = store->getPointerOperand();
  //     Assignment assign(pointer);
  //     if (!stores.count(assign)) {
  //       stores.insert(assign);
  //       cout << "haha" << endl;
  //     }
  //   }
  // }
  return stores;
}


}
