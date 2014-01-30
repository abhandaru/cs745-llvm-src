#include "LocalOpts.h"

// useful headers
using namespace llvm;
using std::cout;
using std::endl;

namespace local {

void constFold(BasicBlock& block) {
  cout << "  block:" << endl;
  for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
    Instruction* instr = &(*it);
    cout << "    " << instr->getOpcodeName();
    if (BinaryOperator* binOp = dyn_cast<BinaryOperator>(instr)) {
      // ConstantInt* a = dyn_cast<ConstantInt>(binOp->getOperand(0));
      // ConstantInt* b = dyn_cast<ConstantInt>(binOp->getOperand(1));
      cout << " <binary> ";
    } else if (UnaryInstruction* unaryOp = dyn_cast<UnaryInstruction>(instr)) {
      cout << " <unrary> ";
    }
    cout << endl;
  }
}

Value* algebraicReduction() {

}

void algebraicIdentities(BasicBlock& block) {
  cout << "  block:" << endl;
  for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
    Instruction* instr = &(*it);
    cout << "    " << instr->getOpcodeName();
    if (BinaryOperator* binOp = dyn_cast<BinaryOperator>(instr)) {
      cout << " <binary>";
      BinaryOperator::BinaryOps opcode = binOp->getOpcode();
      Value* left = binOp->getOperand(0);
      Value* right = binOp->getOperand(1);

      // determine the operand types
      Instruction* leftInstr = dyn_cast<Instruction>(left);
      Instruction* rightInstr = dyn_cast<Instruction>(right);
      ConstantInt* leftValue = dyn_cast<ConstantInt>(left);
      ConstantInt* rightValue = dyn_cast<ConstantInt>(right);

      // both sources are instructions
      if (leftInstr && rightInstr) {
        cout <<  " <both instr>";
        if (leftInstr->isSameOperationAs(rightInstr)) {
          cout << " <same>";
          if (opcode == Instruction::Sub) {
            cout << " <subtract> -> 0";
          } else if (opcode == Instruction::SDiv) {
            cout << " <divide> -> 1";
          }
        }
      }

      // left source is a constant
      if (leftInstr && rightValue) {
        cout << " <instr-const>";
        if (rightValue->isOne() && opcode == Instruction::Mul) {
          cout << " <mul x 1> -> left";
        } else if (rightValue->isZero() && opcode == Instruction::Add) {
          cout << " <add + 0> -> left";
        }
      }
    }
    cout << endl;

  }
}

void eachFunction(Function& function) {
  cout << "function: " << function.getName().data() << endl;
  for (Function::iterator it = function.begin(); it != function.end(); ++it) {
    algebraicIdentities(*it);
  }
}

// We don't modify the program, so we preserve all analyses
// TODO: Change so we can actually modify.
void LocalOpts::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

bool LocalOpts::runOnModule(Module& module) {
  std::cout << "module: " << module.getModuleIdentifier().c_str() << std::endl;
  for (Module::iterator it = module.begin(); it != module.end(); ++it) {
    eachFunction(*it);
  }
  return false;
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
RegisterPass<LocalOpts> X("local-opts", "15745: Local Optimizations");

}
