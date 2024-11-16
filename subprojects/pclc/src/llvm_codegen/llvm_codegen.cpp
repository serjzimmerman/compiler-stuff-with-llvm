// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "ezvis/ezvis.hpp"

#include "llvm_codegen/intrinsics.hpp"
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

class codegen_stack_scope {
  using map_type = utils::transparent::string_unordered_map<llvm::Value *>;

private:
  map_type m_map;

public:
  codegen_stack_scope() = default;

  std::optional<llvm::Value *> lookup(std::string_view name) const {
    auto it = m_map.find(name);
    return (it == m_map.end() ? std::nullopt : std::optional{it->second});
  }

  std::size_t size() const { return m_map.size(); }

  void add_value(std::string_view name, llvm::Value *value) {
    [[maybe_unused]] auto res = m_map.emplace(name, value);
    assert(res.second && "Reinserting var with the same label");
  }
};

class codegen_stack_frame {
private:
  std::vector<codegen_stack_scope> m_blocks;

public:
  void begin_scope() { m_blocks.emplace_back(); }

  void end_scope() {
    assert(m_blocks.size() && "Ending nonexistent scope");
    m_blocks.pop_back();
  }

  void add_value(std::string_view name, llvm::Value *value) {
    assert(m_blocks.size() && "Scope stack is empty");
    auto &back = m_blocks.back();
    back.add_value(name, value);
  }

  std::optional<llvm::Value *> lookup_value(std::string_view name) const {
    llvm::Value *found_value = nullptr;

    [[maybe_unused]] auto found =
        std::find_if(m_blocks.crbegin(), m_blocks.crend(), [&](auto &block) {
          auto found = block.lookup(name);
          if (found)
            found_value = *found;
          return block.lookup(name).has_value();
        });

    return (found == m_blocks.crend() ? std::nullopt
                                      : std::optional{found_value});
  }

  void clear() { m_blocks.clear(); }

  const codegen_stack_scope &front() const & { return m_blocks.front(); }
};

class codegen_visitor final
    : public ezvis::visitor_base<const ast::i_ast_node, codegen_visitor,
                                 llvm::Value *> {
private:
  std::unique_ptr<llvm::Module> m_module;
  std::unique_ptr<llvm::IRBuilder<>> m_builder;
  codegen_stack_frame m_current_frame;
  std::unordered_map<const ast::i_expression *, llvm::Value *> m_value_map;
  std::unordered_map<const ast::function_definition *, llvm::Function *>
      m_function_defs;
  const frontend::functions_analytics *m_functions = nullptr;
  llvm::Function *m_main_function = nullptr;
  llvm::Function *m_current_function = nullptr;

private:
  auto ctx() const & -> llvm::LLVMContext & { return m_module->getContext(); }
  auto frame() & -> codegen_stack_frame & { return m_current_frame; }
  auto frame() const & -> const codegen_stack_frame & {
    return m_current_frame;
  }

  auto &global_stab() const & {
    assert(m_functions->global_stab);
    return *m_functions->global_stab;
  }

  void generate_function_declarations();
  void declarate_global_values();

  void set_current_function(llvm::Function *func) { m_current_function = func; }
  auto *get_current_function() const { return m_current_function; }

  auto get_main_function() const -> llvm::Function * { return m_main_function; }

  auto get_llvm_type(const frontend::types::i_type &type,
                     bool is_function_decl = false) const -> llvm::Type *;

private:
  using to_visit = std::tuple<ast::assignment_statement, ast::binary_expression,
                              ast::constant_expression, ast::print_statement,
                              ast::read_expression, ast::statement_block,
                              ast::unary_expression, ast::variable_expression,
                              ast::return_statement>;

public:
  EZVIS_VISIT_CT(to_visit);

  codegen_visitor(std::string_view module_name, llvm::LLVMContext &ctx,
                  const frontend::frontend_driver &drv)
      : m_module(std::make_unique<llvm::Module>(module_name, ctx)),
        m_builder(std::make_unique<llvm::IRBuilder<>>(ctx)),
        m_functions(&drv.functions()) {}

  auto generate(const ast::binary_expression &) -> llvm::Value *;
  auto generate(const ast::unary_expression &) -> llvm::Value *;
  auto generate(const ast::variable_expression &) -> llvm::Value *;
  auto generate(const ast::return_statement &) -> llvm::Instruction *;

  /// @return Pointer to the right-hand-side of the assignment.
  auto generate(const ast::assignment_statement &) -> llvm::Value *;

  auto generate(const ast::constant_expression &) -> llvm::Value *;
  auto generate(const ast::print_statement &) -> llvm::Instruction *;
  auto generate(const ast::read_expression &) -> llvm::Value *;

  /// @return Always nullptr
  /// TODO: Maybe there should be a separate expression visitor to avoid dummy
  /// return values?
  auto generate(const ast::statement_block &,
                bool global_scope = false) -> llvm::Value *;

  EZVIS_VISIT_INVOKER(generate);

  void generate_function(llvm::Function *llvm_func,
                         const ast::i_ast_node &entry_point);

  void generate_all(const ast::ast_container &ast);

  auto extract_module() -> std::unique_ptr<llvm::Module> {
    return std::move(m_module);
  }
};

auto codegen_visitor::get_llvm_type(const frontend::types::i_type &type,
                                    bool is_function_decl) const
    -> llvm::Type * {
  using namespace frontend::types;

  return ezvis::visit<llvm::Type *, type_composite_function, type_builtin>(
      utils::visitors{
          [&](const type_composite_function &func) {
            auto *return_type = get_llvm_type(func.return_type());
            auto args_types = std::vector<llvm::Type *>{};
            args_types.reserve(func.size());
            std::transform(
                func.cbegin(), func.cend(), std::back_inserter(args_types),
                [&](const auto &type) {
                  return get_llvm_type(type, /*is_function_decl=*/false);
                });
            auto *func_type =
                llvm::FunctionType::get(return_type, args_types, false);
            return is_function_decl ? static_cast<llvm::Type *>(func_type)
                                    : llvm::PointerType::get(ctx(), 0);
          },
          [&](const type_builtin &type) -> llvm::Type * {
            switch (type.get_builtin_class()) {
            case builtin_type_class::E_BUILTIN_INT:
              return llvm::Type::getInt32Ty(ctx());
            case builtin_type_class::E_BUILTIN_VOID:
              return llvm::Type::getVoidTy(ctx());
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
        get_llvm_type(function_type, /*is_function_decl=*/true));
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

  frame().begin_scope();

  for (auto &[name, attrs] : globals) {
    auto *def = attrs.m_definition;
    assert(def);
    auto *type = get_llvm_type(def->type);
    assert(type);
    // NOTE: This does not leak memory. The module takes ownership of this
    // pointer.
    // TODO: Find a better API to construct global variables. There should be
    // something better than this.
    [[maybe_unused]] auto *gv = new llvm::GlobalVariable(
        *m_module, type, /*isConstant=*/false,
        llvm::GlobalVariable::ExternalLinkage,
        /*Initializer=*/llvm::Constant::getNullValue(type),
        /*Name=*/name);

    frame().add_value(name, gv);
  }
}

void codegen_visitor::generate_function(llvm::Function *llvm_func,
                                        const ast::i_ast_node &entry_point) {
  assert(llvm_func);
  set_current_function(llvm_func);
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

auto codegen_visitor::generate(const ast::assignment_statement &ref)
    -> llvm::Value * {
  auto *value_to_store = apply(ref.right());

  for (auto start = ref.rbegin(), finish = ref.rend(); start != finish;
       ++start) {
    m_builder->CreateStore(value_to_store,
                           frame().lookup_value(start->name()).value());
  }

  return value_to_store;
}

auto codegen_visitor::generate(const ast::binary_expression &ref)
    -> llvm::Value * {
  using bin_op = ast::binary_operation;

  auto *lhs = apply(ref.left());
  auto *rhs = apply(ref.right());

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

  return result;
}

auto codegen_visitor::generate(const ast::constant_expression &constant)
    -> llvm::Value * {
  auto ap_int = llvm::APInt(32, constant.value());
  return llvm::Constant::getIntegerValue(get_llvm_type(constant.type), ap_int);
}

auto codegen_visitor::generate(const ast::print_statement &ref)
    -> llvm::Instruction * {
  auto *value = apply(ref.expr());
  return m_builder->CreateCall(intrinsics::get_print_int32_function(*m_module),
                               {value});
}

auto codegen_visitor::generate(const ast::read_expression &) -> llvm::Value * {
  return m_builder->CreateCall(intrinsics::get_read_int32_function(*m_module),
                               /*Args=*/{});
}

auto codegen_visitor::generate(const ast::statement_block &block,
                               bool global_scope) -> llvm::Value * {
  if (global_scope) {
    set_current_function(get_main_function());
    auto *entry_block =
        llvm::BasicBlock::Create(ctx(), "", get_main_function());
    m_builder->SetInsertPoint(entry_block);
    auto *last_ret = m_builder->CreateRet(llvm::Constant::getIntegerValue(
        llvm::Type::getInt32Ty(ctx()), llvm::APInt(32, 0)));
    m_builder->SetInsertPoint(last_ret);
  }

  if (!global_scope) {
    frame().begin_scope();

    for (auto &[name, attrs] : block.stab) {
      auto *variable_def = attrs.m_definition;
      assert(variable_def);
      auto *llvm_value =
          m_builder->CreateAlloca(get_llvm_type(variable_def->type));
      frame().add_value(name, llvm_value);
    }
  }

  for (const auto *st : block) {
    assert(st);

    const auto node_type = frontend::ast::identify_node(st);
    if (node_type != ast::ast_node_type::E_FUNCTION_DEFINITION)
      apply(*st);
  }

  frame().end_scope();

  return nullptr;
}

auto codegen_visitor::generate(const ast::unary_expression &ref)
    -> llvm::Value * {
  using unary_op = ast::unary_operation;

  auto *op = apply(ref.expr());
  assert(op);

  llvm::Value *result = [&]() -> llvm::Value * {
    switch (ref.op_type()) {
    case unary_op::E_UN_OP_NEG:
      return m_builder->CreateNeg(op);
    case unary_op::E_UN_OP_NOT:
      return m_builder->CreateNot(op);
    case unary_op::E_UN_OP_POS:
      return op;
    default:
      std::terminate();
    }
  }();

  return result;
}

auto codegen_visitor::generate(const ast::variable_expression &ref)
    -> llvm::Value * {
  auto *llvm_type = get_llvm_type(ref.type);
  auto *ptr_value = frame().lookup_value(ref.name()).value();
  assert(ptr_value->getType()->isPointerTy());
  return m_builder->CreateLoad(llvm_type, ptr_value);
}

auto codegen_visitor::generate(const ast::return_statement &ref)
    -> llvm::Instruction * {
  auto *op = apply(ref.expr());
  assert(op);
  return m_builder->CreateRet(op);
}

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
