// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer paracl_FlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL                                                                \
  paracl::frontend::parser::symbol_type                                        \
  paracl::frontend::scanner::get_next_token()

#include "location.hpp"
#include "parser_generated.hpp"

namespace paracl::frontend {
class parser_driver;

class scanner final : public yyFlexLexer {
private:
  parser_driver &m_driver;
  position m_pos;

private:
  auto symbol_length() const { return yyleng; }

public:
  scanner(parser_driver &driver, std::string *filename = nullptr)
      : m_driver{driver}, m_pos{filename} {}
  paracl::frontend::parser::symbol_type get_next_token();

  location update_loc() {
    auto old_pos = m_pos;
    auto new_pos = (m_pos += symbol_length());
    return location{old_pos, new_pos};
  }
};

} // namespace paracl::frontend

#include "frontend_driver.hpp"
