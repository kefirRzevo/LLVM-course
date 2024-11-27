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
  TupleType,
  FunctionType,
};

class Type {
  TypeKind kind_;
  size_t size_;
  std::string asString_;

public:
  Type(size_t size, std::string_view asString, TypeKind kind)
      : size_(size), asString_(asString) {}

  TypeKind getKind() const { return kind_; }
  size_t getSize() const { return size_; }
  const std::string& getAsString() const { return asString_; }

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

class ArrayType final : public Type {
  Type *elemType_;
  size_t elemCount_;

  static std::string getResAsString(Type *elemType, size_t elemCount) {
    auto res = std::string{elemType->getAsString()};
    return res + "[" + std::to_string(elemCount) + "]";
  }

public:
  ArrayType(Type *elemType, size_t elemCount)
      : Type(elemType->getSize() * elemCount, getResAsString(elemType, elemCount), TypeKind::ArrayType),
        elemType_(elemType), elemCount_(elemCount) {}

  Type *getElemType() const { return elemType_; }
  size_t getElemCount() const { return elemCount_; }
};

class TupleType final : public Type, std::vector<Type *> {
public:
  using Types = std::vector<Type *>;
  using Types::begin;
  using Types::end;
  using Types::size;
  using Types::empty;

private:
  static size_t getResSize(const Types &types) {
    return std::accumulate(types.begin(), types.end(), size_t{},
                           [](auto &&lhs, auto&& rhs) { return lhs + rhs->getSize(); });
  }

  static std::string getResAsString(const Types &types) {
    auto separator = std::string{};
    auto result = std::string{};
    std::for_each(types.begin(), types.end(), [&](auto &&type) {
      result = separator + type->getAsString();
      separator = ", ";
    });
    return result;
  }

public:
  TupleType(Types &&types)
      : Type(getResSize(types), getResAsString(types), TypeKind::TupleType), Types(std::move(types)) {}
};

class FunctionType final : public Type {
  Type *retType_;
  TupleType *paramTypes_;

  static std::string getResAsString(Type *retType, TupleType *paramTypes) {
    auto res = std::string{retType->getAsString()};
    return res + "(" + std::string{paramTypes->getAsString()} + ")";
  }

public:
  FunctionType(Type *retType, TupleType *paramTypes)
      : Type(sizeof(void *), getResAsString(retType, paramTypes), TypeKind::FunctionType) {
  }

  Type *getRetType() const { return retType_; }
  TupleType *getParamTypes() const { return paramTypes_; }
};

class TypeCreator final {
  using type_uptr = std::unique_ptr<Type>;

  std::unordered_map<std::string, type_uptr> types_;

public:
  template <typename _Type, typename... _Args> _Type *getType(_Args &&...args) {
    auto type = _Type{std::forward<_Args>(args)...};
    auto string = std::string{type.getAsString()};
    auto found = types_.find(string);
    if (found != types_.end()) {
      return static_cast<_Type*>(found->second.get());
    }
    auto uptr = std::make_unique<_Type>(std::move(type));
    auto ptr = uptr.get();
    types_.emplace(string, std::move(uptr));
    return ptr;
  }
};

} // namespace paracl
