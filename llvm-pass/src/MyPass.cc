#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

struct MyModPass : public PassInfoMixin<MyModPass> {

  bool isFuncLogger(StringRef name) {
    return name == "callOptLogger" || name == "optLogger";
  }

  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    outs() << M.getName() << "\n";
    bool changed = false;
    for (auto &F : M) {
      changed = true;
      LLVMContext &Ctx = F.getContext();
      IRBuilder<> builder(Ctx);
      Type *retType = Type::getVoidTy(Ctx);

      // Prepare callOptLogger function
      ArrayRef<Type *> callParamTypes = {builder.getInt8Ty()->getPointerTo(),
                                         builder.getInt8Ty()->getPointerTo()};
      FunctionType *callLogFuncType =
          FunctionType::get(retType, callParamTypes, false);
      FunctionCallee callOptLogFunc = M.getOrInsertFunction("callOptLogger", callLogFuncType);

      // Prepare optLogger function
      ArrayRef<Type *> optParamTypes = {builder.getInt8Ty()->getPointerTo()};
      FunctionType *optLogFuncType =
          FunctionType::get(retType, optParamTypes, false);

      FunctionCallee optLogFunc =
          M.getOrInsertFunction("optLogger", optLogFuncType);

      // Insert loggers for call, binOpt and ret instructions
      for (auto &B : F) {
        for (auto &I : B) {
          if (isFuncLogger(F.getName()) || F.isDeclaration())
            continue;

          if (auto *phi = dyn_cast<PHINode>(&I))
            continue;

          if (auto *call = dyn_cast<CallInst>(&I)) {
            // Insert before call
            builder.SetInsertPoint(call);

            // Insert a call to callLogger function
            Function *callee = call->getCalledFunction();
            if (callee && !isFuncLogger(callee->getName())) {
              Value *calleeName =
                  builder.CreateGlobalStringPtr(callee->getName());
              Value *callerName = builder.CreateGlobalStringPtr(F.getName());
              Value *args[] = {callerName, calleeName};
              builder.CreateCall(callOptLogFunc, args);
            }
            continue;
          }

          builder.SetInsertPoint(&I);
          Value *opName = builder.CreateGlobalStringPtr(I.getOpcodeName());
          Value *args[] = {opName};
          builder.CreateCall(optLogFunc, args);
        }
      }
    }
    return changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
  };
};

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "MyPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "my-pass"){
            MPM.addPass(MyModPass{});
            return true;
          }
          return false;
        }
      );
    }
  };
}