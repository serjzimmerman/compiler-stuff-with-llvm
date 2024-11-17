// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "ezvis/ezvis.hpp"
#include "location.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

namespace paracl::frontend::types {

enum class type_class {
  E_BUILTIN,
  E_COMPOSITE_FUNCTION,
  E_COMPOSITE_ARRAY,
};

enum class builtin_type_class {
  E_BUILTIN_INT,
  E_BUILTIN_VOID,
};

inline std::string builtin_type_to_string(builtin_type_class type_tag) {
  switch (type_tag) {
  case builtin_type_class::E_BUILTIN_VOID:
    return "void";
  case builtin_type_class::E_BUILTIN_INT:
    return "int";
  }

  assert(0 && "Broken builtin_type_class enum");
  std::terminate();
}

class i_type;

using unique_type = std::unique_ptr<i_type>;

class i_type : public ezvis::visitable_base<i_type> {
protected:
  type_class m_type_tag;

protected:
  i_type(type_class type_tag) : m_type_tag{type_tag} {}

public:
  EZVIS_VISITABLE();

  virtual unique_type clone() const = 0;
  virtual std::string to_string() const = 0;
  virtual bool is_equal(const i_type &) const = 0;
  type_class get_class() const { return m_type_tag; }

  virtual ~i_type() {}
};

class generic_type {
private:
  std::unique_ptr<i_type> m_impl = nullptr;

private:
  void check_impl() const {
    if (!m_impl)
      throw std::runtime_error{"Bad type dereference"};
  }

  generic_type(std::unique_ptr<i_type> ptr) : m_impl{std::move(ptr)} {}

public:
  generic_type() = default;

  template <typename T, typename... Ts> static generic_type make(Ts &&...args) {
    return generic_type{std::unique_ptr<T>{new T{std::forward<Ts>(args)...}}};
  }

  generic_type(generic_type &&) = default;
  generic_type &operator=(generic_type &&) = default;

  generic_type(const generic_type &rhs)
      : m_impl{rhs ? rhs.m_impl->clone() : nullptr} {}
  generic_type &operator=(const generic_type &rhs) {
    if (this == &rhs)
      return *this;
    generic_type temp{rhs};
    swap(temp);
    return *this;
  }

  ~generic_type() = default;

public:
  i_type &base() & {
    check_impl();
    return *m_impl;
  }

  const i_type &base() const & {
    check_impl();
    return *m_impl;
  }

  friend bool operator==(const generic_type &lhs, const generic_type &rhs) {
    return lhs.base().is_equal(rhs.base());
  }
  friend bool operator!=(const generic_type &lhs, const generic_type &rhs) {
    return !(lhs == rhs);
  }

  friend bool operator==(const generic_type &lhs, const i_type &rhs) {
    return lhs.base().is_equal(rhs);
  }
  friend bool operator==(const i_type &lhs, const generic_type &rhs) {
    return rhs.base().is_equal(lhs);
  }
  friend bool operator!=(const generic_type &lhs, const i_type &rhs) {
    return !(lhs == rhs);
  }
  friend bool operator!=(const i_type &lhs, const generic_type &rhs) {
    return !(lhs == rhs);
  }

  std::string to_string() const { return base().to_string(); }
  void swap(generic_type &rhs) { std::swap(*this, rhs); }
  operator bool() const { return m_impl.get(); }

  operator i_type &() { return base(); }
  operator const i_type &() const { return base(); }
};

class type_builtin final : public i_type {
private:
  builtin_type_class m_builtin_type_tag;

  EZVIS_VISITABLE();

public:
  static inline const generic_type type_int =
      generic_type::make<type_builtin>(builtin_type_class::E_BUILTIN_INT);
  static inline const generic_type type_void =
      generic_type::make<type_builtin>(builtin_type_class::E_BUILTIN_VOID);

public:
  type_builtin(builtin_type_class type_tag)
      : i_type{type_class::E_BUILTIN}, m_builtin_type_tag{type_tag} {}

  bool is_equal(const i_type &rhs) const override {
    return m_type_tag == rhs.get_class() &&
           static_cast<const type_builtin &>(rhs).m_builtin_type_tag ==
               m_builtin_type_tag;
  }

  auto get_builtin_class() const { return m_builtin_type_tag; }

  std::string to_string() const override {
    return builtin_type_to_string(m_builtin_type_tag);
  }
  unique_type clone() const override {
    return std::make_unique<type_builtin>(*this);
  }
};

class type_composite_function : public i_type,
                                private std::vector<generic_type> {
public:
  generic_type m_return_type;

  EZVIS_VISITABLE();

public:
  type_composite_function(std::vector<generic_type> arg_types,
                          generic_type return_type)
      : i_type{type_class::E_COMPOSITE_FUNCTION}, vector{std::move(arg_types)},
        m_return_type{return_type} {}

  bool is_equal(const i_type &rhs) const override {
    if (m_type_tag != rhs.get_class())
      return false;
    const auto &cast_rhs = static_cast<const type_composite_function &>(rhs);
    if (!m_return_type || !cast_rhs.m_return_type)
      return false;
    return (m_return_type == cast_rhs.m_return_type &&
            vector::size() == cast_rhs.size() &&
            std::equal(cbegin(), cend(), cast_rhs.cbegin()));
  }

  void set_argument_types(const std::vector<generic_type> &arg_types) {
    vector::clear();
    for (auto &&arg : arg_types) {
      vector::push_back(arg);
    }
  }

  std::string to_string() const override {
    std::vector<std::string> arg_types_str;
    std::transform(cbegin(), cend(), std::back_inserter(arg_types_str),
                   [](auto &&elem) { return elem.to_string(); });
    return fmt::format("({}) func({})",
                       m_return_type ? m_return_type.to_string()
                                     : "undetermined",
                       fmt::join(arg_types_str, ", "));
    ;
  }

  unique_type clone() const override {
    std::vector<generic_type> args;
    std::copy(cbegin(), cend(), std::back_inserter(args));
    return std::make_unique<type_composite_function>(std::move(args),
                                                     m_return_type);
  }

  generic_type &return_type() & { return m_return_type; }
  const generic_type &return_type() const & { return m_return_type; }

  using vector::cbegin;
  using vector::cend;
  using vector::crbegin;
  using vector::crend;
  using vector::empty;
  using vector::size;
};

class type_composite_array : public i_type {
public:
  generic_type m_value_type;
  uint32_t m_size;

  EZVIS_VISITABLE();

public:
  type_composite_array(generic_type value_type, uint32_t size)
      : i_type{type_class::E_COMPOSITE_ARRAY}, m_value_type{value_type},
        m_size{size} {}

  // TODO: This should be the default comparison operator.
  bool is_equal(const i_type &rhs) const override {
    if (m_type_tag != rhs.get_class())
      return false;
    const auto &cast_rhs = static_cast<const type_composite_array &>(rhs);
    return m_value_type.base().is_equal(cast_rhs.m_value_type.base()) &&
           m_size == cast_rhs.m_value_type;
  }

  std::string to_string() const override {
    std::vector<std::string> arg_types_str;
    return fmt::format("{}[{}]", m_value_type.to_string(), m_size);
  }

  unique_type clone() const override {
    return std::make_unique<type_composite_array>(*this);
  }
};

} // namespace paracl::frontend::types
