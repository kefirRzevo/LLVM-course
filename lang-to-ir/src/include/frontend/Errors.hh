#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "location.hh"

namespace paracl {

struct Error {
  const location &loc_;
  std::string_view errorType_;
  std::string msg_;

  Error(const location &loc, std::string_view errorType, std::string_view msg)
      : loc_(loc), errorType_(errorType), msg_(msg) {}

  void print(std::ostream &os) const {
    os  << "" << errorType_ << ": " << msg_ << std::endl;
  }

  virtual ~Error() = default;
};

struct Lexical : public Error {
  Lexical(const location &loc, std::string_view msg)
      : Error(loc, "lexical error", msg) {}
};

struct UnknownToken final : public Lexical {
  UnknownToken(const location &loc, std::string_view token)
      : Lexical(loc, "unknown type name '" + std::string{token} + "'") {}
};

struct UnterminatedComment final : public Lexical {
  UnterminatedComment(const location &loc)
      : Lexical(loc, "unterminated /* comment") {}
};

struct Syntax final : public Error {
  Syntax(const location &loc, std::string_view msg)
      : Error(loc, "syntax error", msg) {}
};

struct Semantic : public Error {
  Semantic(const location &loc, std::string_view msg)
      : Error(loc, "semantic error", msg) {}
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
  MismatchingTypeAssignExpr(const location &loc, const std::string &lhsType,
                            const std::string &rhsType)
      : Semantic(loc, "assigning to '" + lhsType +
                          "' from incompatible type '" + rhsType + "'") {}
};

struct MismatchingType final : public Semantic {
  MismatchingType(const location &loc)
      : Semantic(loc, "can not deduce generic type") {}
};

struct IntTypeRequired final : public Semantic {
  IntTypeRequired(const location &loc, const std::string &type)
      : Semantic(loc, "cannot convert '" + type + "' to int type") {}
};

struct SubscriptedValue final : public Semantic {
  SubscriptedValue(const location &loc)
      : Semantic(loc, "subscripted value is not an array") {}
};

struct ArrayBounds final : public Semantic {
  ArrayBounds(const location &loc, int indx, int size)
      : Semantic(loc, "array index '" + std::to_string(indx) +
                          "' is past the end of the array of size '" +
                          std::to_string(size) + "'") {}
};

struct OutOfLoopStatement final : public Semantic {
  OutOfLoopStatement(const location &loc, const std::string &stmt)
      : Semantic(loc,
                 "'" + stmt + "' statement not in loop or switch statement") {}
};

struct Redefinition final : public Semantic {
  Redefinition(const location &loc, const std::string &decl)
      : Semantic(loc, "redefinition of '" + decl + "'") {}
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
    for (auto &&error : *this) {
      error->print(os);
    }
  }
};

} // namespace paracl
