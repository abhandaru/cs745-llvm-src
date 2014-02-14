// 15-745 S14 Assignment 2: reaching-definitions.cpp
// Group: akbhanda, zheq
////////////////////////////////////////////////////////////////////////////////

#include "reaching-definitions.h"


using std::cout;
using std::endl;


namespace llvm {


// TODO: Call DataFlowPass with the right arguments
ReachingDefinitions::ReachingDefinitions() :
    DataFlowPass(ID, ALL, INTERSECTION, FORWARDS) {
  cout << ">> Reaching() constructor" << endl;
};

Assignments ReachingDefinitions::generate(const BasicBlock& block) {
  return DataFlowUtil::defines(block);
}


Assignments ReachingDefinitions::kill(const BasicBlock& block) {
  // TODO: Is this correct?
  return DataFlowUtil::defines(block);
}


char ReachingDefinitions::ID = 0;
RegisterPass<ReachingDefinitions> X("reach", "15745 ReachingDefinitions");


}
