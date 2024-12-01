#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "location.hh"

namespace paracl {

struct Error {
  std::string msg_;

  Error(const location &loc, const std::string &msg) {
    std::ostringstream oss;
    oss << loc << " " << msg;
    msg_ = oss.str();
  }

  void print(std::ostream &os) const { os << msg_ << std::endl; }

  virtual ~Error() = default;
};

struct Lexical : public Error {
  Lexical(const location &loc, const std::string &msg)
      : Error(loc, "lexical error, " + msg) {}
};

struct UnknownToken final : public Lexical {
  UnknownToken(const location &loc, const std::string &token)
      : Lexical(loc, "unknown type name '" + std::string{token} + "'") {}
};

struct UnterminatedComment final : public Lexical {
  UnterminatedComment(const location &loc)
      : Lexical(loc, "unterminated /* comment") {}
};

struct Syntax final : public Error {
  Syntax(const location &loc, const std::string &msg) : Error(loc, msg) {}
};

struct Semantic : public Error {
  Semantic(const location &loc, const std::string &msg)
      : Error(loc, "semantic error, " + msg) {}
};

struct UnassignableExpression final : public Semantic {
  UnassignableExpression(const location &loc)
      : Semantic(loc, "expression is not assignable") {}
};

struct UndeclaredIdentifier final : public Semantic {
  UndeclaredIdentifier(const location &loc, const std::string &ident)
      : Semantic(loc, "use of undeclared identifier '" + ident + "'") {}
};

struct MismatchingTypeUnaryExpr final : public Semantic {
  MismatchingTypeUnaryExpr(const location &loc, const std::string &type)
      : Semantic(loc,
                 "invalid argument type '" + type + "' to unary expression") {}
};

struct MismatchingTypeBinaryExpr final : public Semantic {
  MismatchingTypeBinaryExpr(const location &loc, const std::string &lhsType,
                            const std::string &rhsType)
      : Semantic(loc, "invalid operands to binary expression ('" + lhsType +
                          "' and '" + rhsType + "')") {}
};

struct MismatchingTypeAssignExpr final : public Semantic {
  MismatchingTypeAssignExpr(const location &loc, const std::string &toType,
                            const std::string &fromType)
      : Semantic(loc, "assigning to '" + toType + "' from incompatible type '" +
                          fromType + "'") {}
};

struct MismatchingType final : public Semantic {
  MismatchingType(const location &loc)
      : Semantic(loc, "can not deduce generic type") {}
};

struct MismatchingFunctionTypes final : public Semantic {
  MismatchingFunctionTypes(const location &loc, const std::string &lhsType,
                           const std::string &rhsType)
      : Semantic(loc, "mismatching function types '" + lhsType + "' and '" +
                          rhsType + "'") {}
};

struct IntTypeRequired final : public Semantic {
  IntTypeRequired(const location &loc, const std::string &type)
      : Semantic(loc, "cannot convert '" + type + "' to int type") {}
};

struct SubscriptedValue final : public Semantic {
  SubscriptedValue(const location &loc)
      : Semantic(loc, "subscripted value is not an array or pointer") {}
};

struct Subscript final : public Semantic {
  Subscript(const location &loc, const std::string &type)
      : Semantic(loc, "subscript of pointer to type '" + type + "'") {}
};

struct NotAFunction final : public Semantic {
  NotAFunction(const location &loc, const std::string &type)
      : Semantic(loc, "called object type '" + type +
                          "' is not a function or function pointer") {}
};

struct FuncDefNotAllowed final : public Semantic {
  FuncDefNotAllowed(const location &loc)
      : Semantic(loc, "function definition is not allowed here") {}
};

struct OutOfLoopStatement final : public Semantic {
  OutOfLoopStatement(const location &loc, const std::string &stmt)
      : Semantic(loc,
                 "'" + stmt + "' statement not in loop or switch statement") {}
};

struct OutOfFunctionStatement final : public Semantic {
  OutOfFunctionStatement(const location &loc)
      : Semantic(loc,
                 "'return' statement not in function definition statement") {}
};

struct Redefinition final : public Semantic {
  Redefinition(const location &loc, const std::string &decl)
      : Semantic(loc, "redefinition of '" + decl + "'") {}
};

struct IncorrectReturn final : public Semantic {
  IncorrectReturn(const location &loc, const std::string &retType,
                  const std::string &type)
      : Semantic(loc, "cannot initialize return object of type ' " + retType +
                          "' with a type '" + type + "'") {}
};

class ErrorReporter final : private std::vector<std::unique_ptr<Error>> {
  using Errors = std::vector<std::unique_ptr<Error>>;
  using Errors::begin;
  using Errors::end;
  using Errors::push_back;
  using Errors::size;

public:
  bool hasErrors() const { return size(); }

  template <typename _Type, typename... _Args>
  void reportError(_Args &&...args) {
    auto uptr = std::make_unique<_Type>(std::forward<_Args>(args)...);
    push_back(std::move(uptr));
  }

  void reportAllErrors(std::ostream &os) const {
    for (auto &&error : *this)
      error->print(os);
  }
};

} // namespace paracl
