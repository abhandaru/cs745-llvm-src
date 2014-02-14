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


bool DataFlowPass::runOnFunction(Function& fn) {
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
      cout << "    Block: " << current->getName().data() << endl;
      worklist.pop();

      // determine the meet of all successors
      // TODO: Set to correct TOP.
      bool hasSuccessor = false;
      Assignments meet;
      for (succ_const_iterator I = succ_begin(current), IE = succ_end(current);
          I != IE; ++I) {
        hasSuccessor = true;
        BlockState& succ_state = states[*I];
        meetFunction(succ_state.in, meet);
      }

      // see if we need to inspect this node
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

      if (hasSuccessor && DataFlowUtil::setEquals(state.out, meet)) {
        cout << "      <input unchanged>" << endl;
        continue;
      }

      // perform transfer function
      state.out = meet;
      transferFunction(state.generates, state.kills, state.out, state.in);

      //
      // Debug prints
      //
      cout << "      in: { ";
      for (Assignments::const_iterator i = state.in.begin(); i != state.in.end(); ++i) {
        cout << (*i).pointer->getName().data() << " ";
      }
      cout << "}" << endl;

      //
      //
      //
      for (const_pred_iterator I = pred_begin(current), IE = pred_end(current);
          I != IE; ++I) {
        worklist.push(*I);
      }

      // more debug shit
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

  // Does not modify the incoming Function.
  cout << endl;
  return false;
}


void DataFlowPass::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
}


}
