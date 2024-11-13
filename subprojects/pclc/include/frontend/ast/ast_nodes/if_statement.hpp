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

class if_statement : public i_ast_node {
public:
  symtab control_block_symtab;
  symtab true_symtab;
  symtab false_symtab;

private:
  i_expression *m_condition;
  i_ast_node *m_true_block;
  i_ast_node *m_else_block = nullptr;

  EZVIS_VISITABLE();

public:
  if_statement(i_expression &cond, i_ast_node &true_block, location l)
      : i_ast_node{l}, m_condition{&cond}, m_true_block{&true_block} {}

  if_statement(i_expression &cond, i_ast_node &true_block,
               i_ast_node &else_block, location l)
      : i_ast_node{l}, m_condition{&cond}, m_true_block{&true_block},
        m_else_block{&else_block} {}

  i_expression &cond() const { return *m_condition; }
  i_ast_node &true_block() const { return *m_true_block; }
  i_ast_node *else_block() const { return m_else_block; }
};

} // namespace paracl::frontend::ast
