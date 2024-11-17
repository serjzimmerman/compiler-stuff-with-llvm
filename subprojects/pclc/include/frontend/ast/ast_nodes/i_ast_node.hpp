// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "ezvis/ezvis.hpp"
#include "frontend/types/types.hpp"
#include "location.hpp"
#include "utils/misc.hpp"

namespace paracl::frontend::ast {

class i_ast_node : public ezvis::visitable_base<i_ast_node> {
protected:
  location m_loc;

protected:
  i_ast_node(location l = location{}) : m_loc{l} {}

public:
  EZVIS_VISITABLE();
  location loc() const { return m_loc; }

  virtual ~i_ast_node() {}
};

class i_expression : public i_ast_node {
public:
  types::generic_type type;

public:
  i_expression(location l = location{}, types::generic_type p_type = {})
      : i_ast_node{l}, type{p_type} {}

  std::string type_str() const {
    if (!type)
      return "<undetermined>";
    return type.to_string();
  }
};

// Expressions
class binary_expression;
class constant_expression;
class read_expression;
class assignment_statement;
class statement_block; // Is a statement as well as an expression, just like
                       // assignment
class unary_expression;
class variable_expression;
class function_call;
class function_definition_to_ptr_conv;

// Statements
class if_statement;
class print_statement;
class while_statement;
class function_definition;
class return_statement;
class variable_declaration;

class error_node;

using tuple_expression_nodes =
    std::tuple<assignment_statement, binary_expression, constant_expression,
               read_expression, statement_block, unary_expression,
               variable_expression, function_call,
               function_definition_to_ptr_conv>;

using tuple_all_nodes = utils::tuple_add_types_t<
    tuple_expression_nodes, if_statement, print_statement, while_statement,
    error_node, function_definition, return_statement, variable_declaration>;

} // namespace paracl::frontend::ast
