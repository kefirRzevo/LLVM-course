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
class SearchTable
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
    if (found != end()) {
      return found->second;
    }
    return std::nullopt;
  }
};

class Decl;

class Scope final : SearchTable<Decl *> {
public:
  using VarsMap = SearchTable<Decl *>;
  using VarsMap::begin;
  using VarsMap::declare;
  using VarsMap::declared;
  using VarsMap::empty;
  using VarsMap::end;
  using VarsMap::lookup;
  using VarsMap::size;
};

class ScopeStack final : std::vector<std::reference_wrapper<Scope>> {
public:
  using Stack = std::vector<std::reference_wrapper<Scope>>;
  using Stack::begin;
  using Stack::empty;
  using Stack::end;
  using Stack::rbegin;
  using Stack::rend;
  using Stack::size;

  void beginScope(Scope &scope) { Stack::push_back(scope); }

  void endScope() { Stack::pop_back(); }

  void declare(std::string_view name, Decl *decl) {
    if (!empty()) {
      auto &back = Stack::back();
      back.get().declare(name, decl);
    }
  }

  bool declared(std::string_view name) const {
    auto found = std::find_if(
        rbegin(), rend(), [&](auto &&it) { return it.get().declared(name); });
    return found != rend();
  }

  std::optional<Decl *> lookup(std::string_view name) const {
    auto found = std::find_if(rbegin(), rend(), [&](auto &&it) {
      return it.get().lookup(name) != std::nullopt;
    });
    return found->get().lookup(name);
  }
};

// class Function;

// class FunctionTable final : SearchTable<Function *> {
// public:
//   using FunctionsMap = SearchTable<Function *>;
//   using FunctionsMap::begin;
//   using FunctionsMap::declare;
//   using FunctionsMap::declared;
//   using FunctionsMap::empty;
//   using FunctionsMap::end;
//   using FunctionsMap::lookup;
//   using FunctionsMap::size;
// };

} // namespace paracl
