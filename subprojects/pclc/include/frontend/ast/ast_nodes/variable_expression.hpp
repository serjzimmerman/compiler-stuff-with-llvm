
// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "frontend/types/types.hpp"
#include "i_ast_node.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace paracl::frontend::ast {

class variable_expression : public i_expression {
  std::string m_name;

public:
  EZVIS_VISITABLE();

  variable_expression(std::string p_name, types::generic_type type, location l)
      : i_expression{l, type}, m_name{p_name} {}
  variable_expression(std::string p_name, location l)
      : i_expression{l}, m_name{p_name} {}

  std::string_view name() const & { return m_name; }
};

} // namespace paracl::frontend::ast
