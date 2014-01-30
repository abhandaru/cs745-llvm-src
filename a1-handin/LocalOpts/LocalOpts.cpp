#include "LocalOpts.h"

// useful headers
using namespace llvm;
using std::cout;
using std::endl;

namespace local {

void LocalOpts::constantFolding(BasicBlock& block) {
  LLVMContext& context = block.getContext();
  ValueMap<Value*, Value* > lastStore;

  // first pass to propagate stores to loads
  for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
    Instruction* instr = &(*it);
    if (StoreInst* store = dyn_cast<StoreInst>(instr)) {
      Value* pointer = store->getPointerOperand();
      Value* value = store->getValueOperand();
      std::pair<Value*, Value* > pair(pointer, value);
      lastStore.erase(pointer);
      lastStore.insert(pair);
    } else if (LoadInst* load = dyn_cast<LoadInst>(instr)) {
      Value* pointer = load->getPointerOperand();
      Value* value = lastStore.lookup(pointer);
      if (value) {
        ReplaceInstWithValue(block.getInstList(), it, value);
        fold++;
      }
    }
  }

  // second pass to fold some exposed constants
  bool changed = true;
  while (changed) {
    changed = false;
    for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
      Instruction* instr = &(*it);
      if (BinaryOperator* binOp = dyn_cast<BinaryOperator>(instr)) {
        BinaryOperator::BinaryOps opcode = binOp->getOpcode();
        ConstantInt* left = dyn_cast<ConstantInt>(binOp->getOperand(0));
        ConstantInt* right = dyn_cast<ConstantInt>(binOp->getOperand(1));
        // compress if both constant with correct operation
        if (left && right) {
          uint64_t leftVal = left->getValue().getZExtValue();
          uint64_t rightVal = right->getValue().getZExtValue();
          if (opcode == Instruction::Add) {
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), leftVal + rightVal);
            ReplaceInstWithValue(block.getInstList(), it, value);
            changed = true;
            fold++;
          } else if (opcode == Instruction::Sub) {
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), leftVal - rightVal);
            ReplaceInstWithValue(block.getInstList(), it, value);
            changed = true;
            fold++;
          } else if (opcode == Instruction::Mul) {
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), leftVal * rightVal);
            ReplaceInstWithValue(block.getInstList(), it, value);
            changed = true;
            fold++;
          } else if (opcode == Instruction::SDiv) {
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), leftVal / rightVal);
            ReplaceInstWithValue(block.getInstList(), it, value);
            changed = true;
            fold++;
          }
        }
      }
    }
  }
}

void LocalOpts::strengthReduction(BasicBlock& block) {
  LLVMContext& context = block.getContext();

  for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
    Instruction* instr = &(*it);
    if (BinaryOperator* binOp = dyn_cast<BinaryOperator>(instr)) {
      BinaryOperator::BinaryOps opcode = binOp->getOpcode();
      Value* left = binOp->getOperand(0);
      Value* right = binOp->getOperand(1);

      // determine the operand types
      Instruction* leftInstr = dyn_cast<Instruction>(left);
      Instruction* rightInstr = dyn_cast<Instruction>(right);
      ConstantInt* leftValue = dyn_cast<ConstantInt>(left);
      ConstantInt* rightValue = dyn_cast<ConstantInt>(right);

      // multiply instruction
      if (opcode == Instruction::Mul) {
        if (leftInstr && rightValue) {
          uint64_t value = rightValue->getValue().getZExtValue();
          uint64_t log2Value = log2(value);
          if (value == (1 << log2Value)) {
            ConstantInt* amount = ConstantInt::get(Type::getInt32Ty(context), log2Value);
            Instruction* shift = BinaryOperator::Create(Instruction::Shl, leftInstr, amount);
            ReplaceInstWithInst(block.getInstList(), it, shift);
            strength++;
          }
        } else if (leftValue && rightInstr) {
          uint64_t value = leftValue->getValue().getZExtValue();
          uint64_t log2Value = log2(value);
          if (value == (1 << log2Value)) {
            ConstantInt* amount = ConstantInt::get(Type::getInt32Ty(context), log2Value);
            Instruction* shift = BinaryOperator::Create(Instruction::Shl, rightInstr, amount);
            ReplaceInstWithInst(block.getInstList(), it, shift);
            strength++;
          }
        }

      // divide instruction
      } else if (opcode == Instruction::SDiv) {
        if (leftInstr && rightValue) {
          uint64_t value = rightValue->getValue().getZExtValue();
          uint64_t log2Value = log2(value);
          if (value == (1 << log2Value)) {
            ConstantInt* amount = ConstantInt::get(Type::getInt32Ty(context), log2Value);
            Instruction* shift = BinaryOperator::Create(Instruction::LShr, leftInstr, amount);
            ReplaceInstWithInst(block.getInstList(), it, shift);
            strength++;
          }
        }
      }
    }
  }
}

void LocalOpts::algebraicIdentities(BasicBlock& block) {
  LLVMContext& context = block.getContext();

  // iterate through instructions
  for (BasicBlock::iterator it = block.begin(); it != block.end(); ++it) {
    Instruction* instr = &(*it);
    if (BinaryOperator* binOp = dyn_cast<BinaryOperator>(instr)) {
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
        if (leftInstr->isSameOperationAs(rightInstr)) {
          if (opcode == Instruction::Sub) {
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), 0);
            ReplaceInstWithValue(block.getInstList(), it, value);
            algebra++;
          } else if (opcode == Instruction::SDiv) {
            // TODO: does not catch divide by zero
            ConstantInt* value = ConstantInt::get(Type::getInt32Ty(context), 1);
            ReplaceInstWithValue(block.getInstList(), it, value);
            algebra++;
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
        if ((opcode == Instruction::Mul || opcode == Instruction::SDiv) &&
             rightValue->isOne()) {
          ReplaceInstWithValue(block.getInstList(), it, leftInstr);
          algebra++;
        } else if ((opcode == Instruction::Add || opcode == Instruction::Sub) &&
                    rightValue->isZero()) {
          ReplaceInstWithValue(block.getInstList(), it, leftInstr);
          algebra++;
        }
      }

      // right source is a constant
      else if (leftValue && rightInstr) {
        if (opcode == Instruction::Mul && leftValue->isOne()) {
          ReplaceInstWithValue(block.getInstList(), it, rightInstr);
          algebra++;
        } else if (opcode == Instruction::Add && leftValue->isZero()) {
          ReplaceInstWithValue(block.getInstList(), it, rightInstr);
          algebra++;
        }
      }
    }
  }
}

bool LocalOpts::runOnModule(Module& module) {
  // init counters
  strength = 0;
  fold = 0;
  algebra = 0;

  // run over functions
  for (Module::iterator it = module.begin(); it != module.end(); ++it) {
    eachFunction(*it);
  }

  // print out tranform counts
  cout << "Transformations applied:" << endl;
  cout << "  Algebraic Identities: " << algebra << endl;
  cout << "  Constant Folding: " << fold << endl;
  cout << "  Strength Reductions: " << strength << endl;
  return false;
}

void LocalOpts::eachFunction(Function& function) {
  for (Function::iterator it = function.begin(); it != function.end(); ++it) {
    algebraicIdentities(*it);
    strengthReduction(*it);
    constantFolding(*it);
  }
}

// Changed so we can actually modify the code tree.
void LocalOpts::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
}

uint64_t LocalOpts::log2(uint64_t x) {
  int i = 0;
  while (x >>= 1) {
    i++;
  }
  return i;
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
RegisterPass<LocalOpts> X("local-opts", "15745: Local Optimizations");

}
