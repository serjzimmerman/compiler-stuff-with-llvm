// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "frontend/analysis/function_table.hpp"
#include "frontend/ast/ast_container.hpp"
#include "frontend/ast/ast_nodes.hpp"

#include "frontend/error.hpp"
#include "frontend/types/types.hpp"

#include "ezvis/ezvis.hpp"

#include <iostream>
#include <string_view>

namespace paracl::frontend {

class function_explorer final
    : public ezvis::visitor_base<ast::i_ast_node, function_explorer, void> {
private:
  std::vector<usegraph_type::value_type> m_function_stack;
  std::vector<error_report> *m_error_queue; // Diagnostics
  functions_analytics *m_analytics;
  ast::ast_container *m_ast;

private:
  void report_error(error_report report) {
    m_error_queue->push_back(std::move(report));
  }

public:
  function_explorer() = default;

  EZVIS_VISIT_CT(ast::tuple_all_nodes)

  void explore(const ast::binary_expression &ref) {
    apply(ref.right());
    apply(ref.left());
  }

  void explore(const ast::if_statement &ref) {
    apply(ref.cond());
    apply(ref.true_block());
    if (ref.else_block() != nullptr)
      apply(*ref.else_block());
  }

  void explore(const ast::statement_block &ref) {
    for (auto &statement : ref) {
      assert(statement);
      apply(*statement);
    }
  }

  void explore(const ast::while_statement &ref) {
    apply(ref.cond());
    apply(ref.block());
  }

  void explore(const ast::assignment_statement &ref) { apply(ref.right()); }
  void explore(const ast::print_statement &ref) { apply(ref.expr()); }
  void explore(const ast::unary_expression &ref) { apply(ref.expr()); }

  void explore(ast::return_statement &ref) {
    if (ref.empty())
      return;
    apply(ref.expr());
  }

  void explore(ast::function_definition &);
  void explore(const ast::function_definition_to_ptr_conv &ref);
  void explore(ast::function_call &);

  void explore(const ast::i_ast_node &) {}

  EZVIS_VISIT_INVOKER(explore);

public:
  bool explore(ast::ast_container &ast, functions_analytics &functions,
               std::vector<error_report> &errors) {
    m_function_stack.clear();

    m_ast = &ast;
    m_analytics = &functions;
    m_error_queue = &errors;

    apply(*m_ast->get_root_ptr());
    return errors.empty();
  }
};
} // namespace paracl::frontend
