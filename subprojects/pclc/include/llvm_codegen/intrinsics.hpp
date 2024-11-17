// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

extern "C" {
#include "cswl-sim/bindings.h"
}

#include <cstdint>

namespace llvm {
class ExecutionEngine;
class Module;
class Function;
} // namespace llvm

namespace paracl::llvm_codegen::intrinsics {

void print_int32(int32_t value);
int32_t read_int32();

void add_intrinsics_mapping(llvm::ExecutionEngine &engine);

auto get_print_int32_function(llvm::Module &module) -> llvm::Function *;
auto get_read_int32_function(llvm::Module &module) -> llvm::Function *;
auto get_sim_init(llvm::Module &module) -> llvm::Function *;
auto get_sim_flush(llvm::Module &module) -> llvm::Function *;
auto get_sim_put_pixel(llvm::Module &module) -> llvm::Function *;
auto get_sim_rand(llvm::Module &module) -> llvm::Function *;

} // namespace paracl::llvm_codegen::intrinsics
