#pragma once

#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace paracl {

enum class TypeKind {
  IntType,
  VoidType,
  ArrayType,
  PointerType,
  FunctionType,
};

class Type {
  TypeKind kind_;
  size_t size_;
  std::string asString_;

public:
  Type(size_t size, std::string_view asString, TypeKind kind)
      : kind_(kind), size_(size), asString_(asString) {}

  TypeKind getKind() const { return kind_; }
  size_t getSize() const { return size_; }
  const std::string &getAsString() const { return asString_; }

  virtual ~Type() = default;
};

class IntType final : public Type {
public:
  IntType() : Type(sizeof(int), "int", TypeKind::IntType) {}
};

class VoidType final : public Type {
public:
  VoidType() : Type(0, "void", TypeKind::VoidType) {}
};

class ElementedType : public Type {
  Type *elemType_;

public:
  ElementedType(size_t size, std::string_view asString, TypeKind kind,
                Type *elemType)
      : Type(size, asString, kind), elemType_(elemType) {}

  Type *getElemType() const { return elemType_; }

  static bool isElemented(TypeKind kind) {
    switch (kind) {
    case TypeKind::ArrayType:
    case TypeKind::PointerType:
      return true;
    default:
      return false;
    }
  }
};

class ArrayType final : public ElementedType {
  size_t elemCount_;

  static std::string getResAsString(Type *elemType, size_t elemCount) {
    assert(elemType);
    auto res = std::string{elemType->getAsString()};
    return res + "[" + std::to_string(elemCount) + "]";
  }

public:
  ArrayType(Type *elemType, size_t elemCount)
      : ElementedType(elemType->getSize() * elemCount,
                      getResAsString(elemType, elemCount), TypeKind::ArrayType,
                      elemType),
        elemCount_(elemCount) {}

  size_t getElemCount() const { return elemCount_; }
};

class PointerType final : public ElementedType {
  static std::string getResAsString(Type *elemType) {
    assert(elemType);
    auto res = std::string{elemType->getAsString()};
    return res + "(*)";
  }

public:
  PointerType(Type *elemType)
      : ElementedType(sizeof(void *), getResAsString(elemType),
                      TypeKind::PointerType, elemType) {}
};

class FunctionType final : public Type, std::vector<Type *> {
  using Types = std::vector<Type *>;

  Type *retType_;

  static std::string getResAsString(Type *retType, const Types &paramTypes) {
    assert(retType);
    auto ret = std::string{retType->getAsString()};
    return ret + getTypesAsString(paramTypes.begin(), paramTypes.end());
  }

public:
  using vector::begin;
  using vector::empty;
  using vector::end;
  using vector::size;

  FunctionType(Type *retType, std::vector<Type *> &&paramTypes)
      : Type(sizeof(void *), getResAsString(retType, paramTypes),
             TypeKind::FunctionType),
        vector(std::move(paramTypes)), retType_(retType) {}

  Type *getRetType() const { return retType_; }

  template <typename InputIt>
  static std::string getTypesAsString(InputIt first, InputIt last) {
    auto separator = std::string{};
    auto result = std::string{};
    std::for_each(first, last, [&](auto &&type) {
      result += separator + type->getAsString();
      separator = ", ";
    });
    return "(" + result + ")";
  }
};

class TypeCreator final
    : std::unordered_map<std::string, std::unique_ptr<Type>> {
public:
  template <typename _Type, typename... _Args> _Type *getType(_Args &&...args) {
    auto type = _Type{std::forward<_Args>(args)...};
    auto string = std::string{type.getAsString()};
    auto found = find(string);
    if (found != end()) {
      return static_cast<_Type *>(found->second.get());
    }
    auto uptr = std::make_unique<_Type>(std::move(type));
    auto ptr = uptr.get();
    emplace(string, std::move(uptr));
    return ptr;
  }
};

} // namespace paracl
