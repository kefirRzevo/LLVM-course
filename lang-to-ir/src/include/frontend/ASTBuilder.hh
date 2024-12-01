#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Utils.hh"

namespace paracl {

class INode;

class TranslationUnit;

class ASTBuilder final {
  std::vector<std::unique_ptr<INode>> nodes_;
  TranslationUnit *root_ = nullptr;

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

  void setRoot(TranslationUnit *root) noexcept { root_ = root; }

  TranslationUnit *getRoot() noexcept { return root_; }
};

} // namespace paracl
