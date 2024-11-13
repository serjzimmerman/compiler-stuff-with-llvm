// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "parser_generated.hpp"

#include "frontend/analysis/function_explorer.hpp"
#include "frontend/analysis/main_explorer.hpp"
#include "frontend/analysis/semantic_analyzer.hpp"

#include "frontend/ast/ast_container.hpp"
#include "frontend/error.hpp"
#include "frontend/scanner.hpp"
#include "frontend/source.hpp"
#include "frontend/types/types.hpp"

#include "graphs/directed_graph.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>

namespace paracl::frontend {

class parser_driver {
private:
  scanner m_scanner;
  parser m_parser;

  std::optional<error_kind> m_current_error;
  ast::ast_container m_ast;

  friend class parser;
  friend class scanner;

private:
  void report_error(std::string message, location l) {
    m_current_error = {message, l};
  }

  error_kind take_error() {
    auto error = m_current_error.value();
    m_current_error = std::nullopt;
    return error;
  }

public:
  parser_driver(std::string *filename)
      : m_scanner{*this, filename}, m_parser{m_scanner, *this} {}

  bool parse() { return m_parser.parse(); }
  void switch_input_stream(std::istream *is) {
    m_scanner.switch_streams(is, nullptr);
  }

  template <typename t_node_type, typename... t_args>
  auto *make_ast_node(t_args &&...args) {
    return &m_ast.make_node<t_node_type>(std::forward<t_args>(args)...);
  }

  void set_ast_root_ptr(ast::i_ast_node *ptr) { // nullptr is possible
    m_ast.set_root_ptr(ptr);
  }

  ast::ast_container &ast() & { return m_ast; }
  ast::i_ast_node *get_ast_root_ptr() & { return m_ast.get_root_ptr(); }
};

class frontend_driver {
private:
  source_input m_source;
  error_reporter m_reporter;

  std::unique_ptr<std::istringstream> m_iss;
  std::unique_ptr<parser_driver> m_parsing_driver;

  functions_analytics m_functions;

public:
  frontend_driver(std::filesystem::path input_path)
      : m_source{input_path}, m_reporter{m_source},
        m_iss{std::make_unique<std::istringstream>(m_source.iss())},
        m_parsing_driver{std::make_unique<parser_driver>(m_source.filename())} {
    m_parsing_driver->switch_input_stream(m_iss.get());
  }

  const ast::ast_container &ast() const & { return m_parsing_driver->ast(); }
  const functions_analytics &functions() const & { return m_functions; }

  void parse() { m_parsing_driver->parse(); }

  bool analyze() {
    auto &ast = m_parsing_driver->ast();
    if (!ast.get_root_ptr())
      return true;

    error_queue_type errors;
    function_explorer explorer;

    explorer.explore(ast, m_functions, errors);
    auto scheduled = graphs::recursive_topo_sort(m_functions.usegraph);

    semantic_analyzer analyzer{m_functions};
    analyzer.set_error_queue(errors);
    analyzer.set_ast(ast);
    analyzer.analyze_main(*ast.get_root_ptr());

    for (const auto &e : errors) {
      m_reporter.report_pretty_error(e);
    }

    return errors.empty();
  }
};

} // namespace paracl::frontend
