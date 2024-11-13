// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "frontend/dumper.hpp"
#include "frontend/frontend_driver.hpp"

#include "codegen.hpp"
#include "common.hpp"

#include <concepts>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

int main(int argc, char *argv[]) try {
  std::string input_file_name;
  bool dump_binary = false;

  popl::OptionParser op("Allowed options");

  auto help_option =
      op.add<popl::Switch>("h", "help", "Print this help message");
  auto ast_dump_option = op.add<popl::Switch>("a", "ast-dump", "Dump AST");
  auto input_file_option = op.add<popl::Implicit<std::string>>(
      "i", "input", "Specify input file", "");
  auto output_file_option = op.add<popl::Implicit<std::string>>(
      "o", "output", "Specify output file for compiled program", "a.out");
  auto disas_option = op.add<popl::Switch>(
      "d", "disas", "Disassemble generated code (does not run the program)",
      &dump_binary);

  op.parse(argc, argv);

  if (help_option->is_set()) {
    fmt::println("{}", op.help());
    return k_exit_success;
  }

  if (auto res = read_input_file(*input_file_option, op); res.has_value()) {
    input_file_name = *res;
  } else {
    return k_exit_failure;
  }

  paracl::frontend::frontend_driver drv{input_file_name};
  drv.parse();

  const auto &parse_tree = drv.ast();
  bool valid = drv.analyze();

  if (ast_dump_option->is_set()) {
    paracl::frontend::ast::ast_dump(parse_tree.get_root_ptr(), std::cout);
    return k_exit_success;
  }

  if (!valid) {
    return k_exit_failure;
  }

  paracl::codegen::codegen_visitor generator;
  generator.generate_all(drv.ast(), drv.functions());
  auto ch = generator.to_chunk();

  if (dump_binary) {
    disassemble_chunk(ch);
    return k_exit_success;
  }

  if (output_file_option->is_set()) {
    std::string output_file_name = output_file_option->value();
    std::ofstream output_file;
    utils::try_open_file(output_file, output_file_name, std::ios::binary);
    write_chunk(output_file, ch);
    return k_exit_success;
  }

  execute_chunk(ch);

} catch (std::exception &e) {
  fmt::println(stderr, "Error: {}", e.what());
  return k_exit_failure;
}
