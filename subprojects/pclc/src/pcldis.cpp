// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

int main(int argc, char *argv[]) try {
  std::string input_file_name;

  popl::OptionParser op("Allowed options");
  auto help_option =
      op.add<popl::Switch>("h", "help", "Print this help message");
  auto input_file_option = op.add<popl::Implicit<std::string>>(
      "i", "input", "Specify input file", "");
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

  std::ifstream input_file;
  utils::try_open_file(input_file, input_file_name, std::ios::binary);

  auto ch = paracl::bytecode_vm::decl_vm::read_chunk(input_file);
  if (!ch) {
    fmt::println(stderr, "Could not read input binary");
    return k_exit_failure;
  }

  disassemble_chunk(*ch);
  return k_exit_success;

} catch (std::exception &e) {
  fmt::println(stderr, "Error: {}", e.what());
  return k_exit_failure;
}
