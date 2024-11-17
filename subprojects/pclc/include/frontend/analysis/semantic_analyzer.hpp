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

#include "frontend/analysis/function_table.hpp"
#include "frontend/ast/ast_container.hpp"
#include "frontend/ast/ast_nodes.hpp"

#include "frontend/error.hpp"
#include "frontend/symtab.hpp"
#include "frontend/types/types.hpp"

#include <fmt/format.h>

#include <stack>

namespace paracl::frontend {

class semantic_analyzer final
    : public ezvis::visitor_base<ast::i_ast_node, semantic_analyzer, void> {
private:
  symtab_stack m_scopes;
  functions_analytics *m_functions;
  ast::ast_container *m_ast;

  using raw_block_stack = std::stack<bool>;
  raw_block_stack m_raw_block_stack;

private:
  error_queue_type *m_error_queue;
  error_queue_type m_default_error_queue;

private:
  // Vector of return statements in the current functions.
  ast::return_vector *m_return_statements = nullptr;

private:
  bool m_type_errors_allowed =
      false; // Flag used to indicate that a type mismatch is not an error.
  // Set this flag to true when doing a first pass on recurisive functions.
  bool m_next_raw_block = false;

private:
  void report_error(std::string msg, location loc) {
    m_error_queue->push_back({error_kind{msg, loc}});
  }

  void report_error(error_report report) {
    m_error_queue->push_back(std::move(report));
  }

  bool expect_type_eq(const types::generic_type &lhs, const types::i_type &rhs,
                      location loc) {
    if (m_type_errors_allowed)
      return false;

    auto &&type = lhs;
    if (!type || !(type == rhs)) {

      if (!type) {
        report_error(fmt::format("Expression is not of expected type '{}'",
                                 rhs.to_string()),
                     loc);
      }

      else {
        report_error(fmt::format("Expression is of type '{}', expected '{}'",
                                 type.to_string(), rhs.to_string()),
                     loc);
      }

      return false;
    }

    return true;
  }

  bool expect_type_eq(const ast::i_expression &ref, const types::i_type &rhs) {
    return expect_type_eq(ref.type, rhs, ref.loc());
  }

  void check_return_types_matches(types::generic_type &type, location loc);

private:
  class block_guard {
  private:
    semantic_analyzer &m_analyzer;
    bool m_released = false;

  public:
    block_guard(semantic_analyzer &analyzer) : m_analyzer{analyzer} {}

    block_guard(const block_guard &) = delete;
    block_guard(block_guard &&) = delete;

    block_guard &operator=(const block_guard &) = delete;
    block_guard &operator=(block_guard &&) = delete;

    ~block_guard() {
      if (!m_released)
        m_analyzer.end_scope();
    }

    void release() {
      if (m_released)
        return;
      m_released = true;
      m_analyzer.end_scope();
    }
  };

  [[nodiscard]] block_guard begin_scope(symtab &stab) {
    m_scopes.begin_scope(stab);
    m_raw_block_stack.push(m_next_raw_block);
    return block_guard{*this};
  }

  void end_scope() {
    m_scopes.end_scope();
    m_raw_block_stack.pop();
    m_next_raw_block = false;
  }

  bool in_raw_block() const {
    if (m_raw_block_stack.empty())
      return false;
    return m_raw_block_stack.top();
  }

  bool in_value_block() const { return !in_raw_block(); }
  void next_raw_block() { m_next_raw_block = true; }
  void next_value_block() { m_next_raw_block = false; }

  [[nodiscard]] block_guard next_raw_block(symtab &stab) {
    next_raw_block();
    return begin_scope(stab);
  }

  [[nodiscard]] block_guard next_value_block(symtab &stab) {
    next_value_block();
    return begin_scope(stab);
  }

  ast::statement_block *try_get_block_ptr(ast::i_ast_node &);

public:
  EZVIS_VISIT_CT(ast::tuple_all_nodes)

  void analyze_node(const ast::error_node &ref) {
    report_error(ref.error_msg(), ref.loc());
  }

  // clang-format off
  void analyze_node(ast::read_expression &) {}
  void analyze_node(ast::constant_expression &) {}
  // clang-format on

  void analyze_node(ast::assignment_statement &);
  void analyze_node(ast::binary_expression &);

  void analyze_node(ast::if_statement &);
  void analyze_node(ast::print_statement &);

  void analyze_node(ast::statement_block &, bool main_block = false);
  void analyze_node(ast::unary_expression &);
  bool analyze_node(ast::variable_expression &, bool can_declare = false,
                    bool is_decl = false);
  void analyze_node(ast::while_statement &);

  void analyze_node(ast::function_call &);
  void analyze_node(ast::function_definition &);
  void analyze_node(ast::function_definition_to_ptr_conv &);
  void analyze_node(ast::return_statement &);
  void analyze_node(ast::variable_declaration &);

  EZVIS_VISIT_INVOKER(analyze_node);

  bool analyze_main(ast::i_ast_node &);
  bool analyze_func(ast::function_definition &ref, bool is_recursive);

public:
  semantic_analyzer() : m_error_queue{&m_default_error_queue} {}
  semantic_analyzer(functions_analytics &functions) {
    m_functions = &functions;
  } // Temporary

  void set_error_queue(std::vector<error_report> &errors) {
    m_error_queue = &errors;
  }
  void set_functions(functions_analytics &functions) {
    m_functions = &functions;
  }
  void set_ast(ast::ast_container &ast) { m_ast = &ast; }
  error_queue_type &get_error_queue() & { return *m_error_queue; }
};

} // namespace paracl::frontend
