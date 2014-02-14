// 15-745 S14 Assignment 2: util.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "util.h"


using std::cout;
using std::endl;


namespace llvm {


Assignments DataFlowUtil::uses(const BasicBlock& block) {
  Assignments useSet;
  for (BasicBlock::const_iterator it = block.begin(); it != block.end(); ++it) {
    const Instruction& instr = *it;
    const User* user = &instr;
    // iterate through all operands
    User::const_op_iterator OI, OE;
    for(OI = user->op_begin(), OE = user->op_end(); OI != OE; ++OI) {
      Value* val = *OI;
      // check if the operand is used
      if (isa<Instruction>(val) || isa<Argument>(val)) {
        useSet.insert(Assignment(val));
      }
    }
  }
  return useSet;
}


Assignments DataFlowUtil::defines(const BasicBlock& block) {
  Assignments defSet;
  for (BasicBlock::const_iterator it = block.begin(); it != block.end(); ++it) {
    // there's no result area for an instr, every instruction is actually a definition
    const Instruction& instr = *it;
    const Value* val = &instr;
    defSet.insert(Assignment(val));
  }
  return defSet;
}

Assignments DataFlowUtil::kills(const BasicBlock& block) {
  Assignments killSet;
  const Function* function = block.getParent();
  for (BasicBlock::const_iterator it = block.begin(); it != block.end(); ++it) {
    const Instruction& instr = *it;
    const Value* val = &instr;
    killSet.insert(Assignment(val));
  }
  return killSet;
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
