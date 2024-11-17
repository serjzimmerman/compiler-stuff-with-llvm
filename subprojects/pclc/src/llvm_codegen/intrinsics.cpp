// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "frontend/ast/ast_nodes/function_decl.hpp"

#include "llvm_codegen/intrinsics.hpp"

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <fmt/format.h>

#include <iostream>
#include <string_view>

namespace paracl::llvm_codegen::intrinsics {

void print_int32(int32_t value) { fmt::println("{:d}", value); }

int32_t read_int32() {
  int32_t value;
  std::cin >> value;
  return value;
}

static auto get_intrinsic_function(std::string_view name, llvm::Module &module,
                                   llvm::Type *return_type,
                                   llvm::ArrayRef<llvm::Type *> args = {}) {
  if (auto *ptr = module.getFunction(name))
    return ptr;
  auto *type = llvm::FunctionType::get(return_type, args,
                                       /*isVarArg=*/false);
  return llvm::Function::Create(type, llvm::Function::ExternalLinkage, name,
                                &module);
}

auto get_print_int32_function(llvm::Module &module) -> llvm::Function * {
  auto &ctx = module.getContext();
  return get_intrinsic_function(print_int32_name, module,
                                llvm::Type::getVoidTy(ctx),
                                {llvm::Type::getInt32Ty(ctx)});
}

auto get_read_int32_function(llvm::Module &module) -> llvm::Function * {
  auto &ctx = module.getContext();
  return get_intrinsic_function(read_int32_name, module,
                                llvm::Type::getInt32Ty(ctx));
}

auto get_sim_init(llvm::Module &module) -> llvm::Function * {
  auto &ctx = module.getContext();
  return get_intrinsic_function(sim_init_name, module,
                                llvm::Type::getVoidTy(ctx));
}

auto get_sim_exit(llvm::Module &module) -> llvm::Function * {
  auto &ctx = module.getContext();
  return get_intrinsic_function(sim_exit_name, module,
                                llvm::Type::getVoidTy(ctx));
}

auto get_sim_flush(llvm::Module &module) -> llvm::Function * {
  auto &ctx = module.getContext();
  return get_intrinsic_function(sim_flush_name, module,
                                llvm::Type::getVoidTy(ctx));
}

auto get_sim_put_pixel(llvm::Module &module) -> llvm::Function * {
  auto &ctx = module.getContext();
  auto *int32_type = llvm::Type::getInt32Ty(ctx);
  return get_intrinsic_function(sim_put_pixel_name, module,
                                llvm::Type::getVoidTy(ctx),
                                {
                                    int32_type,
                                    int32_type,
                                    int32_type,
                                });
}

auto get_sim_rand(llvm::Module &module) -> llvm::Function * {
  auto &ctx = module.getContext();
  return get_intrinsic_function(sim_rand_name, module,
                                llvm::Type::getInt32Ty(ctx));
}

void add_intrinsics_mapping(llvm::ExecutionEngine &engine) {
  engine.InstallLazyFunctionCreator([&](const std::string &name) -> void * {
#define HANDLE_INTRINSIC(func_name_, getter_, handler_)                        \
  if (name == func_name_) {                                                    \
    auto gv = reinterpret_cast<void *>(handler_);                              \
    assert(gv);                                                                \
    return gv;                                                                 \
  }
    HANDLE_INTRINSIC(print_int32_name, get_print_int32_function, print_int32);
    HANDLE_INTRINSIC(read_int32_name, get_read_int32_function, read_int32);
    HANDLE_INTRINSIC(sim_init_name, get_sim_init_function, sim_init);
    HANDLE_INTRINSIC(sim_exit_name, get_sim_exit_function, sim_exit);
    HANDLE_INTRINSIC(sim_flush_name, get_sim_flush_function, sim_flush);
    HANDLE_INTRINSIC(sim_rand_name, get_sim_rand_function, sim_rand);
    HANDLE_INTRINSIC(sim_put_pixel_name, get_sim_put_pixel_function,
                     sim_put_pixel);
    return nullptr;
  });
}

void add_function_intrinsics(frontend::ast::ast_container &ast) {
  auto *root_node = ast.get_root_ptr();
  if (!root_node)
    return;

  auto &dummy_intrinsic_body = ast.make_node<frontend::ast::statement_block>();

  using namespace std::string_literals;

  auto &type_int = frontend::types::type_builtin::type_int;
  auto &type_void = frontend::types::type_builtin::type_void;

  auto &sim_init = ast.make_node<frontend::ast::function_definition>(
      std::optional{"sim_init"s}, dummy_intrinsic_body, frontend::location(),
      std::vector<frontend::ast::variable_expression>{}, type_void);

  auto &sim_exit = ast.make_node<frontend::ast::function_definition>(
      std::optional{"sim_exit"s}, dummy_intrinsic_body, frontend::location(),
      std::vector<frontend::ast::variable_expression>{}, type_void);

  auto &sim_flush = ast.make_node<frontend::ast::function_definition>(
      std::optional{"sim_flush"s}, dummy_intrinsic_body, frontend::location(),
      std::vector<frontend::ast::variable_expression>{}, type_void);

  auto &sim_rand = ast.make_node<frontend::ast::function_definition>(
      std::optional{"sim_rand"s}, dummy_intrinsic_body, frontend::location(),
      std::vector<frontend::ast::variable_expression>{}, type_int);

  auto x_argument = ast.make_node<frontend::ast::variable_expression>(
      "__arg_x", type_int, frontend::location());
  auto y_argument = ast.make_node<frontend::ast::variable_expression>(
      "__arg_y", type_int, frontend::location());
  auto color_argument = ast.make_node<frontend::ast::variable_expression>(
      "__arg_color", type_int, frontend::location());
  auto &sim_put_pixel = ast.make_node<frontend::ast::function_definition>(
      std::optional{"sim_put_pixel"s}, dummy_intrinsic_body,
      frontend::location(),
      std::vector<frontend::ast::variable_expression>{x_argument, y_argument,
                                                      color_argument},
      type_void);

  auto &statements = static_cast<frontend::ast::statement_block &>(*root_node);
  statements.add_intrinsic(sim_init);
  statements.add_intrinsic(sim_exit);
  statements.add_intrinsic(sim_flush);
  statements.add_intrinsic(sim_rand);
  statements.add_intrinsic(sim_put_pixel);
}

} // namespace paracl::llvm_codegen::intrinsics
