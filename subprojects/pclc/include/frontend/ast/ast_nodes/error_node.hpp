// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "i_ast_node.hpp"
#include "location.hpp"

#include <string>

namespace paracl::frontend::ast {

class error_node : public i_expression {
private:
  std::string m_error_message;

  EZVIS_VISITABLE();

public:
  error_node(std::string_view msg, location l)
      : i_expression{l}, m_error_message{msg} {};
  std::string error_msg() const { return m_error_message; }
};

} // namespace paracl::frontend::ast
