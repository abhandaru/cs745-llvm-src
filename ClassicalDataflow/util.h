// 15-745 S14 Assignment 2: util.h
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <set>
#include <utility>

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
  Assignment(const Value *ptr) : pointer(ptr) { };
  const Value* pointer;
  inline bool operator==(const Assignment& rhs) const {
    return pointer == rhs.pointer;
  }
  inline bool operator<(const Assignment& rhs) const {
    return pointer < rhs.pointer;
  }
};

typedef SmallSet<Assignment, 10, std::less<Assignment> > Assignments;

class BlockState {
 public:
  Assignments in;
  Assignments out;
  Assignments generates;
  Assignments kills;
};

typedef DenseMap<const BasicBlock*, BlockState> BlockStates;
typedef std::pair<const BasicBlock*, BlockState> BlockStatePair;


//
// Static library functions (for reuse)
//

class DataFlowUtil {
 public:
  static Assignments uses(const BasicBlock& block);
  static Assignments defines(const BasicBlock& block);
  static void setSubtract(Assignments& dest, const Assignments& src);
  static void setUnion(Assignments& dest, const Assignments& src);
  static void setIntersect(Assignments& dest, const Assignments& src);
};


}

#endif
