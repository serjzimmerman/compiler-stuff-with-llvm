// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "frontend/frontend_driver.hpp"

extern "C" {
#include "cswl-sim/bindings.h"
}

#include <cstdint>
#include <string_view>

namespace llvm {
class ExecutionEngine;
class Module;
class Function;
} // namespace llvm

namespace paracl::llvm_codegen::intrinsics {

inline constexpr std::string_view print_int32_name = "__pclc_print_int32";
inline constexpr std::string_view read_int32_name = "__pclc_read_int32";
inline constexpr std::string_view sim_init_name = "__pclc_sim_init";
inline constexpr std::string_view sim_exit_name = "__pclc_sim_exit";
inline constexpr std::string_view sim_flush_name = "__pclc_sim_flush";
inline constexpr std::string_view sim_rand_name = "__pclc_sim_rand";
inline constexpr std::string_view sim_put_pixel_name = "__pclc_sim_put_pixel";

void print_int32(int32_t value);
int32_t read_int32();

void add_intrinsics_mapping(llvm::ExecutionEngine &engine);

auto get_print_int32_function(llvm::Module &module) -> llvm::Function *;
auto get_read_int32_function(llvm::Module &module) -> llvm::Function *;
auto get_sim_init(llvm::Module &module) -> llvm::Function *;
auto get_sim_exit(llvm::Module &module) -> llvm::Function *;
auto get_sim_flush(llvm::Module &module) -> llvm::Function *;
auto get_sim_put_pixel(llvm::Module &module) -> llvm::Function *;
auto get_sim_rand(llvm::Module &module) -> llvm::Function *;

void add_function_intrinsics(frontend::ast::ast_container &ast);

} // namespace paracl::llvm_codegen::intrinsics
