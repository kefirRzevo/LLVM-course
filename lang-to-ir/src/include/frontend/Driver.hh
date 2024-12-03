#pragma once

#include <memory>
#include <string_view>
#include <utility>

#include "frontend/Errors.hh"
#include "parser.tab.hh"

namespace paracl {

class DotGenerator;
class ASTBuilder;
class INode;
class TranslationUnit;
class Lexer;
class ErrorReporter;
class SemanticAnalyzer;
class TypeCreator;

class Type;
class IntType;
class VoidType;
class ArrayType;
class PointerType;
class FunctionType;

class Driver final {
  std::string_view moduleName_;
  std::unique_ptr<ASTBuilder> astBuilder_;
  std::unique_ptr<Lexer> lexer_;
  std::unique_ptr<ErrorReporter> errorReporter_;
  std::unique_ptr<TypeCreator> typeCreator_;

  std::vector<CompoundStmt *> stmts_;

public:
  Driver(std::string_view moduleName);
  Driver(const Driver &) = delete;
  Driver &operator=(const Driver &) = delete;
  Driver(Driver &&) = default;
  Driver &operator=(Driver &&) = default;
  ~Driver();

  std::string_view getModuleName() const { return moduleName_; }

  template <typename _NodeType, typename... _NodeArgs>
  _NodeType *createNode(_NodeArgs &&...args) const {
    return astBuilder_->createNode<_NodeType>(std::forward<_NodeArgs>(args)...);
  }
  TranslationUnit *getRoot() const;

  Parser::symbol_type getNextToken();

  void parse();

  bool hasErrors() const;
  template <typename ErrorType, typename... ErrorArgs>
  void reportError(ErrorArgs &&...args) {
    errorReporter_->reportError<ErrorType>(std::forward<ErrorArgs>(args)...);
  }

  void reportAllErrors(std::ostream &os) const;

  IntType *getIntType();
  VoidType *getVoidType();
  ArrayType *getArrayType(Type *elemType, size_t elemCount);
  PointerType *getPointerType(Type *elemType);
  FunctionType *getFunctionType(Type *retType,
                                std::vector<Type *> &&paramTypes);

  void pushCompoundStmt(CompoundStmt *stmt) { stmts_.push_back(stmt); }
  CompoundStmt *getCompoundStmt() const { return stmts_.back(); }
  void popCompoundStmt() { stmts_.pop_back(); }

  void generateDot(std::string_view filepath) const;

  void analyze();

  void execute();
};

} // namespace paracl
