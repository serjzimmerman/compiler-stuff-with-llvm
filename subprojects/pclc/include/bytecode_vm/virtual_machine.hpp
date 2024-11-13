// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "bytecode_vm/decl_vm.hpp"
#include "bytecode_vm/opcodes.hpp"
#include "utils/files.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>

#include <iostream>
#include <stdexcept>

#include <fmt/core.h>

namespace paracl::bytecode_vm {

// Once again, force internal linkage by putting all of this in an anonymous
// namespace. Just to be safe. constexpr global variables are const, and const
// implies internal linkage, but still... Better safe than sorry.
namespace {

namespace instruction_set {
using decl_vm::instruction_desc;

// push_const: Pushes a constant from the constant pool onto the stack
// `unsigned` -- index of the constant in the pool to push onto the stack
constexpr instruction_desc<E_PUSH_CONST_UNARY, unsigned> push_const_desc =
    "push_const";

// push_cons: Pushes a value from the stack slot onto the top. Is not
// destructive. Absolute addressing
constexpr instruction_desc<E_PUSH_LOCAL_UNARY, unsigned> push_local_desc =
    "push_local";
constexpr instruction_desc<E_MOV_LOCAL_UNARY, unsigned> mov_local_desc =
    "mov_local";

// mov_local_rel: Pops a value from the top of the stack, then moves into a
// stack slot of the address `sp + attr<0>` 'int` -- offset to apply to the
// stack pointer to calculate the final stack slot. Note: can be negative
constexpr instruction_desc<E_MOV_LOCAL_REL_UNARY, int> mov_local_rel_desc =
    "mov_local_rel";

// push_const_rel: Pushes a value from the stack slot onto the top. Is not
// destructive `int` -- same as mov_local_rel
constexpr instruction_desc<E_PUSH_LOCAL_REL_UNARY, int> push_local_rel_desc =
    "push_local_rel";

// pop: Pop a value from the top an discard
constexpr instruction_desc<E_POP_NULLARY> pop_desc = "pop";

// add: Add two topmost value on the stack descrutively. Pushes the result back
constexpr instruction_desc<E_ADD_NULLARY> add_desc = "add";

// sub: Subtracts two values destructively. left-hand side of the subtraction
// lies lower on the stack than the right-hand side
constexpr instruction_desc<E_SUB_NULLARY> sub_desc = "sub";

// mul: Multiply two values descructively. push(pop() * pop())
constexpr instruction_desc<E_MUL_NULLARY> mul_desc = "mul";

// div: Divide two values from the top of the stack. second = pop(), first =
// pop(), push(first / second)
constexpr instruction_desc<E_DIV_NULLARY> div_desc = "div";

// mod: Modulus division. Arguments like other binary operators
constexpr instruction_desc<E_MOD_NULLARY> mod_desc = "mod";

// and: Logical AND of two values from the top of the stack. If both of the
// values are non-zero, then pushes a non-zero value. Otherwise zero.
constexpr instruction_desc<E_AND_NULLARY> and_desc = "and";

// or: Logical OR. See logical AND.
constexpr instruction_desc<E_OR_NULLARY> or_desc = "or";

// not: Logical not. Converts non-zero to zero, zero to non-zero integer from
// the top.
constexpr instruction_desc<E_NOT_NULLARY> not_desc = "not";

// print: Print to stdout. Destructive.
constexpr instruction_desc<E_PRINT_NULLARY> print_desc = "print";

// push_read: Read from stdin and push the value onto the stack.
constexpr instruction_desc<E_PUSH_READ_NULLARY> push_read_desc = "push_read";

// cmp_eq, cmp_ne, cmp_gt, cmp_ls, cmp_ge, cmp_le. Destructive comparison of
// values. Names should be self-explanatory.
constexpr instruction_desc<E_CMP_EQ_NULLARY> cmp_eq_desc = "cmp_eq";
constexpr instruction_desc<E_CMP_NE_NULLARY> cmp_ne_desc = "cmp_ne";
constexpr instruction_desc<E_CMP_GT_NULLARY> cmp_gt_desc = "cmp_gt";
constexpr instruction_desc<E_CMP_LS_NULLARY> cmp_ls_desc = "cmp_ls";
constexpr instruction_desc<E_CMP_GE_NULLARY> cmp_ge_desc = "cmp_ge";
constexpr instruction_desc<E_CMP_LE_NULLARY> cmp_le_desc = "cmp_le";

constexpr instruction_desc<E_JMP_UNARY, unsigned> jmp_desc = "jmp";
constexpr instruction_desc<E_JMP_FALSE_UNARY, unsigned> jmp_false_desc =
    "jmp_false";
constexpr instruction_desc<E_JMP_TRUE_UNARY, unsigned> jmp_true_desc =
    "jmp_true";
constexpr instruction_desc<E_JMP_DYNAMIC_NULLARY> jmp_dynamic_desc =
    "jmp_dynamic";
constexpr instruction_desc<E_JMP_DYNAMIC_REL_UNARY, int> jmp_dynamic_rel_desc =
    "jmp_dynamic_rel";

constexpr instruction_desc<E_LOAD_R0_NULLARY> load_r0_desc = "load_r0";
constexpr instruction_desc<E_STORE_R0_NULLARY> store_r0_desc = "store_r0";

constexpr instruction_desc<E_UPDATE_SP_UNARY, unsigned> update_sp_desc =
    "update_sp";
constexpr instruction_desc<E_PUSH_SP_NULLARY> push_sp_desc = "push_sp";
constexpr instruction_desc<E_SETUP_CALL_NULLARY> setup_call_desc = "call_setup";

constexpr auto push_const_instr = push_const_desc >>
                                  [](auto &&ctx, auto &&attr) {
                                    ctx.push(ctx.constant(std::get<0>(attr)));
                                  };

constexpr instruction_desc<E_RETURN_NULLARY> return_desc = "ret";
constexpr auto return_instr = return_desc >> [](auto &&ctx, auto &&) {
  if (ctx.stack_empty())
    ctx.halt();
  else {
    auto sp = ctx.pop();
    ctx.set_sp(sp);
    auto ip = ctx.pop();
    ctx.set_ip(ip);
  }
};

constexpr auto pop_instr = pop_desc >> [](auto &&ctx, auto &&) { ctx.pop(); };

constexpr auto add_instr = add_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first + second);
};

constexpr auto sub_instr = sub_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first - second);
};

constexpr auto mul_instr = mul_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first * second);
};

constexpr auto div_instr = div_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first / second);
};

constexpr auto mod_instr = mod_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first % second);
};

constexpr auto and_instr = and_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first && second);
};

constexpr auto or_instr = or_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first || second);
};

constexpr auto not_instr = not_desc >> [](auto &&ctx, auto &&) {
  auto first = ctx.pop();
  ctx.push(!first);
};

constexpr auto cmp_eq_instr = cmp_eq_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first == second);
};

constexpr auto cmp_ne_instr = cmp_ne_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first != second);
};

constexpr auto cmp_gt_instr = cmp_gt_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first > second);
};

constexpr auto cmp_ls_instr = cmp_ls_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first < second);
};

constexpr auto cmp_ge_instr = cmp_ge_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first >= second);
};

constexpr auto cmp_le_instr = cmp_le_desc >> [](auto &&ctx, auto &&) {
  auto second = ctx.pop();
  auto first = ctx.pop();
  ctx.push(first <= second);
};

constexpr auto print_instr = print_desc >> [](auto &&ctx, auto &&) {
  auto first = ctx.pop();
  fmt::println("{:d}", first);
};

constexpr auto push_read = push_read_desc >> [](auto &&ctx, auto &&) {
  int val;
  std::cin >> val;
  ctx.push(val);
};

constexpr auto mov_local_rel_instr = mov_local_rel_desc >> [](auto &&ctx,
                                                              auto &&attr) {
  auto val = ctx.pop();
  ctx.at_stack(std::get<0>(attr) + ctx.sp()) = val;
};

constexpr auto push_local_rel_instr = push_local_rel_desc >> [](auto &&ctx,
                                                                auto &&attr) {
  auto val = ctx.at_stack(std::get<0>(attr) + ctx.sp());
  ctx.push(val);
};

constexpr auto conditional_jump = [](auto &&ctx, auto &&attr, bool cond) {
  if (!cond)
    return;
  ctx.set_ip(std::get<0>(attr));
};

constexpr auto jmp_instr = jmp_desc >> [](auto &&ctx, auto &&attr) {
  conditional_jump(ctx, attr, true);
};

constexpr auto jmp_dynamic_instr = jmp_dynamic_desc >> [](auto &&ctx, auto &&) {
  auto first = ctx.pop();
  ctx.set_ip(first);
};

constexpr auto jmp_dynamic_rel_instr = jmp_dynamic_rel_desc >> [](auto &&ctx,
                                                                  auto &&attr) {
  auto val = ctx.at_stack(std::get<0>(attr) + ctx.sp());
  ctx.set_ip(val);
};

constexpr auto jmp_true_instr = jmp_true_desc >> [](auto &&ctx, auto &&attr) {
  auto first = ctx.pop();
  conditional_jump(ctx, attr, first);
};

constexpr auto jmp_false_instr = jmp_false_desc >> [](auto &&ctx, auto &&attr) {
  auto first = ctx.pop();
  conditional_jump(ctx, attr, !first);
};

constexpr auto setup_call_instr = setup_call_desc >> [](auto &&ctx, auto &&) {
  auto cur_sp = ctx.sp();
  ctx.push(cur_sp);
};

constexpr auto push_sp_instr = push_sp_desc >> [](auto &&ctx, auto &&) {
  auto cur_sp = ctx.sp();
  ctx.push(cur_sp);
};

constexpr auto update_sp_instr = update_sp_desc >> [](auto &&ctx, auto &&attr) {
  auto new_sp = ctx.stack_size() - std::get<0>(attr);
  ctx.set_sp(new_sp);
};

constexpr auto load_r0_instr = load_r0_desc >> [](auto &&ctx, auto &&) {
  auto val = ctx.pop();
  ctx.set_r0(val);
};

constexpr auto push_local_instr = push_local_desc >>
                                  [](auto &&ctx, auto &&attr) {
                                    auto val = ctx.at_stack(std::get<0>(attr));
                                    ctx.push(val);
                                  };

constexpr auto mov_local_instr = mov_local_desc >> [](auto &&ctx, auto &&attr) {
  auto val = ctx.pop();
  ctx.at_stack(std::get<0>(attr)) = val;
};

constexpr auto store_r0_instr = store_r0_desc >>
                                [](auto &&ctx, auto &&) { ctx.push(ctx.r0()); };

constexpr auto paracl_isa = decl_vm::instruction_set_description(
    push_const_instr, return_instr, pop_instr, add_instr, sub_instr, mul_instr,
    div_instr, mod_instr, and_instr, or_instr, cmp_eq_instr, cmp_ne_instr,
    cmp_gt_instr, cmp_ls_instr, cmp_ge_instr, cmp_le_instr, print_instr,
    push_read, mov_local_rel_instr, push_local_rel_instr, jmp_instr,
    jmp_true_instr, jmp_false_instr, not_instr, setup_call_instr,
    jmp_dynamic_instr, jmp_dynamic_rel_instr, push_sp_instr, update_sp_instr,
    load_r0_instr, store_r0_instr, push_local_instr, mov_local_instr);

using paracl_isa_type = decltype(paracl_isa);

} // namespace instruction_set

[[maybe_unused]] auto create_paracl_vm() {
  using instruction_set::paracl_isa;
  using instruction_set::paracl_isa_type;
  return decl_vm::virtual_machine<paracl_isa_type>{paracl_isa};
}

} // namespace

} // namespace paracl::bytecode_vm

namespace paracl::bytecode_vm {} // namespace paracl::bytecode_vm
