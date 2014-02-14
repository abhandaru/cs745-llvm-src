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


void DataFlowPass::computeGenKill(const Function& fn, BlockStates& states) {
  for (Function::const_iterator FI = fn.begin(), FE = fn.end(); FI != FE; ++FI) {
    const BasicBlock& block = *FI;
    BlockState state;
    state.generates = generate(block);
    state.kills = kill(block);
    // insert into map
    states.insert(BlockStatePair(&block, state));
  }
}


void DataFlowPass::traverseForwards(const Function& fn, BlockStates& states) {
  std::queue<const BasicBlock*> worklist;
  std::set<const BasicBlock*> visited;

  // Set up initial conditions.
  Assignments top = getTop(fn);
  const BasicBlock* start = &(fn.front());
  BlockState& start_state = states[start];
  start_state.out = top;
  worklist.push(start);

  // Process queue until it is empty.
  while (!worklist.empty()) {
    // inspect 1st element
    const BasicBlock* current = worklist.front();
    cout << "    Block: " << current->getName().data() << endl;
    worklist.pop();

    // determine the meet of all successors
    Assignments meet = top;
    for (const_pred_iterator I = pred_begin(current), IE = pred_end(current);
        I != IE; ++I) {
      BlockState& pred_state = states[*I];
      meetFunction(pred_state.out, meet);
    }

    // see if we need to inspect this node
    BlockState& state = states[current];

    cout << "      meet: { ";
    for (Assignments::const_iterator i = meet.begin(); i != meet.end(); ++i) {
      cout << (*i).pointer->getName().data() << " ";
    }
    cout << "}" << endl;
    cout << "      in: { ";
    for (Assignments::const_iterator i = state.in.begin(); i != state.in.end(); ++i) {
      cout << (*i).pointer->getName().data() << " ";
    }
    cout << "}" << endl;

    if (visited.count(current) && DataFlowUtil::setEquals(state.in, meet)) {
      cout << "      <input unchanged>" << endl;
      continue;
    }

    // perform transfer function
    state.in = meet;
    visited.insert(current);
    transferFunction(state.generates, state.kills, state.in, state.out);

    //
    // Debug prints
    //
    cout << "      out: { ";
    for (Assignments::const_iterator i = state.out.begin(); i != state.out.end(); ++i) {
      cout << (*i).pointer->getName().data() << " ";
    }
    cout << "}" << endl;

    //
    // Add all predecessors to the worklist.
    //
    for (succ_const_iterator I = succ_begin(current), IE = succ_end(current);
        I != IE; ++I) {
      worklist.push(*I);
    }

    // print out the worklist queue
    cout << "      queue: { ";
    for (int i = 0; i < worklist.size(); ++i) {
      const BasicBlock* el = worklist.front();
      cout << el->getName().data() << " ";
      worklist.pop();
      worklist.push(el);
    }
    cout << "}" << endl;
  }
}


void DataFlowPass::traverseBackwards(const Function& fn, BlockStates& states) {
  std::queue<const BasicBlock*> worklist;
  std::set<const BasicBlock*> visited;

  // Set up initial conditions.
  Assignments top = getTop(fn);
  const BasicBlock* start = &(fn.back());
  BlockState& start_state = states[start];
  start_state.out = top;
  worklist.push(start);

  // Process queue until it is empty.
  while (!worklist.empty()) {
    // inspect 1st element
    const BasicBlock* current = worklist.front();
    cout << "    Block: " << current->getName().data() << endl;
    worklist.pop();

    // determine the meet of all successors
    Assignments meet = top;
    for (succ_const_iterator I = succ_begin(current), IE = succ_end(current);
        I != IE; ++I) {
      BlockState& succ_state = states[*I];
      meetFunction(succ_state.in, meet);
    }

    // See if we need to inspect this node.
    BlockState& state = states[current];

    cout << "      meet: { ";
    for (Assignments::const_iterator i = meet.begin(); i != meet.end(); ++i) {
      cout << (*i).pointer->getName().data() << " ";
    }
    cout << "}" << endl;
    cout << "      out: { ";
    for (Assignments::const_iterator i = state.out.begin(); i != state.out.end(); ++i) {
      cout << (*i).pointer->getName().data() << " ";
    }
    cout << "}" << endl;

    if (visited.count(current) && DataFlowUtil::setEquals(state.in, meet)) {
      cout << "      <input unchanged>" << endl;
      continue;
    }

    if (visited.count(current) && DataFlowUtil::setEquals(state.out, meet)) {
      continue;
    }

    // Perform transfer function.
    state.out = meet;
    visited.insert(current);
    transferFunction(state.generates, state.kills, state.out, state.in);

    cout << "      in: { ";
    for (Assignments::const_iterator i = state.in.begin(); i != state.in.end(); ++i) {
      cout << (*i).pointer->getName().data() << " ";
    }
    cout << "}" << endl;

    // Add all predecessors to the worklist.
    for (const_pred_iterator I = pred_begin(current), IE = pred_end(current);
        I != IE; ++I) {
      worklist.push(*I);
    }

    // print out the worklist queue
    cout << "      queue: { ";
    for (int i = 0; i < worklist.size(); ++i) {
      const BasicBlock* el = worklist.front();
      cout << el->getName().data() << " ";
      worklist.pop();
      worklist.push(el);
    }
    cout << "}" << endl;
  }
}


void DataFlowPass::transferFunction(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  meetFunction(generate, output);
}


void DataFlowPass::meetFunction(const Assignments& in, Assignments& out) {
  if (_meet == UNION) {
    DataFlowUtil::setUnion(out, in);
  } else if (_meet == INTERSECTION) {
    DataFlowUtil::setIntersect(out, in);
  }
}


Assignments DataFlowPass::getTop(const Function& fn) {
  if (_top == ALL) {
    return DataFlowUtil::all(fn);
  } else {
    return Assignments();
  }
}


bool DataFlowPass::runOnFunction(Function& fn) {
  cout << "  Function: " << fn.getName().data() << endl;

  // First pass: precompute generate and kill sets.
  BlockStates states;
  computeGenKill(fn, states);
  cout << endl;

  // iterate for a forwards pass
  if (_direction == FORWARDS) {
    traverseForwards(fn, states);

  // iterate for a backwards pass
  } else if (_direction == BACKWARDS) {
    traverseBackwards(fn, states);
  }

  // Does not modify the incoming Function.
  cout << endl;
  return false;
}


void DataFlowPass::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
}

}
