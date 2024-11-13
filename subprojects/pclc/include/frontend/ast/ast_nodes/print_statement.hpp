// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "i_ast_node.hpp"

namespace paracl::frontend::ast {

class print_statement : public i_ast_node {
  i_expression *m_expr;

  EZVIS_VISITABLE();

public:
  print_statement(i_expression &p_expr, location l)
      : i_ast_node{l}, m_expr{&p_expr} {}
  i_expression &expr() const { return *m_expr; }
};

} // namespace paracl::frontend::ast
