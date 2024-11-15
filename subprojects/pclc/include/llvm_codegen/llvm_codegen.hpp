// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

extern "C" {
#include "cswl-sim/bindings.h"
}

#include "frontend/frontend_driver.hpp"

#include <llvm/IR/Module.h>

#include <memory>

namespace paracl::llvm_codegen {

namespace ast = frontend::ast;

auto emit_llvm_module(llvm::LLVMContext &ctx,
                      const frontend::frontend_driver &drv)
    -> std::unique_ptr<llvm::Module>;

} // namespace paracl::llvm_codegen
