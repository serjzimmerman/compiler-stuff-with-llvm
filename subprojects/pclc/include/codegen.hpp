// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "ezvis/ezvis.hpp"

#include "bytecode_vm/bytecode_builder.hpp"
#include "bytecode_vm/decl_vm.hpp"
#include "bytecode_vm/opcodes.hpp"
#include "bytecode_vm/virtual_machine.hpp"

#include "frontend/analysis/function_table.hpp"
#include "frontend/ast/ast_container.hpp"
#include "frontend/ast/ast_nodes.hpp"
#include "frontend/ast/ast_nodes/i_ast_node.hpp"
#include "frontend/ast/node_identifier.hpp"
#include "frontend/symtab.hpp"
#include "utils/misc.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace paracl::codegen {

namespace ast = frontend::ast;
namespace vm_instruction_set = bytecode_vm::instruction_set;
namespace vm_builder = bytecode_vm::builder;

using vm_builder::encoded_instruction;

class codegen_stack_block {
  using map_type = utils::transparent::string_unordered_map<unsigned>;

private:
  unsigned m_top;
  map_type m_map;

public:
  codegen_stack_block(unsigned curr_top = 0) : m_top{curr_top} {}

  std::optional<unsigned> lookup(std::string_view name) const {
    auto it = m_map.find(name);
    return (it == m_map.end() ? std::nullopt : std::optional{it->second});
  }

  void push_dummy() { ++m_top; }
  unsigned size() const { return m_top; }
  unsigned names() const { return m_map.size(); }

  void push_var(std::string_view name) {
    [[maybe_unused]] auto res = m_map.emplace(name, m_top++);
    assert(res.second && "Reinserting var with the same label");
  }

  void pop_dummy() {
    assert(m_top > 0 && "Ending nonexistent scope");
    --m_top;
  }
};

class codegen_stack_frame {
private:
  using map_type = utils::transparent::string_unordered_map<unsigned>;

  struct stack_block {
    unsigned m_top;
    map_type m_map;
  };

  std::vector<codegen_stack_block> m_blocks;

public:
  void begin_scope() {
    auto block = codegen_stack_block{size()};
    m_blocks.push_back(block);
  }

  void begin_scope(const frontend::symtab &stab) {
    begin_scope();

    for (const auto &v : stab) {
      push_var(v.first);
    }
  }

  void end_scope() {
    assert(m_blocks.size() && "Ending nonexistent scope");
    m_blocks.pop_back();
  }

  void push_var(std::string_view name) {
    auto &back = m_blocks.back();
    back.push_var(name);
  }

  void pop_dummy() {
    assert(m_blocks.size() && "Ending nonexistent scope");
    m_blocks.back().pop_dummy();
  }

  std::optional<unsigned> lookup_location(std::string_view name) const {
    unsigned loc = 0;

    [[maybe_unused]] auto found = std::find_if(
        m_blocks.crbegin(), m_blocks.crend(), [name, &loc](auto &block) {
          auto found = block.lookup(name);
          if (found)
            loc = *found;
          return block.lookup(name).has_value();
        });

    return (found == m_blocks.crend() ? std::nullopt : std::optional{loc});
  }

  void push_dummy() { m_blocks.back().push_dummy(); }

  unsigned size() const {
    if (m_blocks.empty())
      return 0;
    return m_blocks.back().size();
  }

  unsigned names() const {
    if (m_blocks.empty())
      return 0;
    return m_blocks.back().names();
  }

  void clear() { m_blocks.clear(); }

  codegen_stack_block front() { return m_blocks.front(); }
};

// It's really necessary to put all of this code inside an anonymous namespace
// to avoid external linkage. paracl_isa relies on lambda types
namespace {

class codegen_visitor final
    : public ezvis::visitor_base<const frontend::ast::i_ast_node,
                                 codegen_visitor, void> {
  using builder_type = bytecode_vm::builder::bytecode_builder<
      decltype(bytecode_vm::instruction_set::paracl_isa)>;

private:
  std::unordered_map<int, unsigned> m_constant_map;

  const frontend::ast::function_definition *m_curr_function;
  struct reloc_constant {
    unsigned m_index;
    unsigned m_address;
  };

  std::vector<reloc_constant> m_return_address_constants;

  struct dyn_jump_reloc {
    unsigned m_index;
    unsigned m_address;
    frontend::ast::function_definition *m_func_ptr;
  };

  std::vector<dyn_jump_reloc> m_dynamic_jumps_constants;

  struct reloc_info {
    unsigned m_reloc_index;
    frontend::ast::function_definition *m_func_ptr;
  };

  std::vector<reloc_info> m_relocations_function_calls;
  std::vector<unsigned> m_exit_relocations;

private:
  std::unordered_map<const frontend::ast::function_definition *, unsigned>
      m_function_defs;
  const frontend::functions_analytics *m_functions;

  codegen_stack_block
      m_global_scope; // Global scope stack variable distribution.
  codegen_stack_frame m_symtab_stack;

  builder_type m_builder;

  unsigned m_prev_stack_size = 0;
  bool m_is_currently_statement = false;

private:
  void set_currently_statement() { m_is_currently_statement = true; }
  void reset_currently_statement() { m_is_currently_statement = false; }
  bool is_currently_statement() const { return m_is_currently_statement; }

  void visit_if_no_else(const frontend::ast::if_statement &);
  void visit_if_with_else(const frontend::ast::if_statement &);

  unsigned lookup_or_insert_constant(int constant);

  unsigned current_constant_index() const {
    return m_constant_map.size() + m_return_address_constants.size() +
           m_dynamic_jumps_constants.size();
  }

private:
  auto emit_with_increment(auto &&desc) {
    increment_stack();
    return emit(desc);
  }

  auto emit_with_decrement(auto &&desc) {
    decrement_stack();
    return emit(desc);
  }

  auto emit(auto &&desc) { return m_builder.emit_operation(desc); }
  void emit_pop() { emit_with_decrement(vm_instruction_set::pop_desc); }

  // clang-format off
  void increment_stack() { m_symtab_stack.push_dummy(); }
  void decrement_stack() { m_symtab_stack.pop_dummy(); }
  // clang-format on

  void begin_scope(const frontend::symtab &stab) {
    m_symtab_stack.begin_scope(stab);
    for (unsigned i = 0; i < stab.size(); ++i) {
      emit(encoded_instruction{vm_instruction_set::push_const_desc,
                               lookup_or_insert_constant(0)});
    }
  }

  void end_scope() {
    auto current_size = m_symtab_stack.size();
    m_symtab_stack.end_scope();
    auto old_size = m_symtab_stack.size();

    assert(current_size >= old_size);

    for (unsigned i = 0; i < current_size - old_size; ++i) {
      emit(vm_instruction_set::pop_desc);
    }
  }

private:
  using to_visit = std::tuple<
      frontend::ast::assignment_statement, frontend::ast::binary_expression,
      frontend::ast::constant_expression, frontend::ast::if_statement,
      frontend::ast::print_statement, frontend::ast::read_expression,
      frontend::ast::statement_block, frontend::ast::unary_expression,
      frontend::ast::variable_expression, frontend::ast::while_statement,
      frontend::ast::function_call, frontend::ast::return_statement,
      frontend::ast::function_definition_to_ptr_conv>;

public:
  EZVIS_VISIT_CT(to_visit);

  codegen_visitor() = default;

  void generate(const frontend::ast::assignment_statement &);
  void generate(const frontend::ast::binary_expression &);
  void generate(const frontend::ast::constant_expression &);
  void generate(const frontend::ast::if_statement &);
  void generate(const frontend::ast::print_statement &);
  void generate(const frontend::ast::read_expression &);
  void generate(const frontend::ast::statement_block &,
                bool global_scope = false);
  void generate(const frontend::ast::unary_expression &);
  void generate(const frontend::ast::variable_expression &);
  void generate(const frontend::ast::while_statement &);
  void generate(const frontend::ast::function_call &);
  void generate(const frontend::ast::return_statement &);
  void generate(const frontend::ast::function_definition_to_ptr_conv &);

  EZVIS_VISIT_INVOKER(generate);

  unsigned generate_function(const frontend::ast::function_definition &);
  void generate_all(const frontend::ast::ast_container &ast,
                    const frontend::functions_analytics &functions);
  bytecode_vm::decl_vm::chunk to_chunk();
};

void codegen_visitor::generate(const ast::constant_expression &ref) {
  auto index = lookup_or_insert_constant(ref.value());
  emit_with_increment(
      encoded_instruction{vm_instruction_set::push_const_desc, index});
}

void codegen_visitor::generate(const ast::read_expression &) {
  emit_with_increment(vm_instruction_set::push_read_desc);
}

void codegen_visitor::generate(const ast::variable_expression &ref) {
  if (auto index = m_symtab_stack.lookup_location(ref.name()); index) {
    emit_with_increment(encoded_instruction{
        vm_instruction_set::push_local_rel_desc, index.value()});
    return;
  }

  if (auto index = m_global_scope.lookup(ref.name()); index) {
    emit_with_increment(encoded_instruction{vm_instruction_set::push_local_desc,
                                            index.value()});
  }
}

void codegen_visitor::generate(const ast::print_statement &ref) {
  reset_currently_statement();
  apply(ref.expr());
  emit_with_decrement(vm_instruction_set::print_desc);
}

void codegen_visitor::generate(const ast::assignment_statement &ref) {
  const bool emit_push = !is_currently_statement();
  apply(ref.right());

  auto move_to_location = [this](std::string_view name) {
    if (auto index = m_symtab_stack.lookup_location(name); index) {
      emit_with_decrement(
          encoded_instruction{vm_instruction_set::mov_local_rel_desc, *index});
      return;
    }

    else if (auto index = m_global_scope.lookup(name); index) {
      emit_with_decrement(
          encoded_instruction{vm_instruction_set::mov_local_desc, *index});
    }
  };

  auto push_from_location = [this](std::string_view name) {
    if (auto index = m_symtab_stack.lookup_location(name); index) {
      emit_with_increment(
          encoded_instruction{vm_instruction_set::push_local_rel_desc, *index});
      return;
    }

    else if (auto index = m_global_scope.lookup(name); index) {
      emit_with_increment(
          encoded_instruction{vm_instruction_set::push_local_desc, *index});
    }
  };

  const auto last_it = std::prev(ref.rend()); // Empty assignments can't exist.
  for (auto start = ref.rbegin(), finish = ref.rend(); start != finish;
       ++start) {
    move_to_location(start->name());

    bool is_last = (start == last_it);
    if (!is_last || (is_last && emit_push)) {
      push_from_location(start->name());
    }
  }
}

void codegen_visitor::generate(const ast::binary_expression &ref) {
  reset_currently_statement();
  apply(ref.left());
  reset_currently_statement();
  apply(ref.right());

  using bin_op = ast::binary_operation;
  switch (ref.op_type()) {
  case bin_op::E_BIN_OP_ADD:
    emit_with_decrement(vm_instruction_set::add_desc);
    break;
  case bin_op::E_BIN_OP_SUB:
    emit_with_decrement(vm_instruction_set::sub_desc);
    break;
  case bin_op::E_BIN_OP_MUL:
    emit_with_decrement(vm_instruction_set::mul_desc);
    break;
  case bin_op::E_BIN_OP_DIV:
    emit_with_decrement(vm_instruction_set::div_desc);
    break;
  case bin_op::E_BIN_OP_MOD:
    emit_with_decrement(vm_instruction_set::mod_desc);
    break;
  case bin_op::E_BIN_OP_EQ:
    emit_with_decrement(vm_instruction_set::cmp_eq_desc);
    break;
  case bin_op::E_BIN_OP_NE:
    emit_with_decrement(vm_instruction_set::cmp_ne_desc);
    break;
  case bin_op::E_BIN_OP_GT:
    emit_with_decrement(vm_instruction_set::cmp_gt_desc);
    break;
  case bin_op::E_BIN_OP_LS:
    emit_with_decrement(vm_instruction_set::cmp_ls_desc);
    break;
  case bin_op::E_BIN_OP_GE:
    emit_with_decrement(vm_instruction_set::cmp_ge_desc);
    break;
  case bin_op::E_BIN_OP_LE:
    emit_with_decrement(vm_instruction_set::cmp_le_desc);
    break;
  case bin_op::E_BIN_OP_AND:
    emit_with_decrement(vm_instruction_set::and_desc);
    break;
  case bin_op::E_BIN_OP_OR:
    emit_with_decrement(vm_instruction_set::or_desc);
    break;
  default:
    std::terminate();
  }
}

void codegen_visitor::generate(const ast::statement_block &ref,
                               bool global_scope) {
  bool should_return = ref.type != frontend::types::type_builtin::type_void;

  unsigned ret_addr_index = 0;
  unsigned prev_stack_size = m_prev_stack_size;

  if (should_return) {
    const auto const_index = current_constant_index();
    m_return_address_constants.push_back({const_index, 0}); // Dummy address
    ret_addr_index = m_return_address_constants.size() - 1;

    m_symtab_stack.begin_scope(); // scope to isolate IP and SP
    emit_with_increment(
        encoded_instruction{vm_instruction_set::push_const_desc, const_index});
    emit_with_increment(vm_instruction_set::setup_call_desc);

    m_prev_stack_size = m_symtab_stack.size();
  }

  begin_scope(ref.stab);

  if (ref.size()) {
    for (auto st_ptr : ref) {
      assert(st_ptr && "Broken statement pointer");
      using frontend::ast::ast_expression_types;
      auto &st = *st_ptr;

      const auto node_type = frontend::ast::identify_node(st);
      const auto is_expression =
          std::find(ast_expression_types.begin(), ast_expression_types.end(),
                    node_type) != ast_expression_types.end();

      bool is_assignment =
          (node_type == frontend::ast::ast_node_type::E_ASSIGNMENT_STATEMENT);
      bool is_return =
          (node_type == frontend::ast::ast_node_type::E_RETURN_STATEMENT);
      bool pop_unused_result = is_expression && !is_return;

      using expressions_and_base =
          utils::tuple_add_types_t<ast::tuple_expression_nodes,
                                   ast::i_ast_node>;
      auto type = ezvis::visit_tuple<frontend::types::generic_type,
                                     expressions_and_base>(
          ::utils::visitors{[](ast::i_expression &expr) { return expr.type; },
                            [](ast::i_ast_node &) {
                              return frontend::types::type_builtin::type_void;
                            }},
          st);

      if (is_assignment && pop_unused_result) {
        set_currently_statement();
      } else {
        reset_currently_statement();
      }

      if (node_type != ast::ast_node_type::E_FUNCTION_DEFINITION) {
        apply(st);
      }

      if ((!is_assignment) && (pop_unused_result) &&
          (type != frontend::types::type_builtin::type_void)) {
        emit_pop();
      }
    }
  }

  if (global_scope)
    m_global_scope = m_symtab_stack.front();
  end_scope();

  if (should_return) {
    m_return_address_constants.at(ret_addr_index).m_address =
        m_builder.current_loc();
    m_prev_stack_size = prev_stack_size;
    m_symtab_stack.end_scope(); // end scope for IP and SP
    emit_with_increment(vm_instruction_set::store_r0_desc);
  }
}

void codegen_visitor::visit_if_no_else(const ast::if_statement &ref) {
  reset_currently_statement();
  apply(ref.cond());

  auto index_jmp_to_false_block = emit_with_decrement(
      encoded_instruction{vm_instruction_set::jmp_false_desc, 0});

  set_currently_statement();
  apply(ref.true_block());

  auto jump_to_index = m_builder.current_loc();
  auto &to_relocate = m_builder.get_as(vm_instruction_set::jmp_false_desc,
                                       index_jmp_to_false_block);

  std::get<0>(to_relocate.m_attr) = jump_to_index;
}

void codegen_visitor::visit_if_with_else(const ast::if_statement &ref) {
  reset_currently_statement();
  apply(ref.cond());

  auto index_jmp_to_false_block = emit_with_decrement(
      encoded_instruction{vm_instruction_set::jmp_false_desc, 0});

  set_currently_statement();
  apply(ref.true_block());
  auto index_jmp_to_after_true_block =
      emit(encoded_instruction{vm_instruction_set::jmp_desc, 0});

  auto &to_relocate_else_jump = m_builder.get_as(
      vm_instruction_set::jmp_false_desc, index_jmp_to_false_block);
  std::get<0>(to_relocate_else_jump.m_attr) = m_builder.current_loc();

  set_currently_statement();
  apply(*ref.else_block());

  auto &to_relocate_after_true_block = m_builder.get_as(
      vm_instruction_set::jmp_desc, index_jmp_to_after_true_block);
  std::get<0>(to_relocate_after_true_block.m_attr) = m_builder.current_loc();
}

void codegen_visitor::generate(const ast::if_statement &ref) {
  begin_scope(ref.control_block_symtab);

  if (!ref.else_block()) {
    visit_if_no_else(ref);
  } else {
    visit_if_with_else(ref);
  }

  end_scope();
}

void codegen_visitor::generate(const ast::while_statement &ref) {
  begin_scope(ref.symbol_table);

  auto while_location_start = m_builder.current_loc();
  reset_currently_statement();
  apply(ref.cond());

  auto index_jmp_to_after_loop = emit_with_decrement(
      encoded_instruction{vm_instruction_set::jmp_false_desc, 0});
  set_currently_statement();

  apply(ref.block());
  emit(encoded_instruction{vm_instruction_set::jmp_desc, while_location_start});
  auto &to_relocate_after_loop_jump = m_builder.get_as(
      vm_instruction_set::jmp_false_desc, index_jmp_to_after_loop);
  std::get<0>(to_relocate_after_loop_jump.m_attr) = m_builder.current_loc();

  end_scope();
}

void codegen_visitor::generate(const ast::unary_expression &ref) {
  using unary_op = ast::unary_operation;
  reset_currently_statement();

  switch (ref.op_type()) {
  case unary_op::E_UN_OP_NEG: {
    emit_with_increment(encoded_instruction{vm_instruction_set::push_const_desc,
                                            lookup_or_insert_constant(0)});
    apply(ref.expr());
    emit_with_decrement(vm_instruction_set::sub_desc);
    break;
  }

  case unary_op::E_UN_OP_POS: {
    apply(ref.expr()); /* Do nothing */
    break;
  }

  case unary_op::E_UN_OP_NOT: {
    apply(ref.expr());
    emit(vm_instruction_set::not_desc);
    break;

  default:
    std::terminate();
  }
  }
}

void codegen_visitor::generate(const ast::function_call &ref) {
  bool is_return = false;

  if (ref.type != frontend::types::type_builtin::type_void) {
    is_return = true;
  }

  const auto const_index = current_constant_index();
  m_return_address_constants.push_back({const_index, 0}); // Dummy address
  const auto ret_addr_index = m_return_address_constants.size() - 1;

  m_symtab_stack.begin_scope(); // scope to isolate IP and SP
  emit_with_increment(
      encoded_instruction{vm_instruction_set::push_const_desc, const_index});
  emit_with_increment(vm_instruction_set::setup_call_desc);

  auto &&n_args = ref.size();
  for (auto &&e : ref) {
    assert(e);
    apply(*e);
  }
  emit(encoded_instruction{vm_instruction_set::update_sp_desc, n_args});

  if (ref.m_def) {
    auto relocate_index =
        emit(encoded_instruction{vm_instruction_set::jmp_desc});
    m_relocations_function_calls.push_back({relocate_index, ref.m_def});
  }

  else {
    int total_depth = m_symtab_stack.size() - n_args;
    int rel_pos =
        m_symtab_stack.lookup_location(ref.name()).value() - total_depth;

    emit_with_increment(
        encoded_instruction{vm_instruction_set::push_local_rel_desc, rel_pos});
    emit_with_decrement(vm_instruction_set::jmp_dynamic_desc);
  }

  m_return_address_constants.at(ret_addr_index).m_address =
      m_builder.current_loc();
  m_symtab_stack.end_scope();

  if (is_return) {
    emit_with_increment(encoded_instruction{vm_instruction_set::store_r0_desc});
  }
}

void codegen_visitor::generate(const frontend::ast::return_statement &ref) {
  if (!ref.empty()) {
    apply(ref.expr());
    emit_with_decrement(vm_instruction_set::load_r0_desc);
  }

  // clean up local variables
  unsigned local_var_n = m_symtab_stack.size() - m_prev_stack_size;
  for (unsigned i = 0; i < local_var_n; ++i) {
    emit(encoded_instruction{vm_instruction_set::pop_desc});
  }

  emit(encoded_instruction{vm_instruction_set::return_desc});
}

void codegen_visitor::generate(
    const frontend::ast::function_definition_to_ptr_conv &ref) {
  const auto const_index = current_constant_index();
  m_dynamic_jumps_constants.push_back(
      {const_index, 0, &ref.definition()}); // Dummy address
  emit_with_increment(
      encoded_instruction{vm_instruction_set::push_const_desc, const_index});
}

unsigned codegen_visitor::generate_function(
    const frontend::ast::function_definition &ref) {
  m_symtab_stack.clear();

  m_curr_function = &ref;
  m_symtab_stack.begin_scope();
  for (auto &&param : ref) {
    m_symtab_stack.push_var(param.name());
  }

  auto &&function_pos = m_builder.current_loc();
  m_function_defs.insert({&ref, function_pos});
  apply(ref.body());

  end_scope();
  emit(encoded_instruction{vm_instruction_set::return_desc});

  return function_pos;
}

unsigned codegen_visitor::lookup_or_insert_constant(int constant) {
  auto found = m_constant_map.find(constant);
  unsigned index;

  if (found == m_constant_map.end()) {
    index = current_constant_index();
    m_constant_map.insert({constant, index});
  }

  else {
    index = found->second;
  }

  return index;
}

void codegen_visitor::generate_all(
    const frontend::ast::ast_container &ast,
    const frontend::functions_analytics &functions) {
  m_return_address_constants.clear();
  m_functions = &functions;

  if (ast.get_root_ptr()) { // clang-format off
    ezvis::visit<void, frontend::ast::statement_block>(
      [this](auto &st) { generate(st, true);  },
      *ast.get_root_ptr()
    ); // clang-format on
  }

  emit(vm_instruction_set::return_desc); // Last instruction is ret
  for (auto &&[name, attr] : functions.named_functions) {
    assert(attr.definition && "Attribute definition pointer can't be nullptr");
    generate_function(*attr.definition);
  }

  for (auto &&reloc : m_relocations_function_calls) {
    auto &relocate_instruction =
        m_builder.get_as(vm_instruction_set::jmp_desc, reloc.m_reloc_index);
    relocate_instruction.m_attr = m_function_defs.at(reloc.m_func_ptr);
  }

  for (auto &&dynjmp : m_dynamic_jumps_constants) {
    assert(dynjmp.m_func_ptr);
    dynjmp.m_address = m_function_defs.at(dynjmp.m_func_ptr);
  }
}

paracl::bytecode_vm::decl_vm::chunk codegen_visitor::to_chunk() {
  auto ch = m_builder.to_chunk();

  std::vector<int> constants;
  constants.resize(current_constant_index());

  for (auto &&[constant, index] : m_constant_map) {
    constants[index] = constant;
  }

  for (auto &&v : m_return_address_constants) {
    constants[v.m_index] = v.m_address;
  }

  for (auto &&v : m_dynamic_jumps_constants) {
    constants[v.m_index] = v.m_address;
  }

  ch.set_constant_pool(std::move(constants));
  return ch;
}

} // namespace

} // namespace paracl::codegen
