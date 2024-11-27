#pragma once

#include <vector>

#include "frontend/Driver.hh"
#include "frontend/Errors.hh"
#include "frontend/Nodes.hh"
#include "frontend/SymTables.hh"

namespace paracl {

class SemanticAnalyzer final
    : public visitor::VisitorBase<INode, SemanticAnalyzer, void> {
  Driver &driver_;
  ScopeStack scopes_;

  std::vector<WhileStmt *> loops_;
  const Type *intType_ = nullptr;
  const Type *noType_ = nullptr;

  bool trySetIntCTI(Expr &node) const {
    auto &cti = node.getCTI();
    if (cti.index() == IndexCTI::ExprCti) {
      auto &asExpr = std::get<ExprCTI>(cti);
      auto expr = asExpr.getExpr();
      auto &exprCti = expr->getCTI();
      if (exprCti.index() == IndexCTI::IntCti) {
        node.setCTI(exprCti);
        return true;
      }
    }
    return false;
  }

public:
  VISIT(AllNodes);

  SemanticAnalyzer(Driver &driver) : driver_(driver) {}

  void analyze() {
    if (!noType_ || !intType_) {
      noType_ = driver_.createNoType();
      auto intTypeCat = driver_.createTypeCategory<IntType>();
      intType_ = driver_.pushBack(noType_, intTypeCat);
    }
    if (root) {
      apply(*root);
    }
  }

  void visit(UnaryOperator &node) {
    auto loc = node.getLoc();
    auto op = node.getOpcode();
    auto subExpr = node.getSubExpr();

    apply(*subExpr);
    node.setType(noType_);
    if (isPostfix(op) || isPrefix(op)) {
      if (!subExpr->isLValue()) {
        driver_.reportError<UnassignableExpression>(loc);
        return;
      }
    }
    auto type = subExpr->getType();
    if (type != intType_) {
      auto typeString = type->getAsString();
      driver_.reportError<MismatchingTypeUnaryExpr>(loc, typeString);
      return;
    }
    node.setType(intType_);
  }

  void visit(BinaryOperator &node) {
    auto loc = node.getLoc();
    auto lhs = node.getLHS();
    auto rhs = node.getRHS();
    auto opc = node.getOpcode();

    auto assign = opc == BinaryOpcode::BIN_ASSIGN;
    if (assign) {
      apply(*rhs);
      apply(*lhs);
      lhs->setCTI(NoCTI{});
      node.setType(noType_);
      if (!lhs->isLValue()) {
        driver_.reportError<UnassignableExpression>(loc);
        return;
      }
    } else {
      apply(*lhs);
      apply(*rhs);
      node.setType(noType_);
    }
    auto lhsType = lhs->getType();
    auto rhsType = rhs->getType();
    if (lhsType != intType_ || rhsType != intType_) {
      auto lhsTypeString = lhsType->getAsString();
      auto rhsTypeString = rhsType->getAsString();
      assign
          ? driver_.reportError<MismatchingTypeAssignExpr>(loc, lhsTypeString,
                                                           rhsTypeString)
          : driver_.reportError<MismatchingTypeBinaryExpr>(loc, lhsTypeString,
                                                           rhsTypeString);
      return;
    }
    node.setType(intType_);
  }

  void visit(ConditionalOperator &node) {
    auto loc = node.getLoc();
    auto cond = node.getCondExpr();
    auto lhs = node.getTrueExpr();
    auto rhs = node.getFalseExpr();

    apply(*cond);
    apply(*lhs);
    apply(*rhs);
    node.setType(noType_);
    auto condType = cond->getType();
    auto lhsType = lhs->getType();
    auto rhsType = rhs->getType();
    if (condType != intType_) {
      auto condTypeString = condType->getAsString();
      driver_.reportError<IntTypeRequired>(loc, condTypeString);
      return;
    }
    if (lhsType != rhsType) {
      auto lhsTypeString = lhsType->getAsString();
      auto rhsTypeString = rhsType->getAsString();
      driver_.reportError<MismatchingTypeBinaryExpr>(loc, lhsTypeString,
                                                     rhsTypeString);
      return;
    }
    node.setType(intType_);
  }

  void visit(IntegerLiteral &node) {
    node.setCTI(IntCTI{node.getVal()});
    node.setType(intType_);
  }

  void visit(UndefLiteral &node) { node.setType(intType_); }

  void visit(DeclExpr &node) {
    auto loc = node.getLoc();
    auto decl = node.getDecl();
    auto name = decl->getName();

    node.setType(noType_);
    if (!scopes_.declared(name)) {
      driver_.reportError<UndeclaredIdentifier>(loc, name);
      return;
    }
    auto expr = decl->getExpr();
    if (expr->hasCTI()) {
      node.setCTI(expr->getCTI());
      trySetIntCTI(node);
    }
    node.setType(decl->getType());
  }

  void visit(LValueToRValue &node) {
    auto expr = node.getExpr();
    apply(*expr);
    node.setVal(Value::RVal);
    node.setType(expr->getType());
  }

  void visit(ArraySubscriptExpr &node) {
    auto loc = node.getLoc();
    auto lhs = node.getLHS();
    auto rhs = node.getRHS();

    apply(*lhs);
    apply(*rhs);
    node.setType(noType_);
    auto rhsType = rhs->getType();
    if (rhsType != intType_) {
      auto rhsTypeString = rhsType->getAsString();
      driver_.reportError<IntTypeRequired>(loc, rhsTypeString);
      return;
    }

    auto lhsType = lhs->getType();
    auto frontCat = lhsType->front();
    auto kind = frontCat->getTypeCategoryKind();
    if (kind == TypeCategoryKind::CTArray) {
      auto asArr = static_cast<const CTArrayType *>(frontCat);
      auto arrSize = asArr->getCTSize();
      auto rhsCti = rhs->getCTI();
      if (rhsCti.index() == IndexCTI::IntCti) {
        auto indx = std::get<IntCTI>(rhsCti).getVal();
        if (indx + 1 > static_cast<int>(arrSize)) {
          driver_.reportError<ArrayBounds>(loc, indx, arrSize);
        }
        auto lhsCti = lhs->getCTI();
        if (lhsCti.index() == IndexCTI::ExprCti) {
          auto expr = std::get<ExprCTI>(lhsCti).getExpr();
          if (expr->getNodeType() == NodeType::InitListExpr) {
            auto asList = static_cast<InitListExpr *>(expr);
            auto nodeCti = asList->getExpr(indx);
            if (nodeCti.has_value()) {
              node.setCTI(ExprCTI{nodeCti.value()});
            }
          }
        }
      }
    } else if (kind != TypeCategoryKind::RTArray) {
      driver_.reportError<SubscriptedValue>(loc);
      return;
    }
    node.setType(driver_.popBack(lhsType));
  }

  void visit(InitListExpr &node) {
    auto loc = node.getLoc();

    if (node.empty()) {
      throw std::runtime_error("Init list is empty");
    }
    for (auto &&expr : node) {
      apply(*expr);
    }
    node.setType(noType_);
    auto firType = node.front()->getType();
    auto sameType = std::all_of(node.begin(), node.end(), [&](auto &&it) {
      return it->getType() == firType;
    });
    if (!sameType) {
      driver_.reportError<MismatchingType>(loc);
      return;
    }
    auto frontSizeCT = firType->front()->getCTSize();
    auto sizeCt = node.getSizeCT();
    if (sizeCt.has_value())
    std::cout << sizeCt.value() << "\t" << frontSizeCT << std::endl;
    Type typeVal;
    if (sizeCt.has_value()) {
      node.setCTI(IntCTI{static_cast<int>(sizeCt.value())});
      auto ctArrCat = driver_.createTypeCategory<CTArrayType>(frontSizeCT, sizeCt.value());
      node.setType(driver_.pushBack(firType, ctArrCat));
    } else {
      auto rtArrCat = driver_.createTypeCategory<RTArrayType>(frontSizeCT);
      node.setType(driver_.pushBack(firType, rtArrCat));
    }
  }

  void visit(RepeatExpr &node) {
    auto elem = node.getElem();
    auto size = node.getSize();

    apply(*elem);
    apply(*size);
    auto elemType = elem->getType();
    auto sizeCt = node.getSizeCT();
    if (sizeCt.has_value()) {
      node.setCTI(IntCTI{static_cast<int>(sizeCt.value())});
    }
    node.setType(elemType);
  }

  void visit(InputExpr &node) { node.setType(intType_); }

  void visit(CompoundStmt &node) {
    scopes_.beginScope(node.getScope());
    for (auto &&stmt : node) {
      apply(*stmt);
    }
    scopes_.endScope();
  }

  void visit(VarDecl &node) {
    auto expr = node.getExpr();
    apply(*expr);
    trySetIntCTI(*expr);
    node.setType(expr->getType());
  }

  void visit(DeclStmt &node) {
    for (auto &&decl : node) {
      if (scopes_.declared(decl->getName())) {
        driver_.reportError<Redefinition>(decl->getLoc(), decl->getName());
        return;
      }
      apply(*decl);
      scopes_.declare(decl->getName(), decl);
    }
  }

  void visit(ValueStmt &node) { apply(*node.getExpr()); }

  void visit(IfStmt &node) {
    scopes_.beginScope(node.getScope());
    apply(*node.getCond());
    apply(*node.getThen());
    if (node.getElse()) {
      apply(*node.getElse());
    }
    scopes_.endScope();
  }

  void visit(WhileStmt &node) {
    scopes_.beginScope(node.getScope());
    loops_.push_back(std::addressof(node));
    apply(*node.getCondExpr());
    apply(*node.getBody());
    loops_.pop_back();
    scopes_.endScope();
  }

  void visit(BreakStmt &node) {
    auto loc = node.getLoc();
    if (loops_.empty()) {
      driver_.reportError<OutOfLoopStatement>(loc, "break");
    } else {
      node.setLoopStmt(loops_.back());
    }
  }

  void visit(ContinueStmt &node) {
    auto loc = node.getLoc();
    if (loops_.empty()) {
      driver_.reportError<OutOfLoopStatement>(loc, "continue");
    } else {
      node.setLoopStmt(loops_.back());
    }
  }

  void visit(OutputStmt &node) { apply(*node.getExpr()); }

  bool declared(std::string_view name) { return scopes_.declared(name); }
};

} // namespace paracl
