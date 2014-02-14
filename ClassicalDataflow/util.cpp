// 15-745 S14 Assignment 2: util.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "util.h"


using std::cout;
using std::endl;


namespace llvm {


Assignments DataFlowUtil::uses(const BasicBlock& block) {
  cout << "      Uses:" << endl;
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
  cout << "      Defines:" << endl;
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


Assignments DataFlowUtil::all(const Function& fn) {
  Assignments all;

  // Function arguments are values too.
  for (Function::const_arg_iterator I = fn.arg_begin(), IE = fn.arg_end();
      I != IE; ++I) {
    const Argument* arg = &(*I);
    Assignment assign(arg);
    all.insert(assign);
  }

  // Populate with all instructions.
  // TODO: What do we do with loads and stores?
  for (Function::const_iterator I = fn.begin(), IE = fn.end(); I != IE; ++I) {
    const BasicBlock& block = *I;
    for (BasicBlock::const_iterator J = block.begin(), JE = block.end();
        J != JE; ++J) {
      const Instruction* instr = &(*J);
      Assignment assign(instr);
      all.insert(assign);
    }
  }
  return all;
}


void DataFlowUtil::setSubtract(Assignments& dest, const Assignments& src) {
  for (Assignments::const_iterator i = src.begin(); i != src.end(); ++i) {
    const Assignment& sub = *i;
    dest.erase(sub);
  }
}


void DataFlowUtil::setUnion(Assignments& dest, const Assignments& src) {
  for (Assignments::const_iterator i = src.begin(); i != src.end(); ++i) {
    const Assignment& add = *i;
    dest.insert(add);
  }
}


void DataFlowUtil::setIntersect(Assignments& dest, const Assignments& src) {
  for (Assignments::const_iterator i = src.begin(); i != src.end(); ++i) {
    const Assignment& test = *i;
    if (dest.count(test) < 1) {
      dest.erase(test);
    }
  }
}


bool DataFlowUtil::setEquals(const Assignments& a, const Assignments& b) {
  // make sure sets are the same length
  if (a.size() != b.size()) {
    return false;
  }

  // ensure they contain the same elements
  for (Assignments::const_iterator i = a.begin(); i != a.end(); ++i) {
    const Assignment& test = *i;
    if (b.count(test) < 1) {
      return false;
    }
  }
  return true;
}


}
