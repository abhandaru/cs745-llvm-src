#include "LocalOpts.h"

// useful headers
using namespace llvm;
using std::cout;
using std::endl;

namespace local {

void constantFold(BasicBlock& block) {
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

void strengthReduction(BasicBlock& block) {
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

      if (opcode == Instruction::Mul) {
        cout << " <mul>";
        if (leftInstr && rightValue) {
          int64_t value = rightValue->getValue().getSExtValue();
          cout << " <left * " << value << ">";
        } else if (leftValue && rightInstr) {

        }
      } else if (opcode == Instruction::SDiv) {
        cout << " <div>";
        if (leftInstr && rightValue) {

        } else if (leftValue && rightInstr) {

        }
      }

    }
    cout << endl;
  }
}

void algebraicIdentities(BasicBlock& block) {
  cout << "  block:" << endl;
  LLVMContext& context = block.getContext();

  // iterate through instructions
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
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), 0);
            ReplaceInstWithValue(block.getInstList(), it, value);
          } else if (opcode == Instruction::SDiv) {
            cout << " <divide> -> 1";
            // TODO: does not catch divide by zero
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), 1);
            ReplaceInstWithValue(block.getInstList(), it, value);
          }
        }
        // clean up references if possible
        if (leftInstr->use_empty())
          leftInstr->eraseFromParent();
        if (rightInstr->use_empty())
          rightInstr->eraseFromParent();
      }

      // left source is a constant
      else if (leftInstr && rightValue) {
        cout << " <instr-const>";
        if ((opcode == Instruction::Mul || opcode == Instruction::SDiv) &&
             rightValue->isOne()) {
          cout << " <instr x/ 1> -> left";
          ReplaceInstWithValue(block.getInstList(), it, leftInstr);
        } else if ((opcode == Instruction::Add || opcode == Instruction::Sub) &&
                    rightValue->isZero()) {
          cout << " <instr +- 0> -> left";
          ReplaceInstWithValue(block.getInstList(), it, leftInstr);
        }
      }

      // left source is a constant
      else if (leftValue && rightInstr) {
        cout << " <instr-const>";
        if (opcode == Instruction::Mul && leftValue->isOne()) {
          cout << " <1 x instr> -> right";
          ReplaceInstWithValue(block.getInstList(), it, rightInstr);
        } else if (opcode == Instruction::Add && leftValue->isZero()) {
          cout << " <0 + instr> -> right";
          ReplaceInstWithValue(block.getInstList(), it, rightInstr);
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
    // strengthReduction(*it);
  }
}

// We don't modify the program, so we preserve all analyses
// TODO: Change so we can actually modify.
void LocalOpts::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
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
