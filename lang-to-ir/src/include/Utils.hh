#pragma once

#include <functional>
#include <string_view>
#include <tuple>

namespace utils {

template <typename... Tuples>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

template <typename T> struct reversed_adapter_val {
  T iterable_;
  auto begin() { return std::rbegin(iterable_); }
  auto end() { return std::rend(iterable_); }
  auto begin() const { return std::rbegin(iterable_); }
  auto end() const { return std::rend(iterable_); }
};

template <typename T> struct reversed_adapter_ref {
  T &iterable_;
  auto begin() { return std::rbegin(iterable_); }
  auto end() { return std::rend(iterable_); }
  auto begin() const { return std::rbegin(iterable_); }
  auto end() const { return std::rend(iterable_); }
};

template <typename T> reversed_adapter_ref<T> reverse(T &iterable) {
  return {iterable};
}

template <typename T> reversed_adapter_val<T> reverse(T &&iterable) {
  return {iterable};
}

struct StringViewHash {
  std::size_t operator()(std::string_view key) const {
    return std::hash<std::string_view>()(key);
  }
};

struct StringViewEqual {
  bool operator()(std::string_view lhs, std::string_view rhs) const {
    return lhs == rhs;
  }
};

} // namespace utils

namespace paracl {

using addr_t = size_t;

} // namespace paracl
