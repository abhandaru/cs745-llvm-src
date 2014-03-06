// 15-745 S14 Assignment 3: loop-invariant-code-motion.cpp
// Group: akbhanda, zheq
///////////////////////////////////////////////////////////////////////////////

#include "loop-invariant-code-motion.h"

using std::cout;
using std::endl;
using std::vector;


namespace llvm {

//
// Call the constructor for the super class.
//
LicmPass::LicmPass() : LoopPass(ID) {
  // do nothing for now
}


//
// Override the runOnLoop function provided by LoopPass.
// Return true because we intend on modifying the control flow graph.
//
bool LicmPass::runOnLoop(Loop *loop, LPPassManager &LPM) {
  unsigned int depth = loop->getLoopDepth();
  cout << "Loop<depth = " << depth << ">" << endl;

  // check if there is a preheader
  BasicBlock* preheader = loop->getLoopPreheader();
  if (preheader) {
    cout << "- found preheader" << endl;
  } else {
    cout << "- no preheader ... skipping" << endl;
    return false;
  }

  // dump preheader
  // preheader->dump();

  // run analysis passes on blocks
  const BlockVector& blocks = loop->getBlocks();
  BlockStates states = dominance.runOnBlocks(blocks);
  for (BlockVector::const_iterator I = blocks.begin(), IE = blocks.end();
    I != IE; ++I) {
    const BasicBlock* block = *I;
    BlockState& state = states[block];
    // print out dominator list
    DataFlowUtil::print(state.out);
    cout << endl;
  }

  // assume we modified the loop
  return true;
};


//
// Set so that we can modify the control flow graph.
// TODO: Is this correct?
//
void LicmPass::getAnalysisUsage(AnalysisUsage& AU) const {
  AU.setPreservesCFG();
}


//
// Do the following to meet the LoopPass API
//
char LicmPass::ID = 0;
RegisterPass<LicmPass> Y("cd-licm", "15745 Liveness");

}
