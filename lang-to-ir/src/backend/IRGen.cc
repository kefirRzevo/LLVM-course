#include <vector>

#include "frontend/Driver.hh"
#include "frontend/Nodes.hh"
#include "frontend/SymTables.hh"
#include "frontend/Types.hh"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

namespace paracl {

class IRGenerator final
    : public visitor::VisitorBase<INode, IRGenerator, llvm::Value *> {
  struct ExternFunc {
    void *ptr_;
    llvm::FunctionCallee callee_;
  };

  struct Loop {
    WhileStmt *loop_;
    llvm::BasicBlock *cond_;
    llvm::BasicBlock *exit_;
  };

  using ScopeExterns = Scope<ExternFunc>;
  using ScopeLLVMVal = Scope<llvm::Value *>;
  using ScopeLLVMValStack = ScopeStack<llvm::Value *>;

  llvm::LLVMContext &context_;
  std::unique_ptr<llvm::Module> module_;
  std::unique_ptr<llvm::IRBuilder<>> builder_;

  ScopeExterns externs_;
  ScopeLLVMValStack scopes_;
  TranslationUnit *root_ = nullptr;
  llvm::Function *curFunc_ = nullptr;
  llvm::Function *mainFunc_ = nullptr;
  FunctionDecl *main_ = nullptr;
  std::vector<Loop> loops_;

  llvm::IntegerType *getLLVMType(IntType *) { return builder_->getInt32Ty(); }

  llvm::Type *getLLVMType(VoidType *) { return builder_->getVoidTy(); }

  llvm::ArrayType *getLLVMType(ArrayType *type) {
    auto elemType = type->getElemType();
    auto elemCount = type->getElemCount();
    auto llvmElemType = getLLVMType(elemType);
    return llvm::ArrayType::get(llvmElemType, elemCount);
  }

  llvm::PointerType *getLLVMType(PointerType *type) {
    auto elemType = type->getElemType();
    auto llvmElemType = getLLVMType(elemType);
    return llvm::PointerType::get(llvmElemType, 0);
  }

  llvm::FunctionType *getLLVMType(FunctionType *type) {
    auto llvmParamTypes = std::vector<llvm::Type *>{};
    std::transform(type->begin(), type->end(),
                   std::back_inserter(llvmParamTypes),
                   [&](auto &&paramType) { return getLLVMType(paramType); });
    auto retType = type->getRetType();
    auto llvmRetType = getLLVMType(retType);
    return llvm::FunctionType::get(llvmRetType, std::move(llvmParamTypes),
                                   false);
  }

  llvm::Type *getLLVMType(Type *type) {
    switch (type->getKind()) {
    case TypeKind::IntType:
      return getLLVMType(static_cast<IntType *>(type));
    case TypeKind::VoidType:
      return getLLVMType(static_cast<VoidType *>(type));
    case TypeKind::ArrayType:
      return getLLVMType(static_cast<ArrayType *>(type));
    case TypeKind::PointerType:
      return getLLVMType(static_cast<PointerType *>(type));
    case TypeKind::FunctionType:
      return getLLVMType(static_cast<FunctionType *>(type));
    default:
      throw std::runtime_error("Unknown type");
    }
  }

  llvm::Value *callMemcpy(llvm::Value *dest, llvm::Value *src,
                          ArrayType *type) {
    auto elemType = type->getElemType();
    auto llvmElemType = getLLVMType(elemType);
    auto llvmSize = builder_->getInt64(type->getSize());
    auto llvmSizeType = builder_->getInt64Ty();
    auto llvmPtr = llvm::PointerType::get(llvmElemType, 0);
    auto llvmFalse = builder_->getFalse();
    auto argsType = std::vector<llvm::Type *>{llvmPtr, llvmPtr, llvmSizeType};
    llvm::Function *memcpy = llvm::Intrinsic::getDeclaration(
        module_.get(), llvm::Intrinsic::memcpy, argsType);
    auto llvmPtrType = llvm::PointerType::get(builder_->getInt8Ty(), 0);
    dest = builder_->CreateBitCast(dest, llvmPtrType);
    src = builder_->CreateBitCast(src, llvmPtrType);
    return builder_->CreateCall(memcpy, {dest, src, llvmSize, llvmFalse});
  }

  void initVariable(llvm::Value *lhs, llvm::Value *rhs, Type *type) {
    switch (type->getKind()) {
    case TypeKind::ArrayType: {
      auto asArray = static_cast<ArrayType *>(type);
      callMemcpy(lhs, rhs, asArray);
      break;
    }
    default:
      builder_->CreateStore(rhs, lhs);
      break;
    }
  }

  llvm::BasicBlock *createBlock(std::string_view name) {
    assert(curFunc_);
    return llvm::BasicBlock::Create(context_, name, curFunc_);
  }

public:
  VISIT(AllNodes);

  IRGenerator(std::string_view filepath, llvm::LLVMContext &ctx)
      : context_(ctx), module_(std::make_unique<llvm::Module>(filepath, ctx)),
        builder_(std::make_unique<llvm::IRBuilder<>>(ctx)) {}

  std::unique_ptr<llvm::Module> releaseModule() { return std::move(module_); }

  void *getExternFuncPtr(std::string_view name) const {
    auto externOpt = externs_.lookup(name);
    if (externOpt.has_value()) {
      auto value = externOpt.value();
      return value.ptr_;
    }
    return nullptr;
  }

  llvm::Function *getMainFunc() const { return mainFunc_; }

  llvm::Value *visit(UnaryOperator &node) {
    auto expr = node.getSubExpr();
    auto llvmVal = apply(*expr);
    switch (node.getOpcode()) {
    case UnaryOpcode::UN_SUB:
      return builder_->CreateNeg(llvmVal);
    case UnaryOpcode::UN_ADD:
      return llvmVal;
    case UnaryOpcode::UN_NOT:
      return builder_->CreateNot(llvmVal);
    default:
      throw std::runtime_error("Unknown unary opcode");
    }
  }

  llvm::Value *visit(BinaryOperator &node) {
    auto lhs = node.getLHS();
    auto rhs = node.getRHS();
    auto opc = node.getOpcode();
    if (isArithmetic(opc)) {
      auto llvmLhs = apply(*lhs);
      auto llvmRhs = apply(*rhs);
      switch (opc) {
      case BinaryOpcode::BIN_MUL:
        return builder_->CreateMul(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_DIV:
        return builder_->CreateSDiv(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_MOD:
        return builder_->CreateSRem(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_ADD:
        return builder_->CreateAdd(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_SUB:
        return builder_->CreateSub(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_SHL:
        return builder_->CreateShl(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_SHR:
        return builder_->CreateAShr(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_L:
        return builder_->CreateICmpSLT(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_G:
        return builder_->CreateICmpSGT(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_LE:
        return builder_->CreateICmpSLE(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_GE:
        return builder_->CreateICmpSGE(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_EQ:
        return builder_->CreateICmpEQ(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_NE:
        return builder_->CreateICmpNE(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_AND:
      case BinaryOpcode::BIN_BIT_AND:
        return builder_->CreateAnd(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_BIT_OR:
      case BinaryOpcode::BIN_OR:
        return builder_->CreateOr(llvmLhs, llvmRhs);
      case BinaryOpcode::BIN_BIT_XOR:
        return builder_->CreateXor(llvmLhs, llvmRhs);
      default:
        assert(0 && "unreachable");
      }
    } else if (opc == BinaryOpcode::BIN_ASSIGN) {
      auto llvmRhs = apply(*rhs);
      auto llvmLhs = apply(*lhs);
      return builder_->CreateStore(llvmRhs, llvmLhs);
    } else if (opc == BinaryOpcode::BIN_COMMA) {
      apply(*lhs);
      auto llvmRhs = apply(*rhs);
      return llvmRhs;
    }
    throw std::runtime_error("Unknown binary opcode");
  }

  llvm::Value *visit(ConditionalOperator &node) {
    auto cond = node.getCondExpr();
    auto llvmCond = apply(*cond);
    auto lhs = node.getTrueExpr();
    auto llvmLhs = apply(*lhs);
    auto rhs = node.getFalseExpr();
    auto llvmRhs = apply(*rhs);
    return builder_->CreateSelect(llvmCond, llvmLhs, llvmRhs);
  }

  llvm::Value *visit(IntegerLiteral &node) {
    auto val = node.getVal();
    return builder_->getInt32(val);
  }

  llvm::Value *visit(UndefLiteral &node) {
    auto val = std::rand() % 100;
    return builder_->getInt32(val);
  }

  llvm::Value *visit(DeclRefExpr &node) {
    auto name = node.getName();
    auto val = scopes_.lookup(name);
    assert(val.has_value());
    return val.value();
  }

  llvm::Value *visit(ImplicitCastExpr &node) {
    auto expr = node.getExpr();
    auto llvmVal = apply(*expr);
    auto castType = node.getImplicitCastType();
    switch (castType) {
    case ImplicitCastType::LValueToRValue: {
      auto type = node.getType();
      if (type->getKind() != TypeKind::PointerType) {
        auto llvmType = getLLVMType(type);
        return builder_->CreateLoad(llvmType, llvmVal);
      }
      return llvmVal;
    }
    case ImplicitCastType::ArrayToPointerDecay: {
      auto exprType = expr->getType();
      assert(exprType->getKind() == TypeKind::ArrayType);
      auto exprTypeAsArr = static_cast<ArrayType *>(exprType);
      auto llvmExprType = getLLVMType(exprTypeAsArr);
      return builder_->CreateGEP(llvmExprType, llvmVal, builder_->getInt32(0));
    }
    default:
      return llvmVal;
    }
  }

  llvm::Value *visit(ArraySubscriptExpr &node) {
    auto lhs = node.getLHS();
    auto rhs = node.getRHS();
    auto llvmLhs = apply(*lhs);
    auto llvmRhs = apply(*rhs);
    auto type = node.getType();
    auto llvmType = getLLVMType(type);
    return builder_->CreateGEP(llvmType, llvmLhs, llvmRhs);
  }

  llvm::Value *visit(InitListExpr &node) {
    auto llvmVals = std::vector<llvm::Constant *>{};
    for (auto &&expr : node) {
      auto llvmVal = apply(*expr);
      assert(llvm::Constant::classof(llvmVal));
      auto asConstant = static_cast<llvm::Constant *>(llvmVal);
      llvmVals.push_back(asConstant);
    }
    auto type = node.getType();
    assert(type->getKind() == TypeKind::ArrayType);
    auto asPtr = static_cast<ArrayType *>(type);
    auto llvmType = getLLVMType(static_cast<ArrayType *>(type));
    auto constant = llvm::ConstantArray::get(llvmType, llvmVals);
    auto ret = new llvm::GlobalVariable(
        *module_, llvmType, true, llvm::GlobalValue::PrivateLinkage, constant);
    return ret;
  }

  llvm::Value *visit(CallExpr &node) {
    auto llvmArgs = std::vector<llvm::Value *>{};
    for (auto &&param : node) {
      auto llvmVal = apply(*param);
      llvmArgs.push_back(llvmVal);
    }
    auto call = node.getCall();
    auto type = call->getType();
    if (type->getKind() == TypeKind::PointerType) {
      auto asPointer = static_cast<PointerType *>(type);
      type = asPointer->getElemType();
    }
    assert(type->getKind() == TypeKind::FunctionType);
    auto llvmType = getLLVMType(static_cast<FunctionType *>(type));
    auto llvmCall = apply(*call);
    return builder_->CreateCall(llvmType, llvmCall, llvmArgs);
  }

  llvm::Value *visit(InputExpr &node) {
    auto scan = externs_.lookup("scan");
    assert(scan.has_value());
    auto value = scan.value();
    auto llvmRetType = getLLVMType(node.getType());
    return builder_->CreateCall(value.callee_);
  }

  llvm::Value *visit(VarDecl &node) {
    auto name = node.getName();
    auto type = node.getType();
    auto llvmType = getLLVMType(type);
    auto llvmVal = builder_->CreateAlloca(llvmType);
    auto expr = node.getExpr();
    auto llvmInit = apply(*expr);
    initVariable(llvmVal, llvmInit, type);
    scopes_.declare(name, llvmVal);
    return nullptr;
  }

  llvm::Value *visit(ParmVarDecl &node) { return nullptr; }

  llvm::Value *visit(FunctionDecl &node) {
    auto name = node.getName();
    auto type = node.getType();
    auto body = node.getBody();
    assert(type->getKind() == TypeKind::FunctionType);
    auto asFunction = static_cast<FunctionType *>(type);
    auto llvmType = getLLVMType(asFunction);
    auto llvmFunc = llvm::Function::Create(
        llvmType, llvm::Function::ExternalLinkage, name, module_.get());
    curFunc_ = llvmFunc;
    if (std::addressof(node) == main_)
      mainFunc_ = llvmFunc;
    scopes_.declare(name, llvmFunc);

    ScopeLLVMVal scope;
    scopes_.beginScope(scope);
    for (auto &&[llvmVal, param] : llvm::zip(llvmFunc->args(), node))
      scopes_.declare(param->getName(), &llvmVal);
    auto entryBlock = createBlock("entry");
    builder_->SetInsertPoint(entryBlock);
    apply(*body);
    scopes_.endScope();
    curFunc_ = nullptr;
    return nullptr;
  }

  llvm::Value *visit(ExternFunctionDecl &node) {
    auto name = node.getName();
    auto type = node.getType();
    assert(type->getKind() == TypeKind::FunctionType);
    auto asFunction = static_cast<FunctionType *>(type);
    auto llvmType = getLLVMType(asFunction);
    auto callee = module_->getOrInsertFunction(name, llvmType);
    externs_.declare(name, {node.getPtr(), std::move(callee)});
    scopes_.declare(name, callee.getCallee());
    return nullptr;
  }

  llvm::Value *visit(OutputStmt &node) {
    auto print = externs_.lookup("print");
    assert(print.has_value());
    auto value = print.value();
    auto expr = node.getExpr();
    auto llvmVal = apply(*expr);
    builder_->CreateCall(value.callee_, {llvmVal});
    return nullptr;
  }

  llvm::Value *visit(CompoundStmt &node) {
    ScopeLLVMVal scope;
    scopes_.beginScope(scope);
    for (auto &&[_, decl] : node.getScope())
      apply(*decl);
    for (auto &&stmt : node)
      apply(*stmt);
    scopes_.endScope();
    return nullptr;
  }

  llvm::Value *visit(DeclStmt &node) { return nullptr; }

  llvm::Value *visit(ValueStmt &node) {
    auto expr = node.getExpr();
    apply(*expr);
    return nullptr;
  }

  llvm::Value *visit(IfStmt &node) {
    auto cond = node.getCond();
    auto then = node.getThen();
    auto Else = node.getElse();
    auto thenBlock = createBlock("then");
    auto elseBlock = Else ? createBlock("else") : nullptr;
    auto contBlock = createBlock("cont");

    auto llvmCond = apply(*cond);
    auto llvmBoolCond = builder_->CreateIsNotNull(llvmCond);
    auto nextBlock = elseBlock ? elseBlock : contBlock;
    builder_->CreateCondBr(llvmBoolCond, thenBlock, nextBlock);

    auto compileBB = [&](Stmt *stmt, llvm::BasicBlock *bb) {
      builder_->SetInsertPoint(bb);
      apply(*stmt);
      bb = builder_->GetInsertBlock();
      if (bb->empty() || !(llvm::isa<llvm::BranchInst>(bb->back()) ||
                           llvm::isa<llvm::ReturnInst>(bb->back())))
        builder_->CreateBr(contBlock);
    };

    compileBB(then, thenBlock);
    if (Else)
      compileBB(Else, elseBlock);
    builder_->SetInsertPoint(contBlock);
    return nullptr;
  }

  llvm::Value *visit(WhileStmt &node) {
    auto cond = node.getCond();
    auto body = node.getBody();
    auto condBlock = createBlock("cond");
    auto bodyBlock = createBlock("body");
    auto exitBlock = createBlock("exit");
    auto contBlock = createBlock("cont");
    loops_.emplace_back(std::addressof(node), condBlock, exitBlock);

    builder_->CreateBr(condBlock);
    builder_->SetInsertPoint(condBlock);
    auto llvmCond = apply(*cond);
    auto llvmBoolCond = builder_->CreateIsNotNull(llvmCond);
    builder_->CreateCondBr(llvmBoolCond, bodyBlock, exitBlock);
    builder_->SetInsertPoint(bodyBlock);
    apply(*body);
    builder_->CreateBr(condBlock);
    builder_->SetInsertPoint(exitBlock);
    builder_->CreateBr(contBlock);
    builder_->SetInsertPoint(contBlock);
    loops_.pop_back();
    return nullptr;
  }

  llvm::Value *visit(BreakStmt &node) {
    const auto &back = loops_.back();
    assert(back.loop_ == node.getLoopStmt());
    auto bb = back.exit_;
    builder_->CreateBr(bb);
    return nullptr;
  }

  llvm::Value *visit(ContinueStmt &node) {
    const auto &back = loops_.back();
    assert(back.loop_ == node.getLoopStmt());
    auto bb = back.cond_;
    builder_->CreateBr(bb);
    return nullptr;
  }

  llvm::Value *visit(ReturnStmt &node) {
    auto expr = node.getExpr();
    if (expr == nullptr) {
      builder_->CreateRetVoid();
      return nullptr;
    }
    auto llvmVal = apply(*expr);
    builder_->CreateRet(llvmVal);
    return nullptr;
  }

  llvm::Value *visit(TranslationUnit &node) {
    main_ = node.getMain();
    ScopeLLVMVal scope;
    scopes_.beginScope(scope);
    for (auto &&decl : node)
      apply(*decl);
    scopes_.endScope();
    module_->print(llvm::outs(), nullptr);
    llvm::verifyModule(*module_);
    return nullptr;
  }
};

void execute(std::string_view filepath, TranslationUnit *root) {
  auto ctx = llvm::LLVMContext{};
  auto generator = IRGenerator{filepath, ctx};
  generator.apply(*root);
  auto mod = generator.releaseModule();

  llvm::outs() << "[EE] Run\n";
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  auto ee = llvm::EngineBuilder(std::move(mod)).create();
  ee->InstallLazyFunctionCreator([&](std::string_view funcName) -> void * {
    return generator.getExternFuncPtr(funcName);
  });
  ee->finalizeObject();
  auto v = ee->runFunction(generator.getMainFunc(), {});
  llvm::outs() << "[EE] Result: " << v.IntVal << "\n";
}

} // namespace paracl
