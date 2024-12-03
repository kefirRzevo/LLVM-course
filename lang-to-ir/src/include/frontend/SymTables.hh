#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Utils.hh"

namespace paracl {

template <typename Value>
class Scope final
    : std::unordered_map<std::string_view, Value, utils::StringViewHash,
                         utils::StringViewEqual> {
public:
  using Map = std::unordered_map<std::string_view, Value, utils::StringViewHash,
                                 utils::StringViewEqual>;
  using Map::begin;
  using Map::empty;
  using Map::end;
  using Map::size;

  void declare(std::string_view name, Value value) {
    Map::try_emplace(name, value);
  }

  bool declared(std::string_view name) const {
    return Map::find(name) != end();
  }

  std::optional<Value> lookup(std::string_view name) const {
    auto found = Map::find(name);
    if (found != end())
      return found->second;
    return std::nullopt;
  }
};

template <typename Value>
class ScopeStack final : std::vector<std::reference_wrapper<Scope<Value>>> {
public:
  using Stack = std::vector<std::reference_wrapper<Scope<Value>>>;
  using Stack::begin;
  using Stack::empty;
  using Stack::end;
  using Stack::rbegin;
  using Stack::rend;
  using Stack::size;

  void beginScope(Scope<Value> &scope) { Stack::push_back(scope); }

  void endScope() { Stack::pop_back(); }

  void declare(std::string_view name, Value value) {
    if (!empty()) {
      auto &back = Stack::back();
      back.get().declare(name, value);
      return;
    }
    assert(0 && "can not declare as scope is empty");
  }

  bool declared(std::string_view name) const {
    auto found = std::find_if(
        rbegin(), rend(), [&](auto &&it) { return it.get().declared(name); });
    return found != rend();
  }

  std::optional<Value> lookup(std::string_view name) const {
    auto found = std::find_if(rbegin(), rend(), [&](auto &&it) {
      return it.get().lookup(name) != std::nullopt;
    });
    if (found != rend())
      return found->get().lookup(name);
    return std::nullopt;
  }
};

} // namespace paracl
