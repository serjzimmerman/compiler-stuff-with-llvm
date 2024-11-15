// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "frontend/ast/ast_nodes/function_decl.hpp"
#include "frontend/symtab.hpp"
#include "graphs/directed_graph.hpp"
#include "utils/transparent.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace paracl::frontend {

class function_table final {
public:
  struct function_attributes {
    ast::function_definition *definition = nullptr;
    bool recursive = false;
  };

private:
  using map_type =
      utils::transparent::string_unordered_map<function_attributes>;
  map_type m_table;

public:
  std::optional<function_attributes> lookup(std::string_view name) const {
    auto found = m_table.find(name);
    if (found == m_table.end())
      return std::nullopt;
    return found->second;
  }

  std::pair<function_attributes, bool>
  define_function(std::string_view name, function_attributes attributes) {
    auto [iter, inserted] = m_table.emplace(name, attributes);
    return std::pair{iter->second, inserted};
  }

  void set_recursive(const std::string &name) {
    m_table.at(name).recursive = true;
    // For some reason ::at does not work well with transparent comparators
  }

  auto begin() { return m_table.begin(); }
  auto end() { return m_table.end(); }
  auto begin() const { return m_table.cbegin(); }
  auto end() const { return m_table.cend(); }
  auto size() const { return m_table.size(); }
};

using usegraph_type =
    graphs::basic_directed_graph<std::string, ast::function_definition *, void>;

struct functions_analytics final {
  function_table named_functions;
  usegraph_type usegraph;
  symtab *global_stab;
};

} // namespace paracl::frontend
