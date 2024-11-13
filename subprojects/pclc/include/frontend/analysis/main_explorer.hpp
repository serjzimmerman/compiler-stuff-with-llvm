// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "frontend/ast/ast_container.hpp"
#include "frontend/ast/ast_nodes/assignment_statement.hpp"
#include "frontend/ast/ast_nodes/i_ast_node.hpp"
#include "frontend/ast/ast_nodes/statement_block.hpp"

#include "frontend/ast/node_identifier.hpp"
#include "frontend/error.hpp"
#include "frontend/symtab.hpp"
#include "frontend/types/types.hpp"

#include "ezvis/ezvis.hpp"

#include <iostream>
#include <string_view>

namespace paracl::frontend {

class main_explorer final
    : public ezvis::visitor_base<ast::i_ast_node, main_explorer, void> {
  symtab m_global_variables;

  using to_visit = ast::tuple_all_nodes;
  EZVIS_VISIT_CT(to_visit)

public:
  void explore(ast::assignment_statement &ref) {
    for (auto &var : ref) {
      auto name = var.name();
      if (!m_global_variables.declared(name))
        m_global_variables.declare(name, &var);
    }
  }

  void explore(ast::i_ast_node &) {}

  EZVIS_VISIT_INVOKER(explore);

  void explore(ast::ast_container &ast) {
    auto &root = *ast.get_root_ptr();
    const auto node_type = frontend::ast::identify_node(root);
    if (node_type != frontend::ast::ast_node_type::E_ERROR_NODE) {
      auto &block = static_cast<ast::statement_block &>(root);
      for (auto *expr : block)
        explore(*expr);
    }
  }
};

} // namespace paracl::frontend
