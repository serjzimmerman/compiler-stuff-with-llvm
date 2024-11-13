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

class constant_expression
    : public i_expression { // Inherit from i_expression but dont pass trivial
                            // builtin type by shared ptr. Should fix in the
                            // future
  int m_val;

  EZVIS_VISITABLE();

public:
  constant_expression(int p_val, location l)
      : i_expression{l, types::type_builtin::type_int}, m_val{p_val} {}
  int value() const { return m_val; }
};

} // namespace paracl::frontend::ast
