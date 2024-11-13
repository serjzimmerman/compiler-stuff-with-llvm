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
#include <vector>

namespace paracl::frontend::ast {

class assignment_statement : public i_expression {
private:
  using lhs_vec = std::vector<variable_expression>;
  lhs_vec m_lefts;
  i_expression *m_right;

  EZVIS_VISITABLE();

public:
  assignment_statement(variable_expression left, i_expression &right,
                       location l)
      : i_expression{l}, m_right{&right} {
    m_lefts.push_back(left);
  }

  // Note[Segei]: Assignment is right associative, so this function appends
  // variables on the left, so location is extended to the left as well.
  void append_variable(variable_expression var) {
    m_lefts.push_back(var);
    m_loc.begin = var.loc().begin;
  }

  auto size() const { return m_lefts.size(); }

  auto begin() { return m_lefts.rbegin(); }
  auto end() { return m_lefts.rend(); }
  auto begin() const { return m_lefts.crbegin(); }
  auto end() const { return m_lefts.crend(); }

  auto rbegin() const { return m_lefts.cbegin(); }
  auto rend() const { return m_lefts.cend(); }

  i_expression &right() const { return *m_right; }
};

} // namespace paracl::frontend::ast
