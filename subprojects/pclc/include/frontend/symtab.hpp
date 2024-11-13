// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "frontend/ast/ast_nodes/i_ast_node.hpp"
#include "frontend/types/types.hpp"
#include "utils/transparent.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace paracl::frontend {

class symtab final {
public:
  struct attributes {
    unsigned m_loc;
    ast::variable_expression *m_definition = nullptr;
  };

private:
  using map_type = utils::transparent::string_unordered_map<attributes>;
  map_type m_table;

public:
  void declare(std::string_view name, ast::variable_expression *def) {
    auto size = m_table.size();
    m_table.emplace(name, attributes{static_cast<unsigned>(size), def});
  }

  bool declared(std::string_view name) const { return m_table.count(name); }
  std::optional<attributes> get_attributes(std::string_view name) const {
    auto found = m_table.find(name);
    if (found == m_table.end())
      return std::nullopt;
    return found->second;
  }

  // Deprecated, prefer attributes func
  std::optional<unsigned> location(std::string_view name) const {
    auto found = m_table.find(name);
    if (found == m_table.end())
      return std::nullopt;
    return found->second.m_loc;
  }

public:
  auto begin() const { return m_table.begin(); }
  auto end() const { return m_table.end(); }
  auto size() const { return m_table.size(); }
};

class symtab_stack final : private std::vector<symtab *> {
public:
  void begin_scope(symtab &stab) { vector::push_back(&stab); }
  void end_scope() { vector::pop_back(); }

  vector::size_type size() const {
    return std::accumulate(
        vector::cbegin(), vector::cend(), std::size_t{0},
        [](auto a, auto &&stab) { return a + stab->size(); });
  }

  vector::size_type blocks() const { return vector::size(); }

  unsigned lookup_scope(std::string_view name) const {
    auto found =
        std::find_if(vector::crbegin(), vector::crend(),
                     [&name](auto &stab) { return stab->declared(name); });
    if (found == vector::crend()) {
      throw std::logic_error{
          "Trying to look up scope of a variable not present in symbol table"};
    }
    return std::distance(vector::crbegin(), found);
  }

  std::optional<symtab::attributes> lookup_symbol(std::string_view name) const {
    auto found =
        std::find_if(vector::crbegin(), vector::crend(),
                     [&name](auto &stab) { return stab->declared(name); });
    if (found == vector::crend())
      return std::nullopt;
    assert(*found && "Symbol table stack broken");
    return (*found)->get_attributes(name);
  }

  bool declared(std::string_view name) const {
    return (lookup_symbol(name) ? true : false);
  }
  void declare(std::string_view name, ast::variable_expression *def) {
    vector::back()->declare(name, def);
  }

  using vector::back;
  using vector::front;
};

} // namespace paracl::frontend
