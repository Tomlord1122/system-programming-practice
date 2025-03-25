#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;
using namespace std;

namespace
{
  struct VTableCheckerModulePass : public PassInfoMixin<VTableCheckerModulePass>
  {
  private:
    bool runOnModule(Module &M);

  public:
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM)
    {
      runOnModule(M);
      return PreservedAnalyses::all();
    }
  };
} // namespace

bool VTableCheckerModulePass::runOnModule(Module &M)
{
  outs() << __FUNCTION__ << '\n';

  // 獲取rtlib.so中的checkVTable函數
  LLVMContext &Ctx = M.getContext();
  FunctionCallee CheckVTableFunc = M.getOrInsertFunction(
      "vtable_security_check", FunctionType::get(Type::getVoidTy(Ctx), {Type::getInt8PtrTy(Ctx)}, false));

  FunctionCallee NewFunc = M.getOrInsertFunction(
      "new_function", FunctionType::get(Type::getVoidTy(Ctx), {}, false));

  for (Function &F : M)
  {
    if (F.getName() == "main")
    {
      for (BasicBlock &BB : F)
      {
        for (Instruction &I : BB)
        {
          outs() << I << "\n";
          if (auto *gepInst = dyn_cast<GetElementPtrInst>(&I))
          {
            Value *BasePtr = gepInst->getPointerOperand(); // obtain base pointer
            IRBuilder<> Builder(&I);
            Builder.CreateCall(CheckVTableFunc, {BasePtr});
            outs() << "Base pointer addr: " << BasePtr << "\n";
            outs() << "Inserted CheckVTableFunc call after loading vtable.\n";
          }
        }
      }
    }
  }
  return true;
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
  return {
      .APIVersion = LLVM_PLUGIN_API_VERSION,
      .PluginName = "VTable checker pass",
      .PluginVersion = "v0.1",
      .RegisterPassBuilderCallbacks =
          [](PassBuilder &PB)
      {
        PB.registerPipelineStartEPCallback(
            [](ModulePassManager &MPM, OptimizationLevel Level)
            {
              MPM.addPass(VTableCheckerModulePass());
            });
      }};
}