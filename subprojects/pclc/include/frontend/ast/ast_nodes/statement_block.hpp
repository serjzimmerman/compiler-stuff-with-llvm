// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "frontend/symtab.hpp"
#include "i_ast_node.hpp"
#include "return_statement.hpp"

#include <cassert>
#include <deque>
#include <vector>

namespace paracl::frontend::ast {

class statement_block : public i_expression, private std::deque<i_ast_node *> {
public:
  symtab stab;
  return_vector return_statements;

private:
  EZVIS_VISITABLE();

public:
  statement_block() = default;
  statement_block(std::vector<i_ast_node *> vec, location l)
      : i_expression{l}, deque(vec.begin(), vec.end()) {}

  void append_statement(i_ast_node &statement) {
    const bool empty = deque::empty();
    deque::push_back(&statement);

    if (empty) {
      m_loc = statement.loc();
    } else {
      m_loc += statement.loc();
    }
  }

  // HACK: Helper function to add intrinsic definitions to the beginning of the
  // ast.
  void add_intrinsic(i_ast_node &statement) { deque::push_front(&statement); }

public:
  using deque::back;
  using deque::begin;
  using deque::cbegin;
  using deque::cend;
  using deque::crbegin;
  using deque::crend;
  using deque::end;
  using deque::front;
  using deque::size;
};

} // namespace paracl::frontend::ast
