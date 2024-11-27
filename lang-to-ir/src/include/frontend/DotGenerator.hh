#pragma once

#include <string>
#include <fstream>
#include <string_view>
#include <vector>

#include "frontend/Nodes.hh"
#include "frontend/Types.hh"

namespace paracl {

class DotGenerator final
		: public visitor::VisitorBase<INode, DotGenerator, void> {
	std::string filepath_;
	std::ofstream os_;

	std::string typeAsString(Type* type) {
		if (type == nullptr)
			return "unknown";
		return type->getAsString();
	}

	void printLink(INode &parent, INode &child) {
		os_ << "\tnode_" << std::addressof(parent) << " -> node_"
				<< std::addressof(child) << ";\n";
	}

	void printDecl(Decl &node, std::string_view name) {
		std::cout << "|" << node.getName() << "|" << std::endl;
		auto loc = node.getLoc();
		auto type = typeAsString(node.getType());
		os_ << "\tnode_" << std::addressof(node);
		os_ << "[fillcolor=green, label = \"";
		os_ << loc << "|" << name << "|" << type << "|";
		os_ << node.getName() << "\"];\n";
	}

	void printStmt(Stmt &node, std::string_view name) {
		auto loc = node.getLoc();
		os_ << "\tnode_" << std::addressof(node);
		os_ << "[fillcolor=purple, label = \"";
		os_ << loc << "|" << name << "\"];\n";
	}

	void printExpr(Expr &node, std::string_view name, std::string_view label = "") {
		auto loc = node.getLoc();
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

	void generate(INode *root) {
		assert(root);
		os_ << "digraph {\n";
		os_ << "\tnode[shape=record, style=filled, fontcolor=black];\n";
		if (root) {
			apply(*root);
		}
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
		auto rhs = node.getRHS();
		assert(lhs && rhs);
		auto label = "\\" + toString(node.getOpcode());
		printExpr(node, "BinaryOperator", label);
		printLink(node, *lhs);
		printLink(node, *rhs);
		apply(*lhs);
		apply(*rhs);
	}

	void visit(ConditionalOperator &node) {
		auto cond = node.getCondExpr();
		auto trueExpr = node.getTrueExpr();
		auto falseExpr = node.getFalseExpr();
		assert(cond && trueExpr && falseExpr);
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

	void visit(UndefLiteral &node) {
		printExpr(node, "UndefLiteral");
	}

	void visit(DeclRefExpr &node) {
		auto decl = node.getDecl();
		//assert(decl);
		printExpr(node, "DeclRefExpr", "undef");
	}

	void visit(ImplicitCastExpr &node) {
		auto expr = node.getExpr();
		auto label = toString(node.getImplicitCastType());
		assert(expr);
		printExpr(node, "ImplicitCastExpr", label);
		printLink(node, *expr);
		apply(*expr);
	}

	void visit(ArraySubscriptExpr &node) {
		auto lhs = node.getLHS();
		auto rhs = node.getRHS();
		assert(lhs && rhs);
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

	void visit(CallExpr& node) {
		auto call = node.getCall();
		assert(call);
		printExpr(node, "CallExpr");
		printLink(node, *call);
		apply(*call);
		for (auto && parm: node) {
			assert(parm);
			printLink(node, *parm);
			apply(*parm);
		}
	}

	void visit(VarDecl &node) {
		auto expr = node.getExpr();
		assert(expr);
		printDecl(node, "VarDecl");
		printLink(node, *expr);
		apply(*expr);
	}

	void visit(ParmVarDecl &node) {
		printDecl(node, "ParmVarDecl");
	}

	void visit(FunctionDecl &node) {
		for (auto &&parm: node) {
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
		auto then = node.getThen();
		auto Else = node.getElse();
		assert(cond && then && Else);
		printLink(node, *cond);
		printLink(node, *then);
		apply(*cond);
		apply(*then);
		if (Else) {
			printStmt(node, "IfElseStmt");
			printLink(node, *Else);
			apply(*Else);
		} else {
			printStmt(node, "IfStmt");
		}
	}

	void visit(WhileStmt &node) {
		auto cond = node.getCond();
		auto body = node.getBody();
		assert(cond && body);
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
		assert(expr);
		printStmt(node, "ReturnStmt");
		printLink(node, *expr);
		apply(*expr);
	}
};

} // namespace paracl
