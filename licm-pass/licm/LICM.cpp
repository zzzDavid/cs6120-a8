#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/LoopPass.h"

using namespace llvm;

namespace {
  struct LICMPass : public LoopPass {
    static char ID;
    LICMPass() : LoopPass(ID) {}

    virtual bool runOnLoop(Loop *L, LPPassManager &LPM) {
      // errs() << "I saw a function called " << F.getName() << "!\n";
      return false;
    }
  };
}

char LICMPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerLICMPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(createPromoteMemoryToRegisterPass());
  PM.add(new LICMPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerLICMPass);