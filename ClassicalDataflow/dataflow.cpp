// 15-745 S14 Assignment 2: dataflow.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

using std::cout;
using std::endl;


namespace llvm {


//
// Subclasses must call this constructor during construction.
// This constructor calls the FunctionPass constructor (it extends that class).
// Use initializer listed to assign constants on instantiation.
//
DataFlowPass::DataFlowPass(char id, Top top, Meet meet, Direction direction) :
    FunctionPass(id),
    _top(top),
    _meet(meet),
    _direction(direction) { };


//
// Compute the generate and kill sets for each basic block in the given
// function. The generate and kill functions are overriden by the subclass.
//
void DataFlowPass::computeGenKill(const BlockList& blocks, BlockStates& states) {
  for (BlockList::const_iterator FI = blocks.begin(), FE = blocks.end();
      FI != FE; ++FI) {
    const BasicBlock& block = *FI;
    BlockState state;
    state.generates = generate(block);
    state.kills = kill(block);
    if (_direction == FORWARDS)
      state.out = init(block);
    else 
      state.in = init(block);
    // insert into map
    states.insert(BlockStatePair(&block, state));
  }
}


//
// Do a forwards traversal on the Control Flow Graph to perform the given
// analysis. The BlockState map is updated during the traversal.
//
void DataFlowPass::traverseForwards(const BlockList& blocks, BlockStates& states) {
  std::queue<const BasicBlock*> worklist;
  std::set<const BasicBlock*> visited;

  // Set up initial conditions.
  const BasicBlock* start = &(blocks.front());
  BlockState& start_state = states[start];

  Assignments topSet = top(*start);
  start_state.out = topSet;
  worklist.push(start);

  // Process queue until it is empty.
  while (!worklist.empty()) {
    // inspect 1st element
    const BasicBlock* current = worklist.front();
    worklist.pop();

    // determine the meet of all successors
    Assignments meet = topSet;
    for (const_pred_iterator I = pred_begin(current), IE = pred_end(current);
        I != IE; ++I) {
      BlockState& pred_state = states[*I];
      meetFunction(pred_state.out, meet);
    }

    // see if we need to inspect this node
    BlockState& state = states[current];
    if (visited.count(current) && DataFlowUtil::setEquals(state.in, meet)) {
      continue;
    }

    // perform transfer function
    state.in = meet;
    visited.insert(current);
    transferFn(state.generates, state.kills, state.in, state.out);

    // Add all predecessors to the worklist.
    for (succ_const_iterator I = succ_begin(current), IE = succ_end(current);
        I != IE; ++I) {
      worklist.push(*I);
    }
  }
}


//
// Do a backwards traversal on the Control Flow Graph to perform the given
// analysis. The BlockState map is updated during the traversal.
//
void DataFlowPass::traverseBackwards(const BlockList& blocks, BlockStates& states) {
  std::queue<const BasicBlock*> worklist;
  std::set<const BasicBlock*> visited;

  // Set up initial conditions.
  const BasicBlock* start = &(blocks.back());
  BlockState& start_state = states[start];

  Assignments topSet = top(*start);
  DataFlowUtil::print(topSet);
  start_state.in = topSet;
  worklist.push(start);

  // Process queue until it is empty.
  while (!worklist.empty()) {
    // inspect 1st element
    const BasicBlock* current = worklist.front();
    worklist.pop();

    // determine the meet of all successors
    Assignments meet = topSet;
    for (succ_const_iterator I = succ_begin(current), IE = succ_end(current);
        I != IE; ++I) {
      BlockState& succ_state = states[*I];
      meetFunction(succ_state.in, meet);
    }

    // See if we need to inspect this node.
    BlockState& state = states[current];
    if (visited.count(current) && DataFlowUtil::setEquals(state.in, meet)) {
      // cout << "      <input unchanged>" << endl;
      continue;
    }

    // Perform transfer function.
    state.out = meet;
    visited.insert(current);
    transferFn(state.generates, state.kills, state.out, state.in);

    // Add all predecessors to the worklist.
    for (const_pred_iterator I = pred_begin(current), IE = pred_end(current);
        I != IE; ++I) {
      worklist.push(*I);
    }
  }
}


//
// Performs the correct meet operation on two input sets.
// The output is stored in the second set. The first set is unmodified.
//
void DataFlowPass::meetFunction(const Assignments& in, Assignments& out) {
  if (_meet == UNION) {
    DataFlowUtil::setUnion(out, in);
  } else if (_meet == INTERSECTION) {
    DataFlowUtil::setIntersect(out, in);
  }
}


//
// Called after the pass is complete.
// Show the results of the pass for each program point b/t blocks.
//
void DataFlowPass::display(const BlockList& blocks, BlockStates& states) {
  for (BlockList::const_iterator I = blocks.begin(), IE = blocks.end();
      I != IE; ++I) {
    const BasicBlock* block = &(*I);
    BlockState& state = states[block];
    if (I == blocks.begin()) {
      DataFlowUtil::print(state.in);
      cout << endl;
    }
    block->dump();
    DataFlowUtil::print(state.out);
    cout << endl;
  }
  cout << endl;
}


BlockStates DataFlowPass::runOnBlocks(const BlockList& blocks) {
  BlockStates states;

  // First pass: precompute generate and kill sets.
  computeGenKill(blocks, states);
  cout << endl;

  // iterate for a forwards pass
  if (_direction == FORWARDS) {
    traverseForwards(blocks, states);
  }

  // iterate for a backwards pass
  else if (_direction == BACKWARDS) {
    traverseBackwards(blocks, states);
  }

  // return copy of states
  return states;
}


//
// Called by the FunctionPass API in LLVM.
//
bool DataFlowPass::runOnFunction(Function& fn) {
  cout << "Function: " << fn.getName().data() << endl << endl;
  BlockList& blocks = fn.getBasicBlockList();
  BlockStates states = runOnBlocks(blocks);

  // Does not modify the incoming Function.
  display(blocks, states);
  return false;
}


void DataFlowPass::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
}

}
