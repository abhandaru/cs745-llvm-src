// 15-745 S14 Assignment 2: util.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <set>

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"


namespace llvm {


//
// Useful types
//

class Assignment {
 public:
  Assignment(Value *ptr) : pointer(ptr) { };
  Value* pointer;
  inline bool operator==(const Assignment& rhs) {
    return pointer == rhs.pointer;
  }
};

typedef SmallSet<Assignment, 10, std::less<Assignment> > Assignments;

class GenKill {
 public:
  Assignments generate;
  Assignments kill;
};


//
// Static library functions (for reuse)
//

class DataFlowUtil {
 public:
  static Assignments uses(const BasicBlock& block);
  static Assignments defines(const BasicBlock& block);
};

}


#endif
