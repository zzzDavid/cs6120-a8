#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils.h"
#include <set>

using namespace llvm;

namespace {
struct LICMPass : public LoopPass {
  static char ID;
  LICMPass() : LoopPass(ID) {}

  virtual bool runOnLoop(Loop *L, LPPassManager &LPM) {
    // Get loop preheader
    BasicBlock *preheader = L->getLoopPreheader();

    bool updated = true;
    std::set<Value *> to_move;

    while (updated) {
      updated = false;
      for (auto *B : L->getBlocksVector()) {
        for (auto &I : *B) {
          if (I.isTerminator()) continue;
          if (I.mayHaveSideEffects()) continue;
          if (I.mayReadFromMemory()) continue;
          if (to_move.count(&I) != 0) continue;
          if (auto* phi = dyn_cast<PHINode>(&I)) continue;
          bool li = true; // is loop invariant?
          for (auto &operand : I.operands()) {
            if (!L->isLoopInvariant(operand) ||
                to_move.count(operand) != 0) {
              li = false;
            }
          }
          if (li) {
            to_move.insert(&I);
            updated = true;
          }
        }
      }
    }

    for (Value *V : to_move) {
      auto* I = cast<Instruction>(V);
      // llvm::outs() << *I << "\n";
      I->moveBefore(&preheader->back());
    }

    return false;
  }
};
} // namespace

char LICMPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerLICMPass(const PassManagerBuilder &,
                             legacy::PassManagerBase &PM) {
  // Enable mem2reg LLVM pass so that we have regular
  // SSA form instead of load/store with pointers
  PM.add(createPromoteMemoryToRegisterPass());
  // My pass
  PM.add(new LICMPass());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible, registerLICMPass);