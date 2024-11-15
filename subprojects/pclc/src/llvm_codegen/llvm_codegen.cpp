// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "ezvis/ezvis.hpp"

#include "llvm_codegen/llvm_codegen.hpp"

#include "frontend/analysis/function_table.hpp"
#include "frontend/ast/ast_container.hpp"
#include "frontend/ast/ast_nodes.hpp"
#include "frontend/ast/ast_nodes/i_ast_node.hpp"
#include "frontend/ast/node_identifier.hpp"
#include "frontend/frontend_driver.hpp"
#include "frontend/symtab.hpp"
#include "utils/misc.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <fmt/format.h>

#include <filesystem>

namespace paracl::llvm_codegen {

class codegen_visitor final
    : public ezvis::visitor_base<const ast::i_ast_node, codegen_visitor, void> {
private:
  std::unique_ptr<llvm::Module> m_module;
  std::unique_ptr<llvm::IRBuilder<>> m_builder;
  std::unordered_map<const ast::i_expression *, llvm::Value *> m_value_map;
  std::unordered_map<const ast::function_definition *, llvm::Function *>
      m_function_defs;
  const frontend::functions_analytics *m_functions = nullptr;
  llvm::Function *m_main_function = nullptr;
  llvm::Function *m_current_function = nullptr;

private:
  auto ctx() const & -> llvm::LLVMContext & { return m_module->getContext(); }

  auto &global_stab() const & {
    assert(m_functions->global_stab);
    return *m_functions->global_stab;
  }

  void generate_function_declarations();
  void declarate_global_values();

  void visit_if_no_else(const ast::if_statement &);
  void visit_if_with_else(const ast::if_statement &);

  void set_current_function(llvm::Function *func) { m_current_function = func; }
  auto *get_current_function() const { return m_current_function; }

  auto get_main_function() const -> llvm::Function * { return m_main_function; }

  void map_ast_to_value(const ast::i_expression &node, llvm::Value *val) {
    [[maybe_unused]] auto [_, inserted] = m_value_map.insert({&node, val});
    assert(inserted);
  }

  auto lookup_value(const ast::i_expression &node) const -> llvm::Value * {
    return m_value_map.at(&node);
  }

  auto get_value(const ast::i_expression &node) const -> llvm::Value *;

private:
  using to_visit = std::tuple<
      ast::assignment_statement, ast::binary_expression,
      ast::constant_expression, ast::if_statement, ast::print_statement,
      ast::read_expression, ast::statement_block, ast::unary_expression,
      ast::variable_expression, ast::while_statement, ast::function_call,
      ast::return_statement, ast::function_definition_to_ptr_conv>;

public:
  EZVIS_VISIT_CT(to_visit);

  codegen_visitor(std::string_view module_name, llvm::LLVMContext &ctx,
                  const frontend::frontend_driver &drv)
      : m_module(std::make_unique<llvm::Module>(module_name, ctx)),
        m_builder(std::make_unique<llvm::IRBuilder<>>(ctx)),
        m_functions(&drv.functions()) {}

  void generate(const ast::assignment_statement &);
  auto generate(const ast::binary_expression &) -> llvm::Value *;
  void generate(const ast::constant_expression &);
  void generate(const ast::if_statement &);
  void generate(const ast::print_statement &);
  void generate(const ast::read_expression &);
  void generate(const ast::statement_block &, bool global_scope = false);
  void generate(const ast::unary_expression &);
  void generate(const ast::variable_expression &);
  void generate(const ast::while_statement &);
  void generate(const ast::function_call &);
  void generate(const ast::return_statement &);
  void generate(const ast::function_definition_to_ptr_conv &);

  EZVIS_VISIT_INVOKER(generate);

  void generate_function(llvm::Function *llvm_func,
                         const ast::i_ast_node &entry_point);

  void generate_all(const ast::ast_container &ast);

  auto extract_module() -> std::unique_ptr<llvm::Module> {
    return std::move(m_module);
  }
};

static auto get_llvm_type(llvm::LLVMContext &ctx,
                          const frontend::types::i_type &type,
                          bool is_function_decl = false) -> llvm::Type * {
  using namespace frontend::types;

  return ezvis::visit<llvm::Type *, type_composite_function, type_builtin>(
      utils::visitors{
          [&](const type_composite_function &func) {
            auto *return_type = get_llvm_type(ctx, func.return_type());
            auto args_types = std::vector<llvm::Type *>{};
            args_types.reserve(func.size());
            std::transform(
                func.cbegin(), func.cend(), std::back_inserter(args_types),
                [&](const auto &type) {
                  return get_llvm_type(ctx, type, /*is_function_decl=*/false);
                });
            auto *func_type =
                llvm::FunctionType::get(return_type, args_types, false);
            return is_function_decl ? static_cast<llvm::Type *>(func_type)
                                    : func_type->getPointerTo();
          },
          [&](const type_builtin &type) -> llvm::Type * {
            switch (type.get_builtin_class()) {
            case builtin_type_class::E_BUILTIN_INT:
              return llvm::Type::getInt32Ty(ctx);
            case builtin_type_class::E_BUILTIN_VOID:
              return llvm::Type::getVoidTy(ctx);
            }
          }},
      type);
}

static auto get_linkage_type(const ast::function_definition &func)
    -> llvm::Function::LinkageTypes {
  return func.is_anonymous() ? llvm::Function::InternalLinkage
                             : llvm::Function::ExternalLinkage;
}

void codegen_visitor::generate_function_declarations() {
  assert(m_functions);

  auto scheduled = graphs::recursive_topo_sort(m_functions->usegraph);
  for (auto &&[key, func] : scheduled) {
    assert(func);

    auto function_type = func->type;
    auto *llvm_function_type = llvm::cast<llvm::FunctionType>(
        get_llvm_type(ctx(), function_type, /*is_function_decl=*/true));
    auto function_name_or_none = func->name;

    auto *llvm_function = llvm::Function::Create(
        llvm_function_type, get_linkage_type(*func),
        /*AddrSpace=*/0, function_name_or_none.value_or(""), m_module.get());

    m_function_defs.insert({func, llvm_function});
  }

  auto *main_func_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx()),
                                                 /*isVarArg=*/false);

  m_main_function = llvm::Function::Create(
      main_func_type, llvm::Function::ExternalLinkage, "main", m_module.get());
}

void codegen_visitor::declarate_global_values() {
  auto &globals = global_stab();

  for (auto &[name, attrs] : globals) {
    auto *def = attrs.m_definition;
    assert(def);
    auto *type = get_llvm_type(ctx(), def->type, /*is_function_decl=*/false);
    assert(type);
    // NOTE: This does not leak memory. The module takes ownership of this
    // pointer.
    [[maybe_unused]] auto *gv = new llvm::GlobalVariable(
        *m_module, type, /*isConstant=*/false,
        llvm::GlobalVariable::ExternalLinkage,
        /*Initializer=*/llvm::Constant::getNullValue(type),
        /*Name=*/name);
    map_ast_to_value(*def, gv);
  }
}

auto codegen_visitor::get_value(const ast::i_expression &node) const
    -> llvm::Value * {
  auto node_type = ast::identify_node(node);

  if (node_type == ast::ast_node_type::E_VARIABLE_EXPRESSION) {
    auto *llvm_type =
        get_llvm_type(ctx(), node.type, /*is_function_decl=*/false);

    auto *ptr_value = lookup_value(node);
    assert(ptr_value->getType()->isPointerTy());
    return m_builder->CreateLoad(llvm_type, lookup_value(node));
  }

  return lookup_value(node);
}

void codegen_visitor::generate_function(llvm::Function *llvm_func,
                                        const ast::i_ast_node &entry_point) {
  set_current_function(llvm_func);
  assert(llvm_func);
  auto *entry_block = llvm::BasicBlock::Create(ctx(), "", llvm_func);
  assert(entry_block);
  m_builder->SetInsertPoint(entry_block);
  apply(entry_point);
}

void codegen_visitor::generate_all(const ast::ast_container &ast) {
  generate_function_declarations();
  declarate_global_values();

  for (const auto &[_, attr] : m_functions->usegraph) {
    auto &&[name, func] = *attr;
    generate_function(m_function_defs.at(func), func->body());
  }

  if (ast.get_root_ptr()) {
    ezvis::visit<void, ast::statement_block>(
        [this](auto &st) { generate(st, /*global_scope=*/true); },
        *ast.get_root_ptr());
  }
}

void codegen_visitor::generate(const ast::assignment_statement &ref) {
  apply(ref.right());
  auto *value_to_store = get_value(ref.right());

  for (auto start = ref.rbegin(), finish = ref.rend(); start != finish;
       ++start) {
    generate(*start);
    m_builder->CreateStore(value_to_store, lookup_value(*start));
  }
}

auto codegen_visitor::generate(const ast::binary_expression &ref)
    -> llvm::Value * {
  using bin_op = ast::binary_operation;

  apply(ref.left());
  apply(ref.right());

  auto *lhs = get_value(ref.left());
  auto *rhs = get_value(ref.right());

  llvm::Value *result = [&]() -> llvm::Value * {
    switch (ref.op_type()) {
    case bin_op::E_BIN_OP_ADD:
      return m_builder->CreateAdd(lhs, rhs);
    case bin_op::E_BIN_OP_SUB:
      return m_builder->CreateSub(lhs, rhs);
    case bin_op::E_BIN_OP_MUL:
      return m_builder->CreateMul(lhs, rhs);
    case bin_op::E_BIN_OP_DIV:
      return m_builder->CreateSDiv(lhs, rhs);
    case bin_op::E_BIN_OP_MOD:
      return m_builder->CreateSRem(lhs, rhs);
    case bin_op::E_BIN_OP_EQ:
      return m_builder->CreateICmpEQ(lhs, rhs);
    case bin_op::E_BIN_OP_NE:
      return m_builder->CreateICmpNE(lhs, rhs);
    case bin_op::E_BIN_OP_GT:
      return m_builder->CreateICmpSGT(lhs, rhs);
    case bin_op::E_BIN_OP_LS:
      return m_builder->CreateICmpSLT(lhs, rhs);
    case bin_op::E_BIN_OP_GE:
      return m_builder->CreateICmpSGE(lhs, rhs);
    case bin_op::E_BIN_OP_LE:
      return m_builder->CreateICmpSLE(lhs, rhs);
    case bin_op::E_BIN_OP_AND:
      return m_builder->CreateAnd(lhs, rhs);
    case bin_op::E_BIN_OP_OR:
      return m_builder->CreateOr(lhs, rhs);
    default:
      std::terminate();
    }
  }();

  map_ast_to_value(ref, result);

  return result;
}

void codegen_visitor::generate(const ast::constant_expression &constant) {
  auto ap_int = llvm::APInt(32, constant.value());
  auto *value = llvm::Constant::getIntegerValue(
      get_llvm_type(ctx(), constant.type), ap_int);
  map_ast_to_value(constant, value);
}

void codegen_visitor::generate(const ast::if_statement &) {}

void codegen_visitor::generate(const ast::print_statement &) {}

void codegen_visitor::generate(const ast::read_expression &) {}

void codegen_visitor::generate(const ast::statement_block &block,
                               bool global_scope) {
  if (global_scope) {
    set_current_function(get_main_function());
    auto *entry_block =
        llvm::BasicBlock::Create(ctx(), "", get_main_function());
    m_builder->SetInsertPoint(entry_block);
  }

  for (const auto *st : block) {
    assert(st);

    const auto node_type = frontend::ast::identify_node(st);
    if (node_type != ast::ast_node_type::E_FUNCTION_DEFINITION)
      apply(*st);
  }
}

void codegen_visitor::generate(const ast::unary_expression &) {}

void codegen_visitor::generate(const ast::variable_expression &ref) {
  if (m_value_map.contains(&ref))
    return;

  auto *val = m_builder->CreateAlloca(
      get_llvm_type(ctx(), ref.type, /*is_function_decl=*/false));

  map_ast_to_value(ref, val);
}

void codegen_visitor::generate(const ast::while_statement &) {}

void codegen_visitor::generate(const ast::function_call &) {}

void codegen_visitor::generate(const ast::return_statement &) {}

void codegen_visitor::generate(const ast::function_definition_to_ptr_conv &) {}

auto emit_llvm_module(llvm::LLVMContext &ctx,
                      const frontend::frontend_driver &drv)
    -> std::unique_ptr<llvm::Module> {
  auto &ast = drv.ast();
  auto filename = absolute(std::filesystem::path(drv.get_filename()));

  const auto *root_node = ast.get_root_ptr();
  assert(root_node);

  auto visitor = codegen_visitor(filename.string(), ctx, drv);

  visitor.generate_all(ast);

  return visitor.extract_module();
}

} // namespace paracl::llvm_codegen
