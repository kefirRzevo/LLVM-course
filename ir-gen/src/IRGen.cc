#include "GUILib.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Function* createSetBoundCondDefine(LLVMContext& context, Module& module, FunctionCallee& guiRandFunc) {
  IRBuilder<> builder(context);
  // define dso_local void @set_bound_cond(ptr nocapture noundef writeonly %0) local_unnamed_addr #0 {
  FunctionType *setBoundCondFuncType = FunctionType::get(builder.getVoidTy(), {builder.getPtrTy()}, false);
  Function *setBoundCondFunc = Function::Create(setBoundCondFuncType, Function::ExternalLinkage, "set_bound_cond", module);

  BasicBlock *BB1 = BasicBlock::Create(context, "", setBoundCondFunc);
  BasicBlock *BB2 = BasicBlock::Create(context, "", setBoundCondFunc);
  BasicBlock *BB3 = BasicBlock::Create(context, "", setBoundCondFunc);

  Value* arg0 = setBoundCondFunc->arg_begin();
  // 1:
  builder.SetInsertPoint(BB1);
  // br label %3
  builder.CreateBr(BB3);
  // 2:                                                ; preds = %3
  builder.SetInsertPoint(BB2);
  // ret void
  builder.CreateRetVoid();
  // 3:                                                ; preds = %1, %3
  builder.SetInsertPoint(BB3);
  // %4 = phi i64 [ 0, %1 ], [ %9, %3 ]
  PHINode *val4 = builder.CreatePHI(builder.getInt64Ty(), 2);
  // %5 = tail call i32 @gui_rand() #5
  CallInst* val5 = builder.CreateCall(guiRandFunc);
  // %6 = trunc i32 %5 to i8
  Value *val6 = builder.CreateTrunc(val5, builder.getInt8Ty());
  // %7 = and i8 %6, 1
  Value *val7 = builder.CreateAnd(val6, builder.getInt8(1));
  // %8 = getelementptr inbounds i8, ptr %0, i64 %4
  Value* val8 = builder.CreateGEP(builder.getInt8Ty(), arg0, val4, "", true);
  // store i8 %7, ptr %8, align 1
  builder.CreateStore(val7, val8);
  // %9 = add nuw nsw i64 %4, 1
  Value *val9 = builder.CreateAdd(val4, builder.getInt64(1), "", true, true);
  // %10 = icmp eq i64 %9, 512
  Value *val10 = builder.CreateICmpEQ(val9, builder.getInt64(512));
  // br i1 %10, label %2, label %3, !llvm.loop !5
  builder.CreateCondBr(val10, BB2, BB3);
  // Link PHI nodes
  // %4 = phi i64 [ 0, %1 ], [ %9, %3 ]
  val4->addIncoming(builder.getInt64(0), BB1);
  val4->addIncoming(val9, BB3);
  return setBoundCondFunc;
}

Function* createApplyRuleDefine(LLVMContext& context, Module& module) {
  IRBuilder<> builder(context);
  // define dso_local void @apply_rule(ptr nocapture noundef readonly %0, ptr nocapture noundef writeonly %1) local_unnamed_addr #3 {
  FunctionType *applyRuleFuncType = FunctionType::get(builder.getVoidTy(), {builder.getPtrTy(), builder.getPtrTy()}, false);
  Function *applyRuleFunc = Function::Create(applyRuleFuncType, Function::ExternalLinkage, "apply_rule", module);

  BasicBlock *BB2 = BasicBlock::Create(context, "", applyRuleFunc);
  BasicBlock *BB3 = BasicBlock::Create(context, "", applyRuleFunc);
  BasicBlock *BB4 = BasicBlock::Create(context, "", applyRuleFunc);

  Value* arg0 = applyRuleFunc->arg_begin();
  Value* arg1 = std::next(applyRuleFunc->arg_begin());
  // 2:
  builder.SetInsertPoint(BB2);
  // br label %4
  builder.CreateBr(BB4);
  // 3:                                                ; preds = %4
  builder.SetInsertPoint(BB3);
  // ret void
  builder.CreateRetVoid();
  // 4:                                                ; preds = %2, %4
  builder.SetInsertPoint(BB4);
  // %5 = phi i64 [ 0, %2 ], [ %17, %4 ]
  PHINode *val5 = builder.CreatePHI(builder.getInt64Ty(), 2);
  // %6 = add nuw i64 %5, 511
  Value *val6 = builder.CreateAdd(val5, builder.getInt64(511), "", true);
  // %7 = and i64 %6, 511
  Value *val7 = builder.CreateAnd(val6, builder.getInt64(511));
  // %8 = getelementptr inbounds i8, ptr %0, i64 %7
  Value* val8 = builder.CreateGEP(builder.getInt8Ty(), arg0, val7, "", true);
  //%9 = load i8, ptr %8, align 1, !tbaa !7, !range !11, !noundef !12
  Value* val9 = builder.CreateLoad(builder.getInt8Ty(), val8);
  //%10 = icmp eq i8 %9, 0
  Value *val10 = builder.CreateICmpEQ(val9, builder.getInt8(0));
  //%11 = select i1 %10, i32 0, i32 4
  Value* val11 = builder.CreateSelect(val10, builder.getInt32(0), builder.getInt32(4));
  // %12 = getelementptr inbounds i8, ptr %0, i64 %5
  Value* val12 = builder.CreateGEP(builder.getInt8Ty(), arg0, val5, "", true);
  // %13 = load i8, ptr %12, align 1, !tbaa !7, !range !11, !noundef !12
  Value* val13 = builder.CreateLoad(builder.getInt8Ty(), val12);
  // %14 = icmp eq i8 %13, 0
  Value *val14 = builder.CreateICmpEQ(val13, builder.getInt8(0));
  // %15 = or disjoint i32 %11, 2
  Value* val15 = builder.CreateOr(val11, builder.getInt32(2));
  // %16 = select i1 %14, i32 %11, i32 %15
  Value* val16 = builder.CreateSelect(val14, val11, val15);
  // %17 = add nuw nsw i64 %5, 1
  Value *val17 = builder.CreateAdd(val5, builder.getInt64(1), "", true, true);
  // %18 = and i64 %17, 511
  Value *val18 = builder.CreateAnd(val17, builder.getInt64(511));
  // %19 = getelementptr inbounds i8, ptr %0, i64 %18
  Value* val19 = builder.CreateGEP(builder.getInt8Ty(), arg0, val18, "", true);
  // %20 = load i8, ptr %19, align 1, !tbaa !7, !range !11, !noundef !12
  Value* val20 = builder.CreateLoad(builder.getInt8Ty(), val19);
  // %21 = zext nneg i8 %20 to i32
  Value* val21 = builder.CreateZExt(val20, builder.getInt32Ty(), "", true);
  // %22 = or disjoint i32 %16, %21
  Value* val22 = builder.CreateOr(val16, val21);
  // %23 = shl nuw nsw i32 1, %22
  Value* val23 = builder.CreateShl(builder.getInt32(1), val22, "", true, true);
  // %24 = and i32 %23, 110
  Value *val24 = builder.CreateAnd(val23, builder.getInt32(110));
  // %25 = icmp ne i32 %24, 0
  Value *val25 = builder.CreateICmpNE(val24, builder.getInt32(0));
  // %26 = zext i1 %25 to i8
  Value* val26 = builder.CreateZExt(val25, builder.getInt8Ty());
  // %27 = getelementptr inbounds i8, ptr %1, i64 %5
  Value* val27 = builder.CreateGEP(builder.getInt8Ty(), arg1, val5, "", true);
  // store i8 %26, ptr %27, align 1
  builder.CreateStore(val26, val27);
  // %28 = icmp eq i64 %17, 512
  Value *val28 = builder.CreateICmpEQ(val17, builder.getInt64(512));
  // br i1 %28, label %3, label %4, !llvm.loop !13
  builder.CreateCondBr(val28, BB3, BB4);
  // Link PHI nodes
  // %5 = phi i64 [ 0, %2 ], [ %17, %4 ]
  val5->addIncoming(builder.getInt64(0), BB2);
  val5->addIncoming(val17, BB4);
  return applyRuleFunc;
}

Function* createAppDefine(LLVMContext& context, Module& module,
                          FunctionCallee& guiRandFunc, FunctionCallee& guiSetPixelFunc,
                          FunctionCallee& guiFlushFunc, FunctionCallee& guiQuitEventFunc) {
  IRBuilder<> builder(context);

  Function *memsetFunc = Intrinsic::getDeclaration(&module, Intrinsic::memset, { builder.getPtrTy(), builder.getInt64Ty() });


  // define dso_local void @app() local_unnamed_addr #0 {
  FunctionType *appFuncType = FunctionType::get(builder.getVoidTy(), false);
  Function *appFunc = Function::Create(appFuncType, Function::ExternalLinkage, "app", module);

  BasicBlock *BB0 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB2 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB10 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB12 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB21 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB46 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB50 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB52 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB55 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB61 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB62 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB63 = BasicBlock::Create(context, "", appFunc);
  BasicBlock *BB66 = BasicBlock::Create(context, "", appFunc);

  // 0:
  builder.SetInsertPoint(BB0);
  // %1 = alloca [512 x [512 x i8]], align 16
  ArrayType *inner = ArrayType::get(builder.getInt8Ty(), 512);
  ArrayType *outer = ArrayType::get(inner, 512);
  AllocaInst* val1 = builder.CreateAlloca(outer);
  val1->setAlignment(Align(16));
  // call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(262144) %1, i8 0, i64 262144, i1 false)
  builder.CreateCall(memsetFunc, {val1, builder.getInt8(0), builder.getInt64(262144), builder.getInt1(false)});
  // br label %2
  builder.CreateBr(BB2);
  // 2:                                                ; preds = %2, %0
  builder.SetInsertPoint(BB2);
  // %3 = phi i64 [ 0, %0 ], [ %8, %2 ]
  PHINode *val3 = builder.CreatePHI(builder.getInt64Ty(), 2);
  // %4 = tail call i32 @gui_rand() #5
  CallInst* val4 = builder.CreateCall(guiRandFunc);
  // %5 = trunc i32 %4 to i8
  Value *val5 = builder.CreateTrunc(val4, builder.getInt8Ty());
  // %6 = and i8 %5, 1
  Value *val6 = builder.CreateAnd(val5, builder.getInt8(1));
  // %7 = getelementptr inbounds i8, ptr %1, i64 %3
  Value* val7 = builder.CreateGEP(builder.getInt8Ty(), val1, val3, "", true);
  // store i8 %6, ptr %7, align 1
  builder.CreateStore(val6, val7);
  // %8 = add nuw nsw i64 %3, 1
  Value *val8 = builder.CreateAdd(val3, builder.getInt64(1), "", true, true);
  // %9 = icmp eq i64 %8, 512
  Value *val9 = builder.CreateICmpEQ(val8, builder.getInt64(512));
  // br i1 %9, label %10, label %2, !llvm.loop !5
  builder.CreateCondBr(val9, BB10, BB2);

  // 10:                                               ; preds = %2
  builder.SetInsertPoint(BB10);
  // %11 = tail call zeroext i1 @gui_quit_event() #5
  CallInst* val11 = builder.CreateCall(guiQuitEventFunc);
  // br i1 %11, label %66, label %12
  builder.CreateCondBr(val11, BB66, BB12);

  // 12:                                               ; preds = %10, %50
  builder.SetInsertPoint(BB12);
  // %13 = phi i32 [ %17, %50 ], [ 0, %10 ]
  PHINode *val13 = builder.CreatePHI(builder.getInt32Ty(), 2);
  // %14 = and i32 %13, 511
  Value *val14 = builder.CreateAnd(val13, builder.getInt32(511));
  // %15 = zext nneg i32 %14 to i64
  Value* val15 = builder.CreateZExt(val14, builder.getInt64Ty(), "", true);
  // %16 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %15
  Value* val16 = builder.CreateGEP(outer, val1, {builder.getInt64(0), val15}, "", true);
  // %17 = add nuw nsw i32 %13, 1
  Value *val17 = builder.CreateAdd(val13, builder.getInt32(1), "", true, true);
  // %18 = and i32 %17, 511
  Value *val18 = builder.CreateAnd(val17, builder.getInt32(511));
  // %19 = zext nneg i32 %18 to i64
  Value* val19 = builder.CreateZExt(val18, builder.getInt64Ty(), "", true);
  // %20 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %19
  Value* val20 = builder.CreateGEP(outer, val1, {builder.getInt64(0), val19}, "", true);
  // br label %21
  builder.CreateBr(BB21);
  // 21:                                               ; preds = %21, %12
  builder.SetInsertPoint(BB21);
  // %22 = phi i64 [ 0, %12 ], [ %34, %21 ]
  PHINode *val22 = builder.CreatePHI(builder.getInt64Ty(), 2);
  // %23 = add nuw nsw i64 %22, 511
  Value *val23 = builder.CreateAdd(val22, builder.getInt64(511), "", true, true);
  // %24 = and i64 %23, 511
  Value *val24 = builder.CreateAnd(val23, builder.getInt64(511));
  // %25 = getelementptr inbounds i8, ptr %16, i64 %24
  Value* val25 = builder.CreateGEP(builder.getInt8Ty(), val16, val24, "", true);
  // %26 = load i8, ptr %25, align 1, !tbaa !7, !range !11, !noundef !12
  Value* val26 = builder.CreateLoad(builder.getInt8Ty(), val25);
  // %27 = icmp eq i8 %26, 0
  Value *val27 = builder.CreateICmpEQ(val26, builder.getInt8(0));
  // %28 = select i1 %27, i32 0, i32 4
  Value* val28 = builder.CreateSelect(val27, builder.getInt32(0), builder.getInt32(4));
  // %29 = getelementptr inbounds i8, ptr %16, i64 %22
  Value* val29 = builder.CreateGEP(builder.getInt8Ty(), val16, val22, "", true);
  // %30 = load i8, ptr %29, align 1, !tbaa !7, !range !11, !noundef !12
  Value* val30 = builder.CreateLoad(builder.getInt8Ty(), val29);
  // %31 = icmp eq i8 %30, 0
  Value *val31 = builder.CreateICmpEQ(val30, builder.getInt8(0));
  // %32 = or disjoint i32 %28, 2
  Value* val32 = builder.CreateOr(val28, builder.getInt32(2));
  // %33 = select i1 %31, i32 %28, i32 %32
  Value* val33 = builder.CreateSelect(val31, val28, val32);
  // %34 = add nuw nsw i64 %22, 1
  Value *val34 = builder.CreateAdd(val22, builder.getInt64(1), "", true, true);
  // %35 = and i64 %34, 511
  Value *val35 = builder.CreateAnd(val34, builder.getInt64(511));
  // %36 = getelementptr inbounds i8, ptr %16, i64 %35
  Value* val36 = builder.CreateGEP(builder.getInt8Ty(), val16, val35, "", true);
  // %37 = load i8, ptr %36, align 1, !tbaa !7, !range !11, !noundef !12
  Value* val37 = builder.CreateLoad(builder.getInt8Ty(), val36);
  // %38 = zext nneg i8 %37 to i32
  Value* val38 = builder.CreateZExt(val37, builder.getInt32Ty(), "", true);
  // %39 = or disjoint i32 %33, %38
  Value* val39 = builder.CreateOr(val33, val38);
  // %40 = shl nuw nsw i32 1, %39
  Value* val40 = builder.CreateShl(builder.getInt32(1), val39, "", true, true);
  // %41 = and i32 %40, 110
  Value *val41 = builder.CreateAnd(val40, builder.getInt32(110));
  // %42 = icmp ne i32 %41, 0
  Value *val42 = builder.CreateICmpNE(val41, builder.getInt32(0));
  // %43 = zext i1 %42 to i8
  Value* val43 = builder.CreateZExt(val42, builder.getInt8Ty());
  // %44 = getelementptr inbounds i8, ptr %20, i64 %22
  Value* val44 = builder.CreateGEP(builder.getInt8Ty(), val20, val22, "", true);
  // store i8 %43, ptr %44, align 1
  builder.CreateStore(val43, val44);
  // %45 = icmp eq i64 %34, 512
  Value *val45 = builder.CreateICmpEQ(val34, builder.getInt64(512));
  // br i1 %45, label %46, label %21, !llvm.loop !15
  builder.CreateCondBr(val45, BB46, BB21);

  // 46:                                               ; preds = %21, %52
  builder.SetInsertPoint(BB46);
  // %47 = phi i64 [ %53, %52 ], [ 0, %21 ]
  PHINode *val47 = builder.CreatePHI(builder.getInt64Ty(), 2);
  // %48 = trunc i64 %47 to i32
  Value *val48 = builder.CreateTrunc(val47, builder.getInt32Ty());
  // %49 = trunc i64 %47 to i32
  Value *val49 = builder.CreateTrunc(val47, builder.getInt32Ty());
  // br label %55
  builder.CreateBr(BB55);

  // 50:                                               ; preds = %52
  builder.SetInsertPoint(BB50);
  // tail call void @gui_flush() #5
  builder.CreateCall(guiFlushFunc);
  // %51 = tail call zeroext i1 @gui_quit_event() #5
  CallInst* val51 = builder.CreateCall(guiQuitEventFunc);
  // br i1 %51, label %66, label %12
  builder.CreateCondBr(val51, BB66, BB12);

  // 52:                                               ; preds = %63
  builder.SetInsertPoint(BB52);
  // %53 = add nuw nsw i64 %47, 1
  Value *val53 = builder.CreateAdd(val47, builder.getInt64(1), "", true, true);
  // %54 = icmp eq i64 %53, 512
  Value *val54 = builder.CreateICmpEQ(val53, builder.getInt64(512));
  // br i1 %54, label %50, label %46, !llvm.loop !16
  builder.CreateCondBr(val54, BB50, BB46);

  // 55:                                               ; preds = %46, %63
  builder.SetInsertPoint(BB55);
  // %56 = phi i64 [ 0, %46 ], [ %64, %63 ]
  PHINode *val56 = builder.CreatePHI(builder.getInt64Ty(), 2);
  // %57 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %47, i64 %56
  Value* val57 = builder.CreateGEP(outer, val1, {builder.getInt64(0), val47, val56}, "", true);
  // %58 = load i8, ptr %57, align 1, !tbaa !7, !range !11, !noundef !12
  Value* val58 = builder.CreateLoad(builder.getInt8Ty(), val57);
  // %59 = icmp eq i8 %58, 0
  Value *val59 = builder.CreateICmpEQ(val58, builder.getInt8(0));
  // %60 = trunc i64 %56 to i32
  Value *val60 = builder.CreateTrunc(val56, builder.getInt32Ty());
  // br i1 %59, label %62, label %61
  builder.CreateCondBr(val59, BB62, BB61);

  // 61:                                               ; preds = %55
  builder.SetInsertPoint(BB61);
  // tail call void @gui_set_pixel(i32 noundef %60, i32 noundef %48, i32 noundef 255) #5
  builder.CreateCall(guiSetPixelFunc, {val60, val48, builder.getInt32(255)});
  // br label %63
  builder.CreateBr(BB63);

  // 62:                                               ; preds = %55
  builder.SetInsertPoint(BB62);
  // tail call void @gui_set_pixel(i32 noundef %60, i32 noundef %49, i32 noundef -1) #5
  builder.CreateCall(guiSetPixelFunc, {val60, val49, builder.getInt32(-1)});
  // br label %63
  builder.CreateBr(BB63);

  // 63:                                               ; preds = %61, %62
  builder.SetInsertPoint(BB63);
  // %64 = add nuw nsw i64 %56, 1
  Value *val64 = builder.CreateAdd(val56, builder.getInt64(1), "", true, true);
  // %65 = icmp eq i64 %64, 512
  Value *val65 = builder.CreateICmpEQ(val64, builder.getInt64(512));
  // br i1 %65, label %52, label %55, !llvm.loop !17
  builder.CreateCondBr(val65, BB52, BB55);

  // 66:                                               ; preds = %50, %10
  builder.SetInsertPoint(BB66);
  // ret void
  builder.CreateRetVoid();
  // Link PHI nodes
  // %3 = phi i64 [ 0, %0 ], [ %8, %2 ]
  val3->addIncoming(builder.getInt64(0), BB0);
  val3->addIncoming(val8, BB2);
  // %13 = phi i32 [ %17, %50 ], [ 0, %10 ]
  val13->addIncoming(val17, BB50);
  val13->addIncoming(builder.getInt32(0), BB10);
  // %22 = phi i64 [ 0, %12 ], [ %34, %21 ]
  val22->addIncoming(builder.getInt64(0), BB12);
  val22->addIncoming(val34, BB21);
  // %47 = phi i64 [ %53, %52 ], [ 0, %21 ]
  val47->addIncoming(val53, BB52);
  val47->addIncoming(builder.getInt64(0), BB21);
  // %56 = phi i64 [ 0, %46 ], [ %64, %63 ]
  val56->addIncoming(builder.getInt64(0), BB46);
  val56->addIncoming(val64, BB63);
  return appFunc;
}

int main() {
  LLVMContext context;
  // ; ModuleID = 'Application.c'
  // source_filename = "Application.c"
  Module *mod = new Module("Application.c", context);
  IRBuilder<> builder(context);

  // declare i32 @gui_rand() local_unnamed_addr #2
  FunctionType *guiRandType = FunctionType::get(builder.getInt32Ty(), false);
  FunctionCallee guiRandFunc = mod->getOrInsertFunction("gui_rand", guiRandType);

  // declare void @gui_set_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #2
  ArrayRef<Type *> guiSetPixelParamTypes = {Type::getInt32Ty(context),
                                            Type::getInt32Ty(context),
                                            Type::getInt32Ty(context)};
  FunctionType *guiSetPixelType = FunctionType::get(builder.getVoidTy(), guiSetPixelParamTypes, false);
  FunctionCallee guiSetPixelFunc = mod->getOrInsertFunction("gui_set_pixel", guiSetPixelType);

  // declare void @gui_flush() local_unnamed_addr #2
  FunctionType *guiFlushType = FunctionType::get(builder.getVoidTy(), false);
  FunctionCallee guiFlushFunc = mod->getOrInsertFunction("gui_flush", guiFlushType);

  // declare zeroext i1 @gui_quit_event() local_unnamed_addr #2
  FunctionType *guiQuitEventType = FunctionType::get(builder.getInt1Ty(), false);
  FunctionCallee guiQuitEventFunc = mod->getOrInsertFunction("gui_quit_event", guiQuitEventType);

  Function* setBoundCondFunc = createSetBoundCondDefine(context, *mod, guiRandFunc);
  Function* applyRuleFunc = createApplyRuleDefine(context, *mod);
  Function* appFunc = createAppDefine(context, *mod, guiRandFunc, guiSetPixelFunc, guiFlushFunc, guiQuitEventFunc);

  // Dump LLVM IR
  mod->print(outs(), nullptr);

  // LLVM IR Interpreter
  outs() << "[EE] Run\n";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(mod)).create();

  ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
    if (fnName == "gui_rand") {
      return reinterpret_cast<void *>(gui_rand);
    }
    if (fnName == "gui_set_pixel") {
      return reinterpret_cast<void *>(gui_set_pixel);
    }
    if (fnName == "gui_flush") {
      return reinterpret_cast<void *>(gui_flush);
    }
    if (fnName == "gui_quit_event") {
      return reinterpret_cast<void *>(gui_quit_event);
    }
    return nullptr;
  });
  ee->finalizeObject();
  gui_init();
  ArrayRef<GenericValue> noargs;
  GenericValue v = ee->runFunction(appFunc, noargs);
  outs() << "[EE] Result: " << v.IntVal << "\n";
  gui_exit();
  return EXIT_SUCCESS;
}
