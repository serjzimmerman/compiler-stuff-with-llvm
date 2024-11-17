// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "frontend/dumper.hpp"
#include "frontend/frontend_driver.hpp"

#ifdef PCLC_VM_CODEGEN
#include "codegen.hpp"
#endif

#ifdef PCLC_LLVM_CODEGEN
#include "llvm_codegen/intrinsics.hpp"
#include "llvm_codegen/llvm_codegen.hpp"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/InitLLVM.h>
#endif

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
  auto debug_option = op.add<popl::Switch>("", "debug", "Enable debug prints");

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

  auto &parse_tree = drv.ast();

#ifdef PCLC_LLVM_CODEGEN
  paracl::llvm_codegen::intrinsics::add_function_intrinsics(drv.ast());
#endif

  bool valid = drv.analyze();

  if (ast_dump_option->is_set()) {
    paracl::frontend::ast::ast_dump(parse_tree.get_root_ptr(), std::cout);
    return k_exit_success;
  }

  if (!valid) {
    return k_exit_failure;
  }

#ifdef PCLC_VM_CODEGEN
  auto handle_vm_codegen = [&]() {
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

    return k_exit_success;
  };

  return handle_vm_codegen();
#endif

#ifdef PCLC_LLVM_CODEGEN
  auto handle_llvm_codegen = [&]() {
    using namespace paracl::llvm_codegen;

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    auto llvm_shutdown = std::unique_ptr<void *, decltype([](auto *) {
                                           llvm::llvm_shutdown();
                                         })>(nullptr, {});

    auto ctx = llvm::LLVMContext();
    auto owning_module = emit_llvm_module(ctx, drv);
    auto &module_ref = *owning_module;

    if (debug_option->is_set())
      module_ref.dump();

    auto builder = llvm::EngineBuilder(std::move(owning_module));
    builder.setOptLevel(llvm::CodeGenOptLevel::Aggressive);
    auto engine = std::unique_ptr<llvm::ExecutionEngine>(builder.create());

    assert(engine);

    intrinsics::add_intrinsics_mapping(*engine);
    engine->setVerifyModules(/*Verify=*/true);
    engine->finalizeObject();
    engine->runFunction(module_ref.getFunction("main"), {});
  };

  handle_llvm_codegen();
#endif

} catch (std::exception &e) {
  fmt::println(stderr, "Error: {}", e.what());
  return k_exit_failure;
}
