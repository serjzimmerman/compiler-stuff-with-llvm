// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "i_ast_node.hpp"
#include "variable_expression.hpp"

#include <cassert>

namespace paracl::frontend::ast {

class variable_declaration : public i_expression {
private:
  variable_expression m_variable;

  EZVIS_VISITABLE();

public:
  variable_declaration(variable_expression expr)
      : i_expression{expr.loc(), expr.type}, m_variable(expr) {}

  auto name() const & -> std::string_view { return m_variable.name(); }
  auto &get_variable_expr() & { return m_variable; }
  const auto &get_variable_expr() const & { return m_variable; }
};

} // namespace paracl::frontend::ast
