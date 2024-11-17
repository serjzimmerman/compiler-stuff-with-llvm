// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "llvm_codegen/intrinsics.hpp"

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <fmt/format.h>

#include <iostream>
#include <string_view>

namespace paracl::llvm_codegen::intrinsics {

inline constexpr std::string_view print_int32_name = "__pclc_print_int32";
inline constexpr std::string_view read_int32_name = "__pclc_read_int32";
inline constexpr std::string_view sim_init_name = "__pclc_sim_init";
inline constexpr std::string_view sim_exit_name = "__pclc_sim_exit";
inline constexpr std::string_view sim_flush_name = "__pclc_sim_flush";
inline constexpr std::string_view sim_rand_name = "__pclc_sim_rand";
inline constexpr std::string_view sim_put_pixel_name = "__pclc_sim_put_pixel";

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

} // namespace paracl::llvm_codegen::intrinsics
