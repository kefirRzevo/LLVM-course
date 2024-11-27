#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
#include <exception>

#include "location.hh"

#include "Utils.hh"
#include "frontend/SymTables.hh"
#include "frontend/Operators.hh"
#include "visitor/Visitor.hh"

namespace paracl {

enum class NodeType : char {
  UnaryOperator,
  BinaryOperator,
  ConditionalOperator,
  IntegerLiteral,
  UndefLiteral,
  DeclRefExpr,
  ImplicitCastExpr,
  ArraySubscriptExpr,
  InitListExpr,
  CallExpr,

  VarDecl,
  ParmVarDecl,
  FunctionDecl,
  ExternFunctionDecl,

  OutputStmt,
  CompoundStmt,
  DeclStmt,
  ValueStmt,
  IfStmt,
  WhileStmt,
  BreakStmt,
  ContinueStmt,
  ReturnStmt,
};

class INode : public visitor::VisitableBase<INode> {
  location loc_;
  NodeType nodeType_;

public:
  VISITABLE();

  INode(const location &loc, NodeType nodeType)
      : loc_(loc), nodeType_(nodeType) {}

  const location &getLoc() const { return loc_; }

  NodeType getNodeType() const { return nodeType_; }
};

enum class ExprKind : bool {
  LVal,
  RVal,
};

class Type;

class Expr : public INode {
  Type *type_;
  ExprKind kind_;

public:
  VISITABLE();

  Expr(const location &loc, ExprKind kind, NodeType nodeType)
      : INode(loc, nodeType), type_(nullptr), kind_(kind) {}

  Type *getType() const { return type_; }

  void setType(Type *type) { type_ = type; }

  bool isLValue() const { return kind_ == ExprKind::LVal; }

  bool isRValue() const { return kind_ == ExprKind::RVal; }
};

class UnaryOperator final : public Expr {
  UnaryOpcode op_;
  Expr *expr_;

  static ExprKind getExprKind(UnaryOpcode ) {
    return ExprKind::RVal;
  }

public:
  VISITABLE();

  UnaryOperator(const location &loc, UnaryOpcode op, Expr *expr)
      : Expr(loc, getExprKind(op), NodeType::UnaryOperator), op_(op),
        expr_(expr) {}

  UnaryOpcode getOpcode() const { return op_; }

  Expr *getSubExpr() const { return expr_; }

  void setSubExpr(Expr* expr) { expr_ = expr; }
};

class BinaryOperator final : public Expr {
  BinaryOpcode op_;
  Expr *lhs_;
  Expr *rhs_;

public:
  VISITABLE();

  BinaryOperator(const location &loc, BinaryOpcode op, Expr *lhs, Expr *rhs)
      : Expr(loc, ExprKind::RVal, NodeType::BinaryOperator), op_(op), lhs_(lhs),
        rhs_(rhs) {}

  BinaryOpcode getOpcode() const { return op_; }

  Expr *getLHS() const { return lhs_; }

  void setLHS(Expr* expr) { lhs_ = expr; }

  Expr* getRHS() const { return rhs_; }

  void setRHS(Expr* expr) { rhs_ = expr; }
};

class ConditionalOperator final : public Expr {
  Expr *cond_;
  Expr *trueExpr_;
  Expr *falseExpr_;

public:
  VISITABLE();

  ConditionalOperator(const location &loc, Expr *cond, Expr *trueExpr,
                      Expr *falseExpr)
      : Expr(loc, ExprKind::RVal, NodeType::ConditionalOperator), cond_(cond),
        trueExpr_(trueExpr), falseExpr_(falseExpr) {}

  Expr *getCondExpr() const { return cond_; }

  void setCondExpr(Expr* expr) { cond_ = expr; }

  Expr *getTrueExpr() const { return trueExpr_; }

  void setTrueExpr(Expr* expr) { trueExpr_ = expr; }

  Expr *getFalseExpr() const { return falseExpr_; }

  void setFalseExpr(Expr* expr) { falseExpr_ = expr; }
};

class IntegerLiteral final : public Expr {
  int value_;

public:
  VISITABLE();

  IntegerLiteral(const location &loc, int value)
      : Expr(loc, ExprKind::RVal, NodeType::IntegerLiteral), value_(value) {}

  int getVal() const { return value_; }

  void setVal(int value) { value_ = value; }
};

class UndefLiteral final : public Expr {
public:
  VISITABLE();

  UndefLiteral(const location &loc)
      : Expr(loc, ExprKind::RVal, NodeType::UndefLiteral) {}
};

class Decl;

class DeclRefExpr final : public Expr {
  std::string name_;
  Decl *decl_;

public:
  VISITABLE();

  DeclRefExpr(const location &loc, std::string_view name)
      : Expr(loc, ExprKind::LVal, NodeType::DeclRefExpr), name_(name), decl_(nullptr) {}

  Decl *getDecl() const { return decl_; }

  void setDecl(Decl* decl) { decl_ = decl; }
};

enum class ImplicitCastType {
  LValueToRValue,
  ArrayToPointerDecay,
  FunctionToPointerDecay,
};

inline std::string toString(ImplicitCastType type) {
  switch (type) {
    case ImplicitCastType::LValueToRValue: return "LValueToRValue";
    case ImplicitCastType::ArrayToPointerDecay: return "ArrayToPointerDecay";
    case ImplicitCastType::FunctionToPointerDecay: return "FunctionToPointerDecay";
    default: throw std::runtime_error("Unknown implicit cast type");
  }
};

class ImplicitCastExpr final : public Expr {
  Expr *expr_;
  ImplicitCastType castType_;

  static ExprKind getExprKind(ImplicitCastType castType) {
    switch (castType) {
    case ImplicitCastType::LValueToRValue:
      return ExprKind::RVal;
    case ImplicitCastType::ArrayToPointerDecay:
      return ExprKind::LVal;
    case ImplicitCastType::FunctionToPointerDecay:
      return ExprKind::LVal;
    default:
      throw std::runtime_error("Unreachable");
    }
  }

public:
  VISITABLE();

  ImplicitCastExpr(const location &loc, Expr *expr, ImplicitCastType castType)
      : Expr(loc, getExprKind(castType), NodeType::ImplicitCastExpr),
        expr_(expr), castType_(castType) {}

  Expr *getExpr() const { return expr_; }

  void setExpr(Expr * expr) { expr_ = expr; }

  ImplicitCastType getImplicitCastType() const { return castType_; }

  void setImplicitCastType(ImplicitCastType castType) { castType_ = castType; }
};

class ArraySubscriptExpr final : public Expr {
  Expr *lhs_;
  Expr *rhs_;

public:
  VISITABLE();

  ArraySubscriptExpr(const location &loc, Expr *lhs, Expr *rhs)
      : Expr(loc, ExprKind::LVal, NodeType::ArraySubscriptExpr), lhs_(lhs),
        rhs_(rhs) {}

  Expr *getLHS() const { return lhs_; }

  void setLHS(Expr* expr) { lhs_ = expr; }

  Expr *getRHS() const { return rhs_; }

  void setRHS(Expr* expr) { rhs_ = expr; }
};

class InitListExpr final : public Expr, std::vector<Expr *> {
public:
  using vector::begin;
  using vector::empty;
  using vector::end;
  using vector::size;

  VISITABLE();

  InitListExpr(const location &loc)
      : Expr(loc, ExprKind::RVal, NodeType::InitListExpr) {}

  void addExpr(Expr *expr) { vector::push_back(expr); }
};

class CallExpr final : public Expr, std::vector<Expr *> {
  Expr *call_;

public:
  using vector::begin;
  using vector::empty;
  using vector::end;
  using vector::size;

  VISITABLE();

  CallExpr(const location &loc, Expr *call)
      : Expr(loc, ExprKind::RVal, NodeType::CallExpr), call_(call) {}

  void addParam(Expr *param) { vector::push_back(param); }

  Expr *getCall() const { return call_; }

  void setFunc(Expr *call) { call_ = call; }
};

class Type;

class Decl : public INode {
protected:
  std::string name_;
  Type *type_;

public:
  VISITABLE();

  Decl(const location &loc, std::string_view name, NodeType nodeType)
      : INode(loc, nodeType), name_(name), type_(nullptr) {}

  Type *getType() const { return type_; }

  void setType(Type *type) { type_ = type; }

  std::string_view getName() const { return name_; }
};

class VarDecl final : public Decl {
  Expr *expr_;

public:
  VISITABLE();

  VarDecl(const location &loc, std::string_view name)
      : Decl(loc, name, NodeType::VarDecl) {}

  Expr *getExpr() const { return expr_; }

  void setExpr(Expr *expr) { expr_ = expr; }
};

class ParmVarDecl final : public Decl {
public:
  VISITABLE();

  ParmVarDecl(const location &loc, std::string_view name)
      : Decl(loc, name, NodeType::ParmVarDecl) {}
};

class Function : public Decl, std::vector<ParmVarDecl *> {
public:
  using vector::begin;
  using vector::empty;
  using vector::end;
  using vector::size;

  VISITABLE();

  Function(const location &loc, std::string_view name, NodeType nodeType)
      : Decl(loc, name, nodeType) {}

  void addParam(ParmVarDecl *param) { vector::push_back(param); }
};

class CompoundStmt;

class FunctionDecl final : public Function {
  CompoundStmt *body_;

public:
  VISITABLE();

  FunctionDecl(const location &loc, std::string_view name)
      : Function(loc, name, NodeType::FunctionDecl), body_(nullptr) {}

  CompoundStmt *getBody() const { return body_; }

  void setBody(CompoundStmt *body) { body_ = body; }
};

class ExternFunctionDecl final : public Function {
  void *ptr_;

public:
  VISITABLE();

  ExternFunctionDecl(const location &loc, std::string_view name)
      : Function(loc, name_, NodeType::FunctionDecl), ptr_(nullptr) {}

  void *getPtr() const { return ptr_; }

  void setPtr(void *ptr) { ptr_ = ptr; }
};

class Stmt : public INode {
public:
  VISITABLE();

  Stmt(const location &loc, NodeType nodeType) : INode(loc, nodeType) {}
};

class OutputStmt : public Stmt {
  Expr* expr_;

public:
  VISITABLE();

  OutputStmt(const location &loc, Expr* expr) : Stmt(loc, NodeType::OutputStmt), expr_(expr) {}

  Expr *getExpr() const { return expr_; }

  void setExpr(Expr* expr) { expr_ = expr; }
};

class CompoundStmt final : public Stmt, std::vector<Stmt *> {
  Scope scope_;

public:
  using Stmts = std::vector<Stmt *>;
  using Stmts::begin;
  using Stmts::empty;
  using Stmts::end;
  using Stmts::size;

  VISITABLE();

  CompoundStmt(const location &loc) : Stmt(loc, NodeType::CompoundStmt) {}

  Scope &getScope() { return scope_; }

  void addStmt(Stmt *stmt) { Stmts::push_back(stmt); }
};

class DeclStmt final : public Stmt, std::vector<Decl *> {
public:
  using Decls = std::vector<Decl *>;
  using Decls::begin;
  using Decls::empty;
  using Decls::end;
  using Decls::size;

  VISITABLE();

  DeclStmt(const location &loc) : Stmt(loc, NodeType::DeclStmt) {}

  void addDecl(Decl *decl) { Decls::push_back(decl); }
};

class ValueStmt final : public Stmt {
  Expr *expr_;

public:
  VISITABLE();

  ValueStmt(const location &loc, Expr *expr)
      : Stmt(loc, NodeType::ValueStmt), expr_(expr) {}

  Expr *getExpr() const { return expr_; }

  void setExpr(Expr* expr) { expr_ = expr; }
};

class IfStmt final : public Stmt {
  Expr *cond_;
  Stmt *then_;
  Stmt *else_;

public:
  VISITABLE();

  IfStmt(const location &loc, Expr *cond, Stmt *then, Stmt *Else = nullptr)
      : Stmt(loc, NodeType::IfStmt), cond_(cond), then_(then), else_(Else) {}

  Expr *getCond() const { return cond_; }

  void setCond(Expr* cond) { cond_ = cond; }

  Stmt *getThen() const { return then_; }

  Stmt *getElse() const { return else_; }
};

class WhileStmt final : public Stmt {
  Expr *cond_;
  Stmt *body_;

public:
  VISITABLE();

  WhileStmt(const location &loc, Expr *cond, Stmt *body)
      : Stmt(loc, NodeType::WhileStmt), cond_(cond), body_(body) {}

  Expr *getCond() const { return cond_; }

  void setCond(Expr* cond) { cond_ = cond; }

  Stmt *getBody() const { return body_; }
};

class BreakStmt final : public Stmt {
  WhileStmt *loop_ = nullptr;

public:
  VISITABLE();

  BreakStmt(const location &loc) : Stmt(loc, NodeType::BreakStmt) {}

  WhileStmt *getLoopStmt() const { return loop_; }

  void setLoopStmt(WhileStmt *stat) { loop_ = stat; }
};

class ContinueStmt final : public Stmt {
  WhileStmt *loop_ = nullptr;

public:
  VISITABLE();

  ContinueStmt(const location &loc) : Stmt(loc, NodeType::ContinueStmt) {}

  WhileStmt *getLoopStmt() const { return loop_; }

  void setLoopStmt(WhileStmt *stat) { loop_ = stat; }
};

class ReturnStmt final : public Stmt {
  Expr *expr_;

public:
  VISITABLE();

  ReturnStmt(const location &loc, Expr *expr)
      : Stmt(loc, NodeType::ReturnStmt), expr_(expr) {}

  Expr *getExpr() const { return expr_; }

  void setExpr(Expr* expr) { expr_ = expr; }
};

using Exprs =
    std::tuple<UnaryOperator, BinaryOperator, ConditionalOperator,
               IntegerLiteral, UndefLiteral, DeclRefExpr, ImplicitCastExpr,
               ArraySubscriptExpr, InitListExpr, CallExpr>;

using Decls =
    std::tuple<VarDecl, ParmVarDecl, FunctionDecl, ExternFunctionDecl>;

using Stmts = std::tuple<OutputStmt, CompoundStmt, DeclStmt, ValueStmt, IfStmt, WhileStmt,
                         BreakStmt, ContinueStmt, ReturnStmt>;

using AllNodes = utils::tuple_cat_t<Exprs, Decls, Stmts>;

} // namespace paracl
