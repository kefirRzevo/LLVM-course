#pragma once

#include <memory>
#include <string_view>
#include <utility>

#include "parser.tab.hh"
#include "frontend/Errors.hh"

namespace paracl {

class DotGenerator;
class ASTBuilder;
class INode;
class CompoundStmt;
class Lexer;
class ErrorReporter;
//class SemanticAnalyzer;
class TypeCreator;

class Type;
class IntType;
class VoidType;
class ArrayType;
class TupleType;
class FunctionType;

class Driver final {
  std::unique_ptr<ASTBuilder> astBuilder_;
  std::unique_ptr<Lexer> lexer_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<ErrorReporter> errorReporter_;
  std::unique_ptr<TypeCreator> typeCreator_;
  std::vector<CompoundStmt* > stmts_;
  //std::unique_ptr<SemanticAnalyzer> semanticAnalyzer_;

public:
  Driver();
  Driver(const Driver&) = delete;
  Driver& operator=(const Driver&) = delete;
  Driver(Driver&&) = default;
  Driver& operator=(Driver&&) = default;
  ~Driver();

  void setFilepath(std::string_view filepath);

  template <typename _NodeType, typename... _NodeArgs>
  _NodeType *createNode(_NodeArgs &&...args) const {
    return astBuilder_->createNode<_NodeType>(std::forward<_NodeArgs>(args)...);
  }
  void setRoot(INode *root) const;
  INode *getRoot() const;
  void beginScope(Scope &scope) const;
  void endScope() const;

  void declare(std::string_view name, Decl *var) const;
  bool declared(std::string_view name) const;
  std::optional<Decl *> lookup(std::string_view name);

  Parser::symbol_type getNextToken();

  int parse();

  bool hasErrors() const;
  template <typename ErrorType, typename... ErrorArgs>
  void reportError(ErrorArgs &&...args) {
    errorReporter_->reportError<ErrorType>(std::forward<ErrorArgs>(args)...);
  }

  void reportAllErrors(std::ostream &os) const;

  IntType* createIntType();
  VoidType* createVoidType();
  ArrayType* createArrayType(Type *elemType, size_t elemCount);
  TupleType* createTupleType(std::vector<Type*> &&types);
  FunctionType* createFunctionType(Type *retType, TupleType *paramTypes);

  void pushCompoundStmt(CompoundStmt* stmt) { stmts_.push_back(stmt); }
  CompoundStmt* getCompoundStmt() const { return stmts_.back(); }
  void popCompoundStmt() { stmts_.pop_back(); }

  void generateDot(std::string_view filepath) const;

  //void analyze(INode *node) const;
};

} // namespace paracl
