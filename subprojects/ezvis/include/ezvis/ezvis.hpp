// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include <ctti/type_id.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace ezvis {

namespace detail {

using unique_tag_type = uint64_t;

// This is a dummy type to get a unique type name
template <typename t_base, typename t_visitable> struct dummy_tag {};
template <typename t_base, typename t_visitable>
constexpr unique_tag_type unique_tag() {
  using dummy_type =
      dummy_tag<std::remove_cv_t<t_base>, std::remove_cv_t<t_visitable>>;
  return ctti::unnamed_type_id<dummy_type>().hash();
}

}; // namespace detail

template <typename t_base> struct visitable_base {
  using base_type = t_base;

  template <typename t_visitable>
  static constexpr detail::unique_tag_type unique_tag_helper_ezvis__(
      const t_visitable * =
          nullptr /* Dummy parameter for template deduction */) {
    return detail::unique_tag<base_type, t_visitable>();
  }

  virtual ezvis::detail::unique_tag_type unique_tag_ezvis__() const {
    return unique_tag_helper_ezvis__<t_base>();
  }

#define EZVIS_VISITABLE()                                                      \
  virtual ezvis::detail::unique_tag_type unique_tag_ezvis__() const override { \
    return unique_tag_helper_ezvis__(this);                                    \
  }
};

namespace detail {

template <typename T, typename... Ts>
struct are_unique : std::conjunction<std::negation<std::is_same<T, Ts>>...,
                                     are_unique<Ts...>> {};
template <typename T> struct are_unique<T> : std::true_type {};

template <typename t_tuple> struct are_tuple_elements_unique {};
template <typename... t_elems>
struct are_tuple_elements_unique<std::tuple<t_elems...>>
    : public are_unique<t_elems...> {};

// clang-format off
template <typename T, bool t_add> struct cond_add_const;
template <typename T> struct cond_add_const<T, true> : std::add_const<T> {};
template <typename T> struct cond_add_const<T, false> { using type = T; };
template <typename T, bool t_add> using cond_add_const_t = typename cond_add_const<T, t_add>::type;
// clang-format on

template <typename t_tovisit, bool t_add> struct cond_add_const_tuple {};
template <bool t_add, typename... Ts>
struct cond_add_const_tuple<std::tuple<Ts...>, t_add> {
  using type = std::tuple<cond_add_const_t<Ts, t_add>...>;
};

template <typename t_key, typename t_value, std::size_t t_size> class ct_map {
public:
  using value_type = std::pair<t_key, t_value>;
  static constexpr auto c_size = t_size;

private:
  std::array<value_type, t_size> m_map;

public:
  template <typename... t_types> constexpr ct_map(t_types &&...vals) {
    m_map = {std::forward<t_types>(vals)...};
  }

  constexpr auto find(t_key key) const {
    return std::find_if(m_map.begin(), m_map.end(),
                        [key](auto elem) { return elem.first == key; });
  }

  constexpr std::size_t size() const { return c_size; }
  constexpr auto begin() const { return m_map.begin(); }
  constexpr auto end() const { return m_map.end(); }
};

} // namespace detail

struct compile_time_vtable;
struct runtime_vtable;

namespace detail {

template <typename t_base, typename t_to_visit>
struct are_hash_values_unique {};
template <typename t_base, typename... t_types>
struct are_hash_values_unique<t_base, std::tuple<t_types...>>
    : public are_unique<std::integral_constant<
          detail::unique_tag_type, detail::unique_tag<t_base, t_types>()>...> {
};

template <typename t_base, typename t_to_visit>
constexpr auto are_hash_values_unique_v =
    are_hash_values_unique<t_base, t_to_visit>::value;

template <typename t_base, typename t_to_visit>
concept unique_hash_values = are_hash_values_unique_v<t_base, t_to_visit>;

template <typename t_base, typename t_visitor, typename t_return_type>
struct vtable_traits {
  using base_type = t_base;          // Base type in the hierarchy
  using visitor_type = t_visitor;    // Visitor base class
  using return_type = t_return_type; // Type that visit method returns
  using function_type = t_return_type (visitor_type::*)(
      base_type &); // Pointer to member function type
};

template <typename t_traits, std::size_t t_size, typename t_storage_flag>
struct vtable_storage {};

template <typename t_traits, std::size_t t_size>
struct vtable_storage<t_traits, t_size, compile_time_vtable> {
  using type = detail::ct_map<detail::unique_tag_type,
                              typename t_traits::function_type, t_size>;
};

template <typename t_traits, std::size_t t_size>
struct vtable_storage<t_traits, t_size, runtime_vtable> {
  using type = std::unordered_map<detail::unique_tag_type,
                                  typename t_traits::function_type>;
};

template <typename t_traits, std::size_t t_size, typename t_storage_flag>
using vtable_storage_t =
    typename vtable_storage<t_traits, t_size, t_storage_flag>::type;

template <typename t_traits, typename t_storage_flag, typename t_to_visit>
struct vtable {};

template <typename t_traits, typename t_storage_flag, typename... t_types>
#ifdef EZVIS_VERIFY
  requires unique_hash_values<typename t_traits::base_type,
                              std::tuple<t_types...>>
#endif
// It is very unlikely that there will be collisions with a 64-bit hash, but we
// should verify it nontheless.
struct vtable<t_traits, t_storage_flag, std::tuple<t_types...>> {
  using base_type = typename t_traits::base_type;
  using visitor_type = typename t_traits::visitor_type;
  using return_type = typename t_traits::return_type;
  using function_type = typename t_traits::function_type;

private:
  using storage_type =
      vtable_storage_t<t_traits, sizeof...(t_types), t_storage_flag>;
  storage_type m_table;

  template <typename t_visitable>
  static constexpr std::pair<detail::unique_tag_type, function_type>
  make_id_func_pair() {
    constexpr auto tag = detail::unique_tag<base_type, t_visitable>();
    // Static cast to avoid UB. See
    // https://en.cppreference.com/w/cpp/language/pointer
    constexpr auto ptr = static_cast<function_type>(
        &visitor_type::template thunk_ezvis__<
            visitor_type, t_visitable, typename visitor_type::invoker_ezvis__>);
    return {tag, ptr};
  }

public:
  constexpr vtable() { m_table = {make_id_func_pair<t_types>()...}; }

  constexpr function_type get(detail::unique_tag_type tag) const {
    auto found_handler = m_table.find(tag);
    if (found_handler != m_table.end())
      return found_handler->second;
    auto found_base = m_table.find(detail::unique_tag<base_type, base_type>());
    if (found_base != m_table.end())
      return found_base->second;
    throw std::runtime_error{"Unimplemented fallback visit"};
  }
};

} // namespace detail

template <typename t_base, typename t_concrete_visitor, typename t_return_type>
class visitor_base {
private:
  using vtable_traits_type =
      detail::vtable_traits<t_base, t_concrete_visitor, t_return_type>;

  using base_type = t_base;
  using return_type = t_return_type;
  using function_type = typename vtable_traits_type::function_type;

public:
  template <typename t_visitor, typename t_visitable, typename t_invoker>
  t_return_type thunk_ezvis__(t_base &base) {
    using const_valid_visitable =
        detail::cond_add_const_t<t_visitable, std::is_const_v<base_type>>;

    constexpr bool has_invoke =
        requires(t_visitor &visitor, const_valid_visitable &visitable) {
          {
            t_invoker::template invoke<t_return_type>(visitor, visitable)
          } -> std::convertible_to<t_return_type>;
        };

    static_assert(has_invoke,
                  "Invoker type does not have an appropriate invoke method");
    return t_invoker::template invoke<t_return_type>(
        static_cast<t_visitor &>(*this),
        static_cast<const_valid_visitable &>(base));
  }

public:
  return_type apply(base_type &base) {
    const auto *vtable = static_cast<t_concrete_visitor &>(*this)
                             .template get_vtable_ezvis__<vtable_traits_type>();
    function_type thunk = vtable->get(base.unique_tag_ezvis__());
    return (static_cast<t_concrete_visitor &>(*this).*thunk)(base);
  }

  return_type operator()(base_type &base) { return apply(base); }

  // clang-format off
#define EZVIS_VISIT_INVOKER(name)                                                                                      \
  struct invoker_ezvis__ {                                                                                             \
    template <typename t_return_type, typename t_visitor, typename t_visitable>                                        \
    static t_return_type invoke(t_visitor &visitor, t_visitable &visitable) {                                          \
      return static_cast<t_return_type>(visitor. name (visitable));                                                    \
    }                                                                                                                  \
  }
  // clang-format on

#define EZVIS_VISIT_CT(tovisit)                                                \
  template <typename t_traits> const auto *get_vtable_ezvis__() const {        \
    static constexpr ezvis::detail::vtable<                                    \
        t_traits, ezvis::compile_time_vtable, tovisit>                         \
        table;                                                                 \
    return &table;                                                             \
  }

#define EZVIS_VISIT_RT(tovisit)                                                \
  template <typename t_traits> const auto *get_vtable_ezvis__() const {        \
    static ezvis::detail::vtable<t_traits, ezvis::runtime_vtable, tovisit>     \
        table;                                                                 \
    return &table;                                                             \
  }
};

namespace detail {

template <typename T, typename... Ts>
using are_all_same =
    std::integral_constant<bool, (... && std::is_same_v<T, Ts>)>;
template <typename T, typename... Ts>
constexpr auto are_all_same_v = are_all_same<T, Ts...>::value;

template <typename t_base, typename tt_return_type, typename t_visitor_functor,
          typename t_to_visit>
class lambda_visitor
    : public visitor_base<
          t_base,
          lambda_visitor<t_base, tt_return_type, t_visitor_functor, t_to_visit>,
          tt_return_type>,
      private t_visitor_functor {
  using to_visit = t_to_visit;

public:
  lambda_visitor(t_visitor_functor func) : t_visitor_functor{func} {}

  EZVIS_VISIT_CT(to_visit);
  auto visit(auto &type) { return t_visitor_functor::operator()(type); }
  EZVIS_VISIT_INVOKER(visit);
};

} // namespace detail

template <typename t_return_type, typename... t_to_visit, typename t_base,
          typename t_visitor_functor>
t_return_type visit(t_visitor_functor func, t_base &base) {
  detail::lambda_visitor<t_base, t_return_type, t_visitor_functor,
                         std::tuple<t_to_visit...>>
      visitor{func};
  return visitor.apply(base);
}

template <typename t_return_type, typename t_to_visit, typename t_base,
          typename t_visitor_functor>
t_return_type visit_tuple(t_visitor_functor func, t_base &base) {
  detail::lambda_visitor<t_base, t_return_type, t_visitor_functor, t_to_visit>
      visitor{func};
  return visitor.apply(base);
}

} // namespace ezvis
