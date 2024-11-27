#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Utils.hh"
#include "frontend/SymTables.hh"

namespace paracl {

class INode;

class ASTBuilder final {
  std::vector<std::unique_ptr<INode>> nodes_;
  ScopeStack scopes_;
  //FunctionTable funcs_;
  INode *root_ = nullptr;

public:
  ASTBuilder() = default;
  ASTBuilder(const ASTBuilder &rhs) = delete;
  ASTBuilder &operator=(const ASTBuilder &rhs) = delete;
  ASTBuilder(ASTBuilder &&) = default;
  ASTBuilder &operator=(ASTBuilder &&) = default;
  ~ASTBuilder() = default;

  template <typename _Type, typename... _Args>
  _Type *createNode(_Args &&...args) {
    auto uptr = std::make_unique<_Type>(std::forward<_Args>(args)...);
    auto ptr = uptr.get();
    nodes_.push_back(std::move(uptr));
    return ptr;
  }

  void setRoot(INode *root) noexcept { root_ = root; }

  INode *getRoot() noexcept { return root_; }

  void beginScope(Scope &scope) {
    scopes_.beginScope(scope);
  }

  void endScope() {
    scopes_.endScope();
  }

  void declare(std::string_view name, Decl *decl) {
    scopes_.declare(name, decl);
  }

  bool declared(std::string_view name) {
    return scopes_.declared(name);
  }

  std::optional<Decl *> lookup(std::string_view name) {
    return scopes_.lookup(name);
  }

  // void declareFunction(std::string_view name, Function *function) {
  //   return funcs_.declare(name, function);
  // }

  // bool declaredFunction(std::string_view name) {
  //   return funcs_.declared(name);
  // }

  // std::optional<Function *> lookupFunction(std::string_view name) {
  //   return funcs_.lookup(name);
  // }
};

} // namespace paracl
