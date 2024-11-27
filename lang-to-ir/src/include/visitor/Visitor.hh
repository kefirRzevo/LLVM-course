#pragma once

#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "ctti/type_id.hh"

namespace visitor {

using tag_type = std::uint64_t;

template <typename _VisitableBase, typename _Visitable> struct Tag {};

template <typename _VisitableBase, typename _Visitable>
inline constexpr visitor::tag_type hash() {
  using tag_type = Tag<typename std::remove_cv<_VisitableBase>::type,
                       typename std::remove_cv<_Visitable>::type>;
  return ctti::unnamed_type_id<tag_type>().hash();
}

template <typename _VisitableBase> struct VisitableBase {
  using base_type = _VisitableBase;

  template <typename _Visitable>
  static constexpr visitor::tag_type
  getTagHelper(const _Visitable * = nullptr) {
    return hash<base_type, _Visitable>();
  }

  virtual visitor::tag_type getTag() const { return getTagHelper<base_type>(); }

#define VISITABLE()                                                            \
  virtual visitor::tag_type getTag() const override {                          \
    return getTagHelper(this);                                                 \
  }

  virtual ~VisitableBase() = default;
};

template <typename _VisitableBase, typename _Visitor, typename _ReturnType>
struct VTableTraits {
  using base_type = _VisitableBase;
  using visitor_type = _Visitor;
  using return_type = _ReturnType;
  using function_type = return_type (visitor_type::*)(base_type &);
};

template <typename _Traits, typename toVisit> struct VTable {};

template <typename _Traits, typename... types>
struct VTable<_Traits, std::tuple<types...>> {
  using base_type = typename _Traits::base_type;
  using visitor_type = typename _Traits::visitor_type;
  using return_type = typename _Traits::return_type;
  using function_type = typename _Traits::function_type;

  template <typename _Visitable>
  static constexpr std::pair<visitor::tag_type, function_type> record() {
    constexpr auto tag = hash<base_type, _Visitable>();
    auto funcPtr = static_cast<function_type>(
        &visitor_type::template thunk<visitor_type, _Visitable>);
    return {tag, funcPtr};
  }

  constexpr VTable() { storage_ = {record<types>()...}; }

  constexpr function_type get(visitor::tag_type tag) const {
    auto found = storage_.find(tag);
    if (found != storage_.cend()) {
      return found->second;
    }
    auto base_tag = hash<base_type, base_type>();
    found = storage_.find(base_tag);
    if (found != storage_.end()) {
      return found->second;
    }
    throw std::runtime_error("Can't find visit handler");
  }

private:
  std::unordered_map<visitor::tag_type, function_type> storage_;
};

template <typename _VisitableBase, typename _VisitorImpl, typename _ReturnType>
struct VisitorBase {
  using vtable_traits = VTableTraits<_VisitableBase, _VisitorImpl, _ReturnType>;
  using base_type = typename vtable_traits::base_type;
  using return_type = typename vtable_traits::return_type;
  using function_type = typename vtable_traits::function_type;

  using visitor_type = _VisitorImpl;

  template <typename _Visitor, typename _Visitable>
  return_type thunk(base_type &base) {
    using constancy =
        typename std::conditional<std::is_const<_Visitor>::value,
                                  const _Visitable, _Visitable>::type;
    return static_cast<visitor_type *>(this)->visit(
        static_cast<constancy &>(base));
  }

  return_type apply(base_type &base) {
    const auto *vtable =
        static_cast<visitor_type &>(*this).template getVTable<vtable_traits>();
    function_type func = vtable->get(base.getTag());
    return (static_cast<visitor_type &>(*this).*func)(base);
  }

#define VISIT(tovisit)                                                         \
  template <typename _Traits> const auto *getVTable() const {                  \
    static visitor::VTable<_Traits, tovisit> vTable;                           \
    return std::addressof(vTable);                                             \
  }

  virtual ~VisitorBase() = default;
};

} // namespace visitor
