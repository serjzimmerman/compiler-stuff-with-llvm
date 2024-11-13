// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "i_ast_node.hpp"

#include <string>
#include <string_view>

namespace paracl::frontend::ast {

class function_call : public i_expression, private std::vector<i_expression *> {
private:
  std::string m_name;

private:
  EZVIS_VISITABLE();

public:
  ast::function_definition *m_def;

  function_call(std::string name, location l,
                std::vector<i_expression *> params = {})
      : i_expression{l}, vector{std::move(params)}, m_name{std::move(name)} {}

  std::string_view name() const & { return m_name; }

  using vector::cbegin;
  using vector::cend;
  using vector::crbegin;
  using vector::crend;
  using vector::empty;
  using vector::size;

  auto begin() const { return vector::begin(); }
  auto end() const { return vector::end(); }

  void append_parameter(i_expression *ptr) { vector::push_back(ptr); }
};

} // namespace paracl::frontend::ast
