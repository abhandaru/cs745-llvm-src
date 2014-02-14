// 15-745 S14 Assignment 2: liveness.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "liveness.h"


using std::cout;
using std::endl;


namespace llvm {


Liveness::Liveness() : DataFlowPass(ID, NONE, UNION, BACKWARDS) {
  cout << ">> Liveness() constructor" << endl;
};


Assignments Liveness::generate(const BasicBlock& block) {
  return DataFlowUtil::uses(block);
}


Assignments Liveness::kill(const BasicBlock& block) {
  return DataFlowUtil::defines(block);
}


void Liveness::transferFn(const Assignments& generate,
    const Assignments& kill, const Assignments& input, Assignments& output) {
  output = input;
  DataFlowUtil::setSubtract(output, kill);
  meetFunction(generate, output);
}


char Liveness::ID = 0;
RegisterPass<Liveness> X("live", "15745 Liveness");

}
