// 15-745 S14 Assignment 2: util.cpp
// Group: akbhanda, zheq
//
// Overview: This file contains useful typedefs and utility functions for the
//   the DataFlowPass class and its subclasses. The DataFlowUtil class is
//   completely stateless.
///////////////////////////////////////////////////////////////////////////////


#include "util.h"


using std::cerr;
using std::cout;
using std::endl;


namespace llvm {


//
// For a given BasicBlock, compute which variables are used.
//
Assignments DataFlowUtil::uses(const BasicBlock& block) {
  Assignments useSet;
  for (BasicBlock::const_reverse_iterator it = block.rbegin(); it != block.rend(); ++it) {
    const Instruction& instr = *it;
    const User* user = &instr;
    // iterate through all operands
    User::const_op_iterator OI, OE;
    for(OI = user->op_begin(), OE = user->op_end(); OI != OE; ++OI) {
      Value* val = *OI;
      // check if the operand is used
      if ((isa<Instruction>(val) || isa<Argument>(val)) && (!isa<PHINode>(val))) {
        useSet.insert(Assignment(val));
      }
    }
    useSet.erase(Assignment(&instr));
  }
  return useSet;
}


//
// For a given BasicBlock, compute which variables are defined.
//
Assignments DataFlowUtil::defines(const BasicBlock& block) {
  Assignments defSet;
  for (BasicBlock::const_iterator it = block.begin(); it != block.end(); ++it) {
    // there's no result area for an instr, every instruction is actually a definition
    const Instruction& instr = *it;
    defSet.insert(Assignment(&instr));
  }
  return defSet;
}


Assignments DataFlowUtil::kills(const BasicBlock& block) {
  Assignments killSet;
  const Function& function = *block.getParent();
  for (BasicBlock::const_iterator it = block.begin(); it != block.end(); ++it) {
    const Instruction& inst = *it;

    for(Function::const_iterator itrF = function.begin(); itrF != function.end(); ++itrF) {
      const BasicBlock& bb = *itrF;
      if (&bb == &block) {
        for(BasicBlock::const_iterator itrB = bb.begin(); itrB != bb.end(); ++itrB) {
          const Instruction& instr = *itrB;
          if (&inst == &instr) {
            killSet.insert(Assignment(&instr));
          }
        }
      }
    }
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
  // TODO: What do we do with loads and stores? Treat the same for now.
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


//
// The following functions perform basic set operations in O(n*log(m)) time,
// where m and n are the sizes of the sets. For our purposes, this is fast
// enough.
//
// The result of these operations is stored back into the 1st argument.
//
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
  Assignments result;
  for (Assignments::const_iterator i = src.begin(); i != src.end(); ++i) {
    const Assignment& test = *i;
    if (src.count(test) > 0 && dest.count(test) > 0) {
      result.insert(test);
    }
  }
  // rewrite the destination
  dest = result;
}


//
// Determine if 2 sets contain the same elements.
//
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


//
// Helper/debug function to quickly print out a set of assignments.
// Later if we change how we implement assignment sets, we will have to update
// this function.
//
void DataFlowUtil::print(const Assignments& assignments) {
  cerr << "{ ";
  for (Assignments::const_iterator I = assignments.begin(),
      IE = assignments.end(); I != IE; ++I) {
    cerr << (*I).pointer->getName().data() << " ";
  }
  cerr << "}";
}


}
