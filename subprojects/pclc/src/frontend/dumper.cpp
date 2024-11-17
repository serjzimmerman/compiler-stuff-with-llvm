// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "frontend/dumper.hpp"
#include "ezvis/ezvis.hpp"
#include "frontend/ast/ast_nodes.hpp"
#include "utils/files.hpp"
#include <fmt/core.h>

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace paracl::frontend::ast {

class ast_dumper
    : public ezvis::visitor_base<const i_ast_node, ast_dumper, void> {
private:
  using to_visit = tuple_all_nodes;

public:
  std::vector<const i_ast_node *> m_queue;
  std::stringstream m_ss;

private:
  void print_declare_node(const i_ast_node &ref, std::string_view label) {
    m_ss << fmt::format("\tnode_{} [label = \"{}\"];\n", fmt::ptr(&ref), label);
  }

  void print_bind_node(const i_ast_node &parent, const i_ast_node &child,
                       std::string_view label = "") {
    m_ss << fmt::format("\tnode_{} -> node_{} [label = \"{}\"]\n",
                        fmt::ptr(&parent), fmt::ptr(&child), label);
  }

public:
  ast_dumper() = default;

  EZVIS_VISIT_CT(to_visit)

  void dump_node(const assignment_statement &ref);
  void dump_node(const binary_expression &ref);
  void dump_node(const if_statement &ref);
  void dump_node(const print_statement &ref);
  void dump_node(const statement_block &ref);
  void dump_node(const unary_expression &ref);
  void dump_node(const while_statement &ref);
  void dump_node(const variable_declaration &ref);

  void dump_node(const read_expression &ref) {
    print_declare_node(ref, "<read> ?");
  }
  void dump_node(const error_node &ref) { print_declare_node(ref, "<error>"); }

  void dump_node(const variable_expression &ref) {
    print_declare_node(ref, fmt::format("<identifier> {}\n<type> {}",
                                        ref.name(), ref.type_str()));
  }

  void dump_node(const constant_expression &ref) {
    print_declare_node(ref,
                       fmt::format("<integer constant> {:d}", ref.value()));
  }

  void dump_node(const function_definition &ref);
  void dump_node(const return_statement &ref);
  void dump_node(const function_call &ref);
  void dump_node(const function_definition_to_ptr_conv &ref);

  EZVIS_VISIT_INVOKER(dump_node);

private:
  void add_next(const i_ast_node &node) { m_queue.push_back(&node); }

  const i_ast_node *take_next() {
    if (m_queue.empty()) {
      return nullptr;
    }

    const auto *ptr = m_queue.back();
    m_queue.pop_back();
    return ptr;
  }

public:
  std::string ast_dump(const i_ast_node *root) {
    m_ss.clear();
    m_queue.clear();

    if (root) {
      add_next(*root);
    }

    m_ss << fmt::format("digraph abstract_syntax_tree_{} {{\n", fmt::ptr(this));
    while (const auto *ptr = take_next()) {
      apply(*ptr);
    }
    m_ss << "}\n";

    return m_ss.str();
  }
};

void ast_dumper::dump_node(const assignment_statement &ref) {
  print_declare_node(ref, "<assignment>");
  const i_ast_node *prev = &ref;

  for (const auto &curr_ref : ref) {
    add_next(curr_ref);
    print_bind_node(*prev, curr_ref);
    prev = &curr_ref;
  }

  print_bind_node(ref, ref.right());
  add_next(ref.right());
}

void ast_dumper::dump_node(const binary_expression &ref) {
  print_declare_node(
      ref, fmt::format("<binary expression>: {}",
                       ast::binary_operation_to_string(ref.op_type())));

  print_bind_node(ref, ref.left());
  print_bind_node(ref, ref.right());

  add_next(ref.left());
  add_next(ref.right());
}

void ast_dumper::dump_node(const variable_declaration &ref) {
  print_declare_node(ref, fmt::format("<identifier> {}\n<type> {}", ref.name(),
                                      ref.type_str()));
}

void ast_dumper::dump_node(const if_statement &ref) {
  print_declare_node(ref, "<if>");

  print_bind_node(ref, ref.cond(), "<condition>");
  print_bind_node(ref, ref.true_block(), "<then>");

  add_next(ref.cond());
  add_next(ref.true_block());

  if (ref.else_block()) {
    print_bind_node(ref, *ref.else_block(), "<else>");
    add_next(*ref.else_block());
  }
}

void ast_dumper::dump_node(const print_statement &ref) {
  print_declare_node(ref, "<print_statement>");
  print_bind_node(ref, ref.expr());
  add_next(ref.expr());
}

void ast_dumper::dump_node(const statement_block &ref) {
  print_declare_node(
      ref, fmt::format("<statement_block>\n<type>: {}", ref.type_str()));
  for (const auto &v : ref) {
    print_bind_node(ref, *v);
    add_next(*v);
  }
}

void ast_dumper::dump_node(const unary_expression &ref) {
  print_declare_node(
      ref, fmt::format("<binary expression> {}",
                       ast::unary_operation_to_string(ref.op_type())));
  print_bind_node(ref, ref.expr());
  add_next(ref.expr());
}

void ast_dumper::dump_node(const while_statement &ref) {
  print_declare_node(ref, "<while>");

  print_bind_node(ref, ref.cond(), "<condition>");
  print_bind_node(ref, ref.block(), "<body>");

  add_next(ref.cond());
  add_next(ref.block());
}

void ast_dumper::dump_node(const function_definition &ref) {
  auto label = fmt::format(
      "<function definition>: {}\n<arg count>: {}\n<type>: {}",
      ref.name.value_or("anonymous"), ref.size(), ref.type.to_string());

  print_declare_node(ref, label);
  for (unsigned i = 0; const auto &v : ref) {
    print_bind_node(ref, v, fmt::format("arg {}", i++));
    add_next(v);
  }

  print_bind_node(ref, ref.body());
  add_next(ref.body());
}

void ast_dumper::dump_node(const return_statement &ref) {
  print_declare_node(ref, "<return statement>\n");
  print_bind_node(ref, ref.expr(), "<expression>");
  add_next(ref.expr());
}

void ast_dumper::dump_node(const function_call &ref) {
  print_declare_node(ref, fmt::format("<function call>: {}\n<param count>: {}",
                                      ref.name(), ref.size()));

  for (unsigned i = 0; const auto &v : ref) {
    print_bind_node(ref, *v, fmt::format("param {}", i++));
    add_next(*v);
  }
}

void ast_dumper::dump_node(const function_definition_to_ptr_conv &ref) {
  print_declare_node(ref, "<function def to ptr conversion>");
  print_bind_node(ref, ref.definition());
  add_next(ref.definition());
}

std::string ast_dump_str(const i_ast_node *node) {
  ast_dumper dumper;
  return dumper.ast_dump(node);
}

} // namespace paracl::frontend::ast
