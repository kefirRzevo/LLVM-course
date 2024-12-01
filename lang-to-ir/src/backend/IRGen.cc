// #include <vector>

// #include "frontend/Driver.hh"
// #include "frontend/Nodes.hh"
// #include "frontend/SymTables.hh"
// #include "frontend/Types.hh"
// #include "visitor/Visitor.hh"

// #include <llvm/ExecutionEngine/ExecutionEngine.h>
// #include <llvm/ExecutionEngine/GenericValue.h>
// #include <llvm/IR/IRBuilder.h>
// #include <llvm/IR/LLVMContext.h>
// #include <llvm/IR/Module.h>
// #include <llvm/Support/TargetSelect.h>
// #include <llvm/Support/raw_ostream.h>

// namespace paracl {

// class IRGenerator final
//     : public visitor::VisitorBase<INode, IRGenerator, llvm::Value*> {
//   using ScopeLLVMVal = SearchTable<llvm::Value *>;
//   using ScopeLLVMValStack = SearchTableStack<llvm::Value *>;

//   Driver &driver_;
//   std::unique_ptr<llvm::Module> module_;
//   std::unique_ptr<llvm::IRBuilder<>> builder_;

//   ScopeLLVMValStack currentFrame_;
//   std::unordered_map<Expr *, llvm::Value *> valueMap_;
//   std::unordered_map<FunctionDecl *, llvm::Function *> funcDecls_;
// 	std::unordered_map<FunctionDecl *, llvm::Function *> externFuncDecls_;
//   llvm::Function *mainFunc_ = nullptr;
//   llvm::Function *curFunc_ = nullptr;

//   llvm::Type *getLLVMType(Type *type) {
//     switch (type->getKind()) {
//     case TypeKind::IntType:
//       return builder_->getInt32Ty();
//     case TypeKind::VoidType:
//       return builder_->getVoidTy();
//     case TypeKind::ArrayType: {
//       auto asArray = static_cast<ArrayType *>(type);
//       auto elemType = asArray->getElemType();
//       auto llvmElemType = getLLVMType(elemType);
//       return llvm::ArrayType::get(llvmElemType, asArray->getElemCount());
//     }
//     case TypeKind::PointerType:
//       return builder_->getPtrTy();
//     case TypeKind::FunctionType: {
//       auto asFunc = static_cast<FunctionType *>(type);
//       auto llvmParamTypes = std::vector<llvm::Type *>{};
//       std::transform(asFunc->begin(), asFunc->end(),
//                      std::back_inserter(llvmParamTypes),
//                      [&](auto &&paramType) { return getLLVMType(paramType); });
//       auto retType = asFunc->getRetType();
//       auto llvmRetType = getLLVMType(retType);
//       return llvm::FunctionType::get(llvmRetType, std::move(llvmParamTypes),
//                                      false);
//     }
//     default:
//       throw std::runtime_error("Unknown expr type");
//     }
//   }

// public:
// 	VISIT(AllNodes);

//   IRGenerator(Driver &driver, llvm::LLVMContext &ctx)
//       : driver_(driver),
//         module_(std::make_unique<llvm::Module>(driver.getModuleName(), ctx)),
//         builder_(std::make_unique<llvm::IRBuilder<>>(ctx)) {}

// 	void generateIR() {

// 	}

// 	std::unique_ptr<llvm::Module> releaseModule() {
// 		return std::move(module_);
// 	}

// 	llvm::Function* getMain() { return mainFunc_; }

//   llvm::Value* visit(UnaryOperator &node) {
// 		auto expr = node.getSubExpr();
// 		auto llvmVal = apply(*expr);
// 		switch (node.getOpcode()) {
// 		case UnaryOpcode::UN_SUB:
// 			return builder_->CreateNeg(llvmVal);
// 		case UnaryOpcode::UN_ADD:
// 			return llvmVal;
// 		case UnaryOpcode::UN_NOT:
// 			return builder_->CreateNot(llvmVal);
// 		default:
// 			throw std::runtime_error("Unknown unary opcode");
// 		}
// 	}

//   llvm::Value* visit(BinaryOperator &node) {
// 		auto lhs = node.getLHS();
// 		auto rhs = node.getRHS();
// 		auto opc = node.getOpcode();
// 		if (isArithmetic(opc)) {
// 			auto llvmLhs = apply(*lhs);
// 			auto llvmRhs = apply(*rhs);
// 			switch (opc) {
// 			case BinaryOpcode::BIN_MUL:
// 				return builder_->CreateMul(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_DIV:
// 				return builder_->CreateSDiv(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_MOD:
// 				return builder_->CreateSRem(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_ADD:
// 				return builder_->CreateAdd(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_SUB:
// 				return builder_->CreateSub(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_L:
// 				return builder_->CreateICmpSLT(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_G:
// 				return builder_->CreateICmpSGT(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_LE:
// 				return builder_->CreateICmpSLE(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_GE:
// 				return builder_->CreateICmpSGE(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_EQ:
// 				return builder_->CreateICmpEQ(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_NE:
// 				return builder_->CreateICmpNE(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_AND:
// 				return builder_->CreateAnd(llvmLhs, llvmRhs);
// 			case BinaryOpcode::BIN_OR:
// 				return builder_->CreateOr(llvmLhs, llvmRhs);
// 			default:
// 				assert(0 && "unreachable");
// 			}
// 		} else if (opc == BinaryOpcode::BIN_ASSIGN) {
// 			auto llvmRhs = apply(*rhs);
// 			auto llvmLhs = apply(*lhs);
// 			return builder_->CreateStore(llvmRhs, llvmLhs);
// 		} else if (opc == BinaryOpcode::BIN_COMMA) {
// 			apply(*lhs);
// 			auto llvmRhs = apply(*rhs);
// 			return llvmRhs;
// 		}
// 		throw std::runtime_error("Unknown binary opcode");
// 	}

//   llvm::Value* visit(ConditionalOperator &node) {
//     auto cond = node.getCondExpr();
// 		auto llvmCond = apply(*cond);
// 		auto lhs = node.getTrueExpr();
// 		auto llvmLhs = apply(*lhs);
// 		auto rhs = node.getFalseExpr();
// 		auto llvmRhs = apply(*rhs);
// 		return builder_->CreateSelect(llvmCond, llvmLhs, llvmRhs);
// 	}

//   llvm::Value* visit(IntegerLiteral &node) {
// 		auto val = node.getVal();
// 		return builder_->getInt32(val);
// 	}

//   llvm::Value* visit(UndefLiteral &node) {
// 		auto val = std::rand() % 100;
// 		return builder_->getInt32(val);
// 	}

//   llvm::Value* visit(DeclRefExpr &node) {
// 		auto name = node.getName();
// 		return 
// 	}

//   llvm::Value* visit(ImplicitCastExpr &node) {}

//   llvm::Value* visit(ArraySubscriptExpr &node) {}

//   llvm::Value* visit(InitListExpr &node) {}

//   llvm::Value* visit(CallExpr &node) {}

//   llvm::Value* visit(InputExpr &node) {}

//   llvm::Value* visit(VarDecl &node) {}

//   llvm::Value* visit(ParmVarDecl &node) {}

//   llvm::Value* visit(FunctionDecl &node) {}

//   llvm::Value* visit(ExternFunctionDecl &node) {}

//   llvm::Value* visit(OutputStmt &node) {}

//   llvm::Value* visit(CompoundStmt &node) {}

//   llvm::Value* visit(DeclStmt &node) {}

//   llvm::Value* visit(ValueStmt &node) {}

//   llvm::Value* visit(IfStmt &node) {}

//   llvm::Value* visit(WhileStmt &node) {}

//   llvm::Value* visit(BreakStmt &node) {}

//   llvm::Value* visit(ContinueStmt &node) {}

//   llvm::Value* visit(ReturnStmt &node) {}
// };

// void execute(Driver &driver) {
//   llvm::LLVMContext ctx;
//   IRGenerator generator{driver, ctx};
// 	auto mod = generator.releaseModule();

//   ExecutionEngine *ee = EngineBuilder(mod).create();

//   ee->InstallLazyFunctionCreator([&](std::string_view funcName) -> void * {
//     if (fnName == "gui_rand") {
//       return reinterpret_cast<void *>(gui_rand);
//     }
//     if (fnName == "gui_set_pixel") {
//       return reinterpret_cast<void *>(gui_set_pixel);
//     }
//     if (fnName == "gui_flush") {
//       return reinterpret_cast<void *>(gui_flush);
//     }
//     if (fnName == "gui_quit_event") {
//       return reinterpret_cast<void *>(gui_quit_event);
//     }
//     return nullptr;
//   });
//   ee->finalizeObject();
//   GenericValue v = ee->runFunction(appFunc, noargs);
//   outs() << "[EE] Result: " << v.IntVal << "\n";
// }

// } // namespace paracl
