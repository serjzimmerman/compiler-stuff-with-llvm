// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include <fmt/format.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <iostream>
#include <string_view>

namespace paracl::llvm_codegen::intrinsics {

inline constexpr std::string_view print_int32_name = "__pclc_print_int32";
inline constexpr std::string_view read_int32_name = "__pclc_read_int32";

void print_int32(int32_t value) { fmt::println("{:d}", value); }

int32_t read_int32() {
  int32_t value;
  std::cin >> value;
  return value;
}

auto get_print_int32_function(llvm::Module &module) -> llvm::Function * {
  if (auto *ptr = module.getFunction(print_int32_name))
    return ptr;
  auto &ctx = module.getContext();
  auto *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx),
                                       {llvm::Type::getInt32Ty(ctx)},
                                       /*isVarArg=*/false);
  return llvm::Function::Create(type, llvm::Function::ExternalLinkage,
                                print_int32_name, &module);
}

auto get_read_int32_function(llvm::Module &module) -> llvm::Function * {
  if (auto *ptr = module.getFunction(read_int32_name))
    return ptr;
  auto &ctx = module.getContext();
  auto *type =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx), /*Params=*/{},
                              /*IsVarArgs=*/false);
  return llvm::Function::Create(type, llvm::Function::ExternalLinkage,
                                read_int32_name, &module);
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
    return nullptr;
  });
}

} // namespace paracl::llvm_codegen::intrinsics
