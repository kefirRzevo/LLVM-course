#include <vector>

#include "frontend/Driver.hh"
#include "frontend/Errors.hh"
#include "frontend/Nodes.hh"
#include "frontend/SymTables.hh"
#include "frontend/Types.hh"

namespace paracl {

class SemanticAnalyzer final
    : public visitor::VisitorBase<INode, SemanticAnalyzer, bool> {
  Driver &driver_;
  ScopeDeclStack scopes_;
  FunctionDecl* curFunc_;
  FunctionDecl* mainFunc_;
  std::vector<WhileStmt *> loops_;

  Expr *createLValToRValCast(Expr *expr) {
    assert(expr);
    const auto &loc = expr->getLoc();
    auto type = expr->getType();
    assert(type);
    auto castType = ImplicitCastType::LValueToRValue;
    auto node = driver_.createNode<ImplicitCastExpr>(loc, expr, castType);
    assert(node);
    switch (type->getKind()) {
    case TypeKind::ArrayType: {
      castType = ImplicitCastType::ArrayToPointerDecay;
      auto asArray = static_cast<ArrayType *>(type);
      auto elemType = asArray->getElemType();
      assert(elemType);
      type = driver_.getPointerType(elemType);
      assert(type);
      break;
    }
    case TypeKind::FunctionType: {
      castType = ImplicitCastType::FunctionToPointerDecay;
      type = driver_.getPointerType(type);
      assert(type);
      break;
    }
    case TypeKind::IntType:
    case TypeKind::VoidType:
    case TypeKind::PointerType:
      break;
    default:
      throw std::runtime_error("Unknown expr type");
    }
    node->setImplicitCastType(castType);
    node->setType(type);
    node->setKind(ExprKind::RVal);
    return node;
  }

public:
  VISIT(AllNodes);

  SemanticAnalyzer(Driver &driver) : driver_(driver) {
    auto root = driver.getRoot();
    assert(root);
    mainFunc_ = root->getMain();
    assert(mainFunc_);
  }

  bool visit(UnaryOperator &node) {
    const auto &loc = node.getLoc();
    auto expr = node.getSubExpr();
    assert(expr);
    if (!apply(*expr))
      return false;
    if (expr->isLValue()) {
      auto cast = createLValToRValCast(expr);
      assert(cast);
      node.setSubExpr(cast);
      expr = cast;
    }
    auto type = expr->getType();
    assert(type);
    auto intType = driver_.getIntType();
    assert(intType);
    if (type != intType) {
      driver_.reportError<MismatchingTypeUnaryExpr>(loc, type->getAsString());
      return false;
    }
    node.setType(intType);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(BinaryOperator &node) {
    const auto &loc = node.getLoc();
    auto lhs = node.getLHS();
    assert(lhs);
    auto rhs = node.getRHS();
    assert(rhs);
    auto opc = node.getOpcode();

    if (isArithmetic(opc)) {
      if (!apply(*rhs))
        return false;
      if (rhs->isLValue()) {
        auto cast = createLValToRValCast(rhs);
        assert(cast);
        node.setRHS(cast);
        rhs = cast;
      }
      if (!apply(*lhs))
        return false;
      if (lhs->isLValue()) {
        auto cast = createLValToRValCast(lhs);
        assert(cast);
        node.setLHS(cast);
        lhs = cast;
      }
      auto lhsType = lhs->getType();
      assert(lhsType);
      auto rhsType = rhs->getType();
      assert(rhsType);
      auto intType = driver_.getIntType();
      assert(intType);
      if (lhsType != intType || rhsType != intType) {
        driver_.reportError<MismatchingTypeBinaryExpr>(
            loc, lhsType->getAsString(), rhsType->getAsString());
        return false;
      }
      node.setType(intType);
      node.setKind(ExprKind::RVal);
      return true;
    } else if (opc == BinaryOpcode::BIN_ASSIGN) {
      if (!apply(*rhs))
        return false;
      if (rhs->isLValue()) {
        auto cast = createLValToRValCast(rhs);
        assert(cast);
        node.setRHS(cast);
        rhs = cast;
      }
      if (!apply(*lhs))
        return false;
      if (!lhs->isLValue()) {
        driver_.reportError<UnassignableExpression>(loc);
        return false;
      }
      auto lhsType = lhs->getType();
      assert(lhsType);
      auto rhsType = rhs->getType();
      assert(rhsType);
      if (lhsType != rhsType) {
        driver_.reportError<MismatchingTypeAssignExpr>(
            loc, lhsType->getAsString(), rhsType->getAsString());
        return false;
      }
      node.setType(lhsType);
      node.setKind(lhs->getKind());
      return true;
    } else if (opc == BinaryOpcode::BIN_COMMA) {
      if (!apply(*lhs))
        return false;
      if (!apply(*rhs))
        return false;
      auto rhsType = rhs->getType();
      assert(rhsType);
      node.setType(rhsType);
      node.setKind(rhs->getKind());
      return true;
    } 
    throw std::runtime_error("Unknown binary opcode");
  }

  bool visit(ConditionalOperator &node) {
    const auto &loc = node.getLoc();
    auto cond = node.getCondExpr();
    assert(cond);
    if (!apply(*cond))
      return false;
    auto condType = cond->getType();
    assert(condType);
    auto intType = driver_.getIntType();
    assert(intType);
    if (condType != intType) {
      driver_.reportError<IntTypeRequired>(loc, condType->getAsString());
      return false;
    }
    auto lhs = node.getTrueExpr();
    assert(lhs);
    auto rhs = node.getFalseExpr();
    assert(rhs);
    if (!apply(*rhs))
      return false;
    if (rhs->isLValue()) {
      auto cast = createLValToRValCast(rhs);
      assert(cast);
      node.setTrueExpr(cast);
      rhs = cast;
    }
    if (!apply(*lhs))
      return false;
    if (lhs->isLValue()) {
      auto cast = createLValToRValCast(lhs);
      assert(cast);
      node.setFalseExpr(cast);
      lhs = cast;
    }
    auto lhsType = lhs->getType();
    assert(lhsType);
    auto rhsType = rhs->getType();
    assert(rhsType);
    if (lhsType != intType || rhsType != intType) {
      driver_.reportError<MismatchingTypeBinaryExpr>(
          loc, lhsType->getAsString(), rhsType->getAsString());
      return false;
    }
    node.setType(intType);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(IntegerLiteral &node) {
    auto intType = driver_.getIntType();
    assert(intType);
    node.setType(intType);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(UndefLiteral &node) {
    auto intType = driver_.getIntType();
    assert(intType);
    node.setType(intType);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(DeclRefExpr &node) {
    const auto &loc = node.getLoc();
    auto name = node.getName();
    auto declOpt = scopes_.lookup(name);
    if (!declOpt.has_value()) {
      driver_.reportError<UndeclaredIdentifier>(loc, std::string{name});
      return false;
    }
    auto decl = declOpt.value();
    assert(decl);
    node.setDecl(decl);
    auto declType = decl->getType();
    assert(declType);
    node.setType(declType);
    node.setKind(ExprKind::LVal);
    return true;
  }

  bool visit(ImplicitCastExpr &node) {
    auto expr = node.getExpr();
    assert(expr);
    if (!apply(*expr))
      return false;
    auto type = expr->getType();
    assert(type);
    node.setType(type);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(ArraySubscriptExpr &node) {
    const auto &loc = node.getLoc();
    auto lhs = node.getLHS();
    assert(lhs);
    auto rhs = node.getRHS();
    assert(rhs);
    if (!apply(*rhs))
      return false;
    auto rhsType = rhs->getType();
    assert(rhsType);
    auto intType = driver_.getIntType();
    assert(intType);
    if (rhsType != intType) {
      driver_.reportError<IntTypeRequired>(loc, rhsType->getAsString());
      return false;
    }
    if (!apply(*lhs))
      return false;
    if (lhs->isLValue()) {
      auto cast = createLValToRValCast(lhs);
      assert(cast);
      node.setLHS(cast);
      lhs = cast;
    }
    auto lhsType = lhs->getType();
    assert(lhsType);
    if (!ElementedType::isElemented(lhsType->getKind())) {
      driver_.reportError<SubscriptedValue>(loc);
      return false;
    }
    auto asElemented = static_cast<ElementedType *>(lhsType);
    auto type = asElemented->getElemType();
    assert(type);
    if (type->getKind() != TypeKind::IntType &&
        type->getKind() != TypeKind::ArrayType) {
      driver_.reportError<Subscript>(loc, type->getAsString());
      return false;
    }
    node.setType(type);
    node.setKind(ExprKind::LVal);
    return true;
  }

  bool visit(InitListExpr &node) {
    const auto &loc = node.getLoc();
    if (node.empty()) {
      auto intType = driver_.getIntType();
      assert(intType);
      auto type = driver_.getArrayType(intType, 0);
      assert(type);
      node.setType(type);
      node.setKind(ExprKind::RVal);
      return true;
    }
    auto hasErr = bool{false};
    std::transform(node.begin(), node.end(), node.begin(), [&](auto &&expr) {
      assert(expr);
      if (!apply(*expr))
        hasErr = true;
      if (expr->isLValue())
        return createLValToRValCast(expr);
      return expr;
    });
    if (hasErr)
      return false;
    if (std::adjacent_find(node.begin(), node.end(),
                           [=](auto &&lhs, auto &&rhs) {
                             auto lhsType = lhs->getType();
                             auto rhsType = rhs->getType();
                             assert(lhsType);
                             assert(rhsType);
                             return lhsType != rhsType;
                           }) != node.end()) {
      driver_.reportError<MismatchingType>(loc);
      return false;
    }
    auto elemType = (*node.begin())->getType();
    assert(elemType);
    auto type = driver_.getArrayType(elemType, node.size());
    assert(type);
    node.setType(type);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(CallExpr &node) {
    const auto &loc = node.getLoc();
    auto call = node.getCall();
    assert(call);
    if (!apply(*call))
      return false;
    auto callType = call->getType();
    auto hasErr = bool{false};
    if (callType->getKind() != TypeKind::FunctionType)
      hasErr = true;
    if (callType->getKind() == TypeKind::PointerType) {
        auto asPointer = static_cast<PointerType*>(callType);
        auto elemType = asPointer->getElemType();
        if (elemType->getKind() == TypeKind::FunctionType) {
          hasErr = false;
          callType = elemType;
        }
    }
    if (hasErr) {
      driver_.reportError<MismatchingType>(loc);
      return false;
    }
    auto asFunction = static_cast<FunctionType *>(callType);
    std::transform(node.begin(), node.end(), node.begin(), [&](auto &&expr) {
      assert(expr);
      if (!apply(*expr))
        hasErr = true;
      if (!hasErr && expr->isLValue())
        return createLValToRValCast(expr);
      return expr;
    });
    if (hasErr)
      return false;
    auto paramTypes = std::vector<Type *>{};
    std::transform(node.begin(), node.end(), std::back_inserter(paramTypes),
                   [](auto &&param) {
                     auto paramType = param->getType();
                     assert(paramType);
                     return paramType;
                   });
    auto retType = asFunction->getRetType();
    assert(retType);
    if (!std::equal(paramTypes.begin(), paramTypes.end(), asFunction->begin(),
                    asFunction->end())) {
      auto lhs = FunctionType::getTypesAsString(asFunction->begin(),
                                                asFunction->end());
      auto rhs =
          FunctionType::getTypesAsString(paramTypes.begin(), paramTypes.end());
      driver_.reportError<MismatchingFunctionTypes>(loc, lhs, rhs);
      return false;
    }
    if (call->isLValue()) {
      auto cast = createLValToRValCast(call);
      assert(cast);
      node.setCall(cast);
      call = cast;
    }
    node.setType(retType);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(InputExpr &node) {
    auto intType = driver_.getIntType();
    assert(intType);
    node.setType(intType);
    node.setKind(ExprKind::RVal);
    return true;
  }

  bool visit(VarDecl &node) {
    const auto &loc = node.getLoc();
    auto expr = node.getExpr();
    assert(expr);
    if (!apply(*expr))
      return false;
    auto name = node.getName();
    if (scopes_.declared(name)) {
      driver_.reportError<Redefinition>(loc, std::string{name});
      return false;
    }
    scopes_.declare(name, std::addressof(node));
    if (expr->isLValue()) {
      auto cast = createLValToRValCast(expr);
      assert(cast);
      node.setExpr(cast);
      expr = cast;
    }
    auto type = node.getType();
    assert(type);
    auto exprType = expr->getType();
    assert(exprType);
    if (exprType->getKind() == TypeKind::ArrayType && type->getKind() == TypeKind::ArrayType) {
      auto exprTypeAsArr = static_cast<ArrayType*>(exprType);
      auto typeAsArr = static_cast<ArrayType*>(type);
      auto exprElemType = exprTypeAsArr->getElemType();
      auto exprElemCount = exprTypeAsArr->getElemCount();
      auto elemType = typeAsArr->getElemType();
      auto elemCount = typeAsArr->getElemCount();
      if (exprElemType == elemType && exprElemCount != elemCount) {
        auto correctExprType = driver_.getArrayType(elemType, elemCount);
        expr->setType(correctExprType);
        exprType = correctExprType;
      } 
    }
    if (type != exprType) {
      driver_.reportError<MismatchingTypeAssignExpr>(loc, type->getAsString(),
                                                     exprType->getAsString());
      return false;
    }
    return true;
  }

  bool visit(ParmVarDecl &node) {
    const auto &loc = node.getLoc();
    auto name = node.getName();
    if (scopes_.declared(name)) {
      driver_.reportError<Redefinition>(loc, std::string{name});
      return false;
    }
    scopes_.declare(name, std::addressof(node));
    return true;
  }

  bool visit(FunctionDecl &node) {
    const auto &loc = node.getLoc();
    auto name = node.getName();
    if (scopes_.declared(name)) {
      driver_.reportError<Redefinition>(loc, std::string{name});
      return false;
    }
    curFunc_ = std::addressof(node);
    scopes_.declare(name, curFunc_);
    scopes_.beginScope(node.getScope());
    for (auto &&param : node) {
      assert(param);
      apply(*param);
    }
    auto body = node.getBody();
    assert(body);
    apply(*body);
    scopes_.endScope();
    return true;
  }

  bool visit(ExternFunctionDecl &node) {
    const auto &loc = node.getLoc();
    auto name = node.getName();
    if (scopes_.declared(name)) {
      driver_.reportError<Redefinition>(loc, std::string{name});
      return false;
    }
    scopes_.declare(name, std::addressof(node));
    return true;
  }

  bool visit(OutputStmt &node) {
    const auto &loc = node.getLoc();
    auto expr = node.getExpr();
    assert(expr);
    if (!apply(*expr))
      return false;
    if (expr->isLValue()) {
      auto cast = createLValToRValCast(expr);
      assert(cast);
      node.setExpr(cast);
      expr = cast;
    }
    auto exprType = expr->getType();
    assert(exprType);
    if (exprType != driver_.getIntType()) {
      driver_.reportError<IntTypeRequired>(loc, exprType->getAsString());
      return false;
    }
    return true;
  }

  bool visit(CompoundStmt &node) {
    scopes_.beginScope(node.getScope());
    for (auto &&stmt : node) {
      assert(stmt);
      apply(*stmt);
    }
    scopes_.endScope();
    return true;
  }

  bool visit(DeclStmt &node) {
    for (auto &&decl : node) {
      assert(decl);
      apply(*decl);
    }
    return true;
  }

  bool visit(ValueStmt &node) {
    const auto &loc = node.getLoc();
    auto expr = node.getExpr();
    assert(expr);
    return apply(*expr);
  }

  bool visit(IfStmt &node) {
    const auto &loc = node.getLoc();
    auto cond = node.getCond();
    assert(cond);
    if (apply(*cond)) {
      auto condType = cond->getType();
      assert(condType);
      if (condType != driver_.getIntType()) {
        driver_.reportError<IntTypeRequired>(loc, condType->getAsString());
      } else if (cond->isLValue()) {
        auto cast = createLValToRValCast(cond);
        assert(cast);
        node.setCond(cast);
      }
    }
    auto then = node.getThen();
    assert(then);
    apply(*then);
    auto Else = node.getElse();
    if (Else)
      apply(*Else);
    return true;
  }

  bool visit(WhileStmt &node) {
    loops_.push_back(std::addressof(node));
    const auto &loc = node.getLoc();
    auto cond = node.getCond();
    assert(cond);
    if (apply(*cond)) {
      auto condType = cond->getType();
      assert(condType);
      if (condType != driver_.getIntType()) {
        driver_.reportError<IntTypeRequired>(loc, condType->getAsString());
      } else if (cond->isLValue()) {
        auto cast = createLValToRValCast(cond);
        assert(cast);
        node.setCond(cast);
      }
    }
    auto body = node.getBody();
    assert(body);
    apply(*body);
    loops_.pop_back();
    return true;
  }

  bool visit(BreakStmt &node) {
    const auto &loc = node.getLoc();
    if (loops_.empty()) {
      driver_.reportError<OutOfLoopStatement>(loc, "break");
      return false;
    }
    node.setLoopStmt(loops_.back());
    return true;
  }

  bool visit(ContinueStmt &node) {
    const auto &loc = node.getLoc();
    if (loops_.empty()) {
      driver_.reportError<OutOfLoopStatement>(loc, "continue");
      return false;
    }
    node.setLoopStmt(loops_.back());
    return true;
  }

  bool visit(ReturnStmt &node) {
    const auto &loc = node.getLoc();
    if (curFunc_ == mainFunc_) {
      driver_.reportError<OutOfFunctionStatement>(loc);
      return false;
    }
    auto type = static_cast<Type *>(driver_.getVoidType());
    auto expr = node.getExpr();
    if (expr != nullptr) {
      if (!apply(*expr))
        return false;
      type = expr->getType();
      if (expr->isLValue()) {
        auto cast = createLValToRValCast(expr);
        assert(cast);
        node.setExpr(cast);
        expr = cast;
      }
    }
    auto funcType = curFunc_->getType();
    assert(funcType);
    auto asFunction = static_cast<FunctionType *>(funcType);
    auto retType = asFunction->getRetType();
    assert(retType);
    assert(type);
    if (retType != type) {
      driver_.reportError<IncorrectReturn>(loc, retType->getAsString(),
                                           type->getAsString());
      return false;
    }
    return true;
  }

  bool visit(TranslationUnit &node) {
    auto& globalScope = node.getScope();
    scopes_.beginScope(globalScope);
    for (auto &&decl : node) {
      assert(decl);
      apply(*decl);
    }
    scopes_.endScope();
    return true;
  }
};

void semanticAnalyze(Driver& driver) {
	SemanticAnalyzer analyzer{driver};
  auto root = driver.getRoot();
  auto main = root->getMain();
  root->push_back(main);
  analyzer.apply(*root);
  auto retStmt = driver.createNode<ReturnStmt>(location{}, nullptr);
  auto body = main->getBody();
  body->push_back(retStmt);
}

} // namespace paracl
