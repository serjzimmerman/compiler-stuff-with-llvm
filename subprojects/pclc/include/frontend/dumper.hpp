// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "ast/ast_nodes/i_ast_node.hpp"

#include <iostream>
#include <string>

namespace paracl::frontend::ast {

std::string ast_dump_str(const i_ast_node *node);

inline void ast_dump(i_ast_node *node, std::ostream &os) {
  os << ast_dump_str(node);
}

} // namespace paracl::frontend::ast
