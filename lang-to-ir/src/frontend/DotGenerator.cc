#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "frontend/Nodes.hh"
#include "frontend/Types.hh"

namespace paracl {

class DotGenerator final
    : public visitor::VisitorBase<INode, DotGenerator, void> {
  std::string filepath_;
  std::ofstream os_;

  std::string typeAsString(Type *type) {
    if (type == nullptr)
      return "unknown";
    return type->getAsString();
  }

  void printLink(INode &parent, INode &child) {
    os_ << "\tnode_" << std::addressof(parent) << " -> node_"
        << std::addressof(child) << ";\n";
  }

  void printDecl(Decl &node, std::string_view name) {
    const auto &loc = node.getLoc();
    auto type = typeAsString(node.getType());
    os_ << "\tnode_" << std::addressof(node);
    os_ << "[fillcolor=green, label = \"";
    os_ << loc << "|" << name << "|" << type << "|";
    os_ << node.getName() << "\"];\n";
  }

  void printStmt(Stmt &node, std::string_view name) {
    const auto &loc = node.getLoc();
    os_ << "\tnode_" << std::addressof(node);
    os_ << "[fillcolor=purple, label = \"";
    os_ << loc << "|" << name << "\"];\n";
  }

  void printExpr(Expr &node, std::string_view name,
                 std::string_view label = "") {
    const auto &loc = node.getLoc();
    auto type = typeAsString(node.getType());
    os_ << "\tnode_" << std::addressof(node);
    os_ << "[fillcolor=orange, label = \"";
    os_ << loc << "|" << name << "|" << type;
    if (label != "")
      os_ << "|" << label;
    os_ << "\"];\n";
  }

public:
  VISIT(AllNodes);

  DotGenerator(std::string_view filepath)
      : filepath_(filepath), os_(filepath_) {}

  void generate(TranslationUnit *root) {
    assert(root);
    os_ << "digraph {\n";
    os_ << "\tnode[shape=record, style=filled, fontcolor=black];\n";
    apply(*root);
    os_ << "}" << std::endl;
    os_.flush();
  }

  void visit(UnaryOperator &node) {
    auto expr = node.getSubExpr();
    assert(expr);
    printExpr(node, "UnaryOperator", toString(node.getOpcode()));
    printLink(node, *expr);
    apply(*expr);
  }

  void visit(BinaryOperator &node) {
    auto lhs = node.getLHS();
    assert(lhs);
    auto rhs = node.getRHS();
    assert(rhs);
    auto label = "\\" + toString(node.getOpcode());
    printExpr(node, "BinaryOperator", label);
    printLink(node, *lhs);
    printLink(node, *rhs);
    apply(*lhs);
    apply(*rhs);
  }

  void visit(ConditionalOperator &node) {
    auto cond = node.getCondExpr();
    assert(cond);
    auto trueExpr = node.getTrueExpr();
    assert(trueExpr);
    auto falseExpr = node.getFalseExpr();
    assert(falseExpr);
    printExpr(node, "ConditionalOperator");
    printLink(node, *cond);
    printLink(node, *trueExpr);
    printLink(node, *falseExpr);
    apply(*cond);
    apply(*trueExpr);
    apply(*falseExpr);
  }

  void visit(IntegerLiteral &node) {
    auto val = node.getVal();
    printExpr(node, "IntegerLiteral", std::to_string(val));
  }

  void visit(UndefLiteral &node) { printExpr(node, "UndefLiteral"); }

  void visit(DeclRefExpr &node) {
    auto decl = node.getDecl();
    printExpr(node, "DeclRefExpr", node.getName());
  }

  void visit(ImplicitCastExpr &node) {
    auto expr = node.getExpr();
    assert(expr);
    auto castType = node.getImplicitCastType();
    auto label = toString(castType);
    printExpr(node, "ImplicitCastExpr", label);
    printLink(node, *expr);
    apply(*expr);
  }

  void visit(ArraySubscriptExpr &node) {
    auto lhs = node.getLHS();
    assert(lhs);
    auto rhs = node.getRHS();
    assert(rhs);
    printExpr(node, "ArraySubscriptExpr");
    printLink(node, *lhs);
    printLink(node, *rhs);
    apply(*lhs);
    apply(*rhs);
  }

  void visit(InitListExpr &node) {
    printExpr(node, "InitListExpr");
    for (auto &&expr : node) {
      assert(expr);
      printLink(node, *expr);
      apply(*expr);
    }
  }

  void visit(CallExpr &node) {
    auto call = node.getCall();
    assert(call);
    printExpr(node, "CallExpr");
    printLink(node, *call);
    apply(*call);
    for (auto &&param : node) {
      assert(param);
      printLink(node, *param);
      apply(*param);
    }
  }

  void visit(InputExpr &node) { printExpr(node, "InputExpr"); }

  void visit(VarDecl &node) {
    auto expr = node.getExpr();
    assert(expr);
    printDecl(node, "VarDecl");
    printLink(node, *expr);
    apply(*expr);
  }

  void visit(ParmVarDecl &node) { printDecl(node, "ParmVarDecl"); }

  void visit(FunctionDecl &node) {
    for (auto &&parm : node) {
      assert(parm);
      printLink(node, *parm);
      apply(*parm);
    }
    auto stmt = node.getBody();
    assert(stmt);
    printDecl(node, "FunctionDecl");
    printLink(node, *stmt);
    apply(*stmt);
  }

  void visit(ExternFunctionDecl &node) {
    for (auto &&parm : node) {
      assert(parm);
      printLink(node, *parm);
      apply(*parm);
    }
    printDecl(node, "ExternFunctionDecl");
  }

  void visit(OutputStmt &node) {
    auto expr = node.getExpr();
    assert(expr);
    printStmt(node, "OutputStmt");
    printLink(node, *expr);
    apply(*expr);
  }

  void visit(CompoundStmt &node) {
    printStmt(node, "CompoundStmt");
    for (auto &&stmt : node) {
      assert(stmt);
      printLink(node, *stmt);
      apply(*stmt);
    }
  }

  void visit(DeclStmt &node) {
    printStmt(node, "DeclStmt");
    for (auto &&decl : node) {
      assert(decl);
      printLink(node, *decl);
      apply(*decl);
    }
  }

  void visit(ValueStmt &node) {
    auto expr = node.getExpr();
    assert(expr);
    printStmt(node, "ValueStmt");
    printLink(node, *expr);
    apply(*expr);
  }

  void visit(IfStmt &node) {
    auto cond = node.getCond();
    assert(cond);
    auto then = node.getThen();
    assert(then);
    auto Else = node.getElse();
    printLink(node, *cond);
    printLink(node, *then);
    apply(*cond);
    apply(*then);
    if (Else) {
      printStmt(node, "IfElseStmt");
      printLink(node, *Else);
      apply(*Else);
      return;
    }
    printStmt(node, "IfStmt");
  }

  void visit(WhileStmt &node) {
    auto cond = node.getCond();
    assert(cond);
    auto body = node.getBody();
    assert(body);
    printStmt(node, "WhileStmt");
    printLink(node, *cond);
    printLink(node, *body);
    apply(*cond);
    apply(*body);
  }

  void visit(BreakStmt &node) { printStmt(node, "BreakStmt"); }

  void visit(ContinueStmt &node) { printStmt(node, "ContinueStmt"); }

  void visit(ReturnStmt &node) {
    auto expr = node.getExpr();
    printStmt(node, "ReturnStmt");
    if (expr) {
      printLink(node, *expr);
      apply(*expr);
    }
  }

  void visit(TranslationUnit &node) {
    printStmt(node, "TranslationUnit");
    for (auto &&decl : node) {
      assert(decl);
      printLink(node, *decl);
      apply(*decl);
    }
  }
};

void generateDot(std::string_view filepath, TranslationUnit* root) {
	DotGenerator generator{filepath};
	generator.generate(root);
}

} // namespace paracl
