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
#include "statement_block.hpp"

namespace paracl::frontend::ast {

class while_statement : public i_ast_node {
public:
  symtab symbol_table;

private:
  i_expression *m_condition;
  i_ast_node *m_block;

  EZVIS_VISITABLE();

public:
  while_statement(i_expression &cond, i_ast_node &block, location l)
      : i_ast_node{l}, m_condition{&cond}, m_block{&block} {}

  i_expression &cond() const { return *m_condition; }
  i_ast_node &block() const { return *m_block; }
};

} // namespace paracl::frontend::ast
