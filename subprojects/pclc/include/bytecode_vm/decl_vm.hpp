// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "utils/algorithm.hpp"
#include "utils/files.hpp"
#include "utils/misc.hpp"

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

namespace paracl::bytecode_vm::decl_vm {

class vm_error : public std::runtime_error {
public:
  vm_error(std::string err_msg) : std::runtime_error{err_msg} {}
};

using constant_pool_type = std::vector<int>;
using binary_code_buffer_type = std::vector<char>;

class chunk {
private:
  binary_code_buffer_type m_binary_code;
  constant_pool_type m_constant_pool;

public:
  using value_type = binary_code_buffer_type::value_type;

  chunk() = default;

  chunk(binary_code_buffer_type p_bin, constant_pool_type p_const)
      : m_binary_code{std::move(p_bin)}, m_constant_pool{std::move(p_const)} {}

  chunk(std::input_iterator auto bin_begin, std::input_iterator auto bin_end,
        std::input_iterator auto const_begin,
        std::input_iterator auto const_end)
      : m_binary_code{bin_begin, bin_end},
        m_constant_pool{const_begin, const_end} {}

  template <typename T> void push_value(T &&val) {
    utils::write_little_endian(std::forward<T>(val),
                               std::back_inserter(m_binary_code));
  };

  void push_back(value_type code) { m_binary_code.push_back(code); }
  void set_constant_pool(constant_pool_type constants) {
    m_constant_pool = std::move(constants);
  }

  auto binary_begin() const { return m_binary_code.cbegin(); }
  auto binary_end() const { return m_binary_code.cend(); }
  auto binary_size() const { return m_binary_code.size(); }
  auto binary_data() const { return m_binary_code.data(); }

  auto constants_begin() const { return m_constant_pool.cbegin(); }
  auto constants_end() const { return m_constant_pool.cend(); }
  auto constants_size() const { return m_constant_pool.size(); }

  auto constant_at(std::size_t id) const { return m_constant_pool.at(id); }
};

std::optional<chunk> read_chunk(std::istream &);
void write_chunk(std::ostream &, const chunk &);

template <typename, typename> struct instruction;

using opcode_underlying_type = chunk::value_type;
template <opcode_underlying_type ident, typename... Ts>
struct instruction_desc {
  static constexpr auto opcode = ident;
  static constexpr auto binary_size =
      sizeof(opcode_underlying_type) + (sizeof(Ts) + ... + 0);

  const std::string_view name;
  using attribute_types = std::tuple<Ts...>;

  constexpr auto get_name() const { return name; }
  static constexpr auto get_opcode() { return opcode; }
  static constexpr auto get_size() { return binary_size; }

  constexpr instruction_desc(const char *debug_name) : name{debug_name} {
    if (!debug_name || name[0] == '\0')
      throw vm_error{"Empty debug names aren't allowed"};
  }

  constexpr auto operator>>(auto action) const {
    return instruction{*this, action};
  }

  template <auto... I>
  static void pretty_print(auto &os, const attribute_types &tuple,
                           std::index_sequence<I...>) {
    auto print_list_element = [&os, &tuple](auto i) {
      os << (i == 0 ? "" : ", "),
          utils::padded_hex_printer(os, std::get<i>(tuple));
    };
    (print_list_element(std::integral_constant<std::size_t, I>()), ...);
  }

  template <typename t_stream>
  t_stream &pretty_print(t_stream &os, const attribute_types &attr) const {
    os << name;
    if constexpr (std::tuple_size_v<attribute_types> != 0) {
      os << " [ ";
      pretty_print(
          os, attr,
          std::make_index_sequence<std::tuple_size_v<attribute_types>>());
      os << " ]";
    }
    return os;
  }
};

template <typename t_desc, typename t_action> struct instruction {
  using description_type = t_desc;
  using attribute_tuple_type = typename description_type::attribute_types;

public:
  const description_type description;
  t_action action = nullptr;

public:
  constexpr instruction(t_desc p_description, t_action p_action)
      : description{p_description}, action{p_action} {}
  constexpr auto get_name() const { return description.get_name(); }
  constexpr auto get_opcode() const { return description.get_opcode(); }
  constexpr auto get_size() const { return description.get_size(); }

  template <typename t_stream>
  t_stream &pretty_print(t_stream &os, const attribute_tuple_type &attr) const {
    description.pretty_print(os, attr);
    return os;
  }

  struct decoded_instruction {
    const instruction *instr;
    attribute_tuple_type attributes;
  };

  template <auto I>
  static std::tuple_element_t<I, attribute_tuple_type>
  decode_attribute(auto &first, auto last) {
    auto [val, iter] = ::utils::read_little_endian<
        std::tuple_element_t<I, attribute_tuple_type>>(first, last);
    if (!val)
      throw vm_error{"Decoding error"};
    first = iter;
    return val.value();
  }

  template <auto... I>
  static attribute_tuple_type
  decode_attributes(std::forward_iterator auto &first,
                    [[maybe_unused]] std::forward_iterator auto last,
                    std::index_sequence<I...>) {
    return std::make_tuple(decode_attribute<I>(first, last)...);
  }

  decoded_instruction decode(std::forward_iterator auto &first,
                             std::forward_iterator auto last) const {
    auto seq =
        std::make_index_sequence<std::tuple_size_v<attribute_tuple_type>>{};
    auto attributes = decode_attributes(first, last, seq);
    return decoded_instruction{this, attributes};
  }
};

template <typename> class virtual_machine;
using execution_value_type = int32_t;

template <typename t_desc> struct context {
  friend class virtual_machine<t_desc>;

private:
  using execution_stack_type = std::vector<execution_value_type>;

  execution_stack_type m_execution_stack;
  chunk m_program_code;

  binary_code_buffer_type::const_iterator m_ip, m_ip_end;
  execution_stack_type::size_type m_sp = 0;
  execution_value_type m_r0 = 0;

  bool m_halted = false;

public:
  context() = default;

  context(chunk ch) : m_program_code{ch} {
    m_ip = m_program_code.binary_begin();
    m_ip_end = m_program_code.binary_end();
  }

  unsigned ip() const {
    return std::distance(m_program_code.binary_begin(), m_ip);
  }
  unsigned sp() const { return m_sp; }

  void set_sp(unsigned new_sp) { m_sp = new_sp; }
  void set_r0(execution_value_type new_r0) { m_r0 = new_r0; }

  auto stack_size() const { return m_execution_stack.size(); }
  bool stack_empty() const { return m_execution_stack.empty(); }

  execution_value_type r0() const { return m_r0; }

  auto &at_stack(unsigned index) & {
    if (index >= m_execution_stack.size())
      throw std::out_of_range{"Out of range index in at_stack"};
    return m_execution_stack.at(index);
  }

  void set_ip(unsigned new_ip) {
    m_ip = m_program_code.binary_begin();
    std::advance(m_ip, new_ip);
  }

  auto pop() {
    if (m_execution_stack.size() == 0)
      throw vm_error{"Bad stack pop"};
    auto top = m_execution_stack.back();
    m_execution_stack.pop_back();
    return top;
  }

  void push(execution_value_type val) { m_execution_stack.push_back(val); }
  void halt() { m_halted = true; }
  bool is_halted() const { return m_halted; }
  auto constant(unsigned id) const { return m_program_code.constant_at(id); }
};

template <typename... t_instructions> struct instruction_set_description {
  using instruction_variant_type =
      std::variant<std::monostate, const t_instructions *...>;
  using instruction_tuple_type = std::tuple<t_instructions...>;

  static constexpr auto max_table_size =
      std::numeric_limits<opcode_underlying_type>::max() + 1;
  std::array<instruction_variant_type, max_table_size> instruction_lookup_table;

  constexpr instruction_set_description(const t_instructions &...instructions)
      : instruction_lookup_table{} {
    ((instruction_lookup_table[instructions.get_opcode()] =
          instruction_variant_type{std::addressof(instructions)}),
     ...);
  }
};

template <typename t_desc> class virtual_machine {
  t_desc instruction_set;
  context<t_desc> m_execution_context;

public:
  constexpr virtual_machine(t_desc desc)
      : instruction_set{desc}, m_execution_context{} {}

  void set_program_code(chunk ch) { m_execution_context = std::move(ch); }
  bool is_halted() const { return m_execution_context.is_halted(); }

  void execute_instruction() {
    auto &ctx = m_execution_context;

    if (ctx.is_halted())
      throw vm_error{"Can't execute, VM is halted"};
    auto current_instruction =
        instruction_set.instruction_lookup_table[*(m_execution_context.m_ip++)];

    // clang-format off
    std::visit(::utils::visitors{
      [this](std::monostate) {
        m_execution_context.halt();
        throw vm_error{"Unknown opcode"};},
      [&ctx](const auto *instr) {
        auto attr = instr->decode(ctx.m_ip, ctx.m_ip_end).attributes;
        instr->action(ctx, attr); }}, current_instruction);
    // clang-format on
  }

  bool execute() {
    while (!m_execution_context.is_halted()) {
      execute_instruction();
    }

    auto &ctx = m_execution_context;
    return (ctx.m_execution_stack.size() == 0);
  }
};

inline auto read_raw_data(std::istream &is) {
  using iter_type = std::istreambuf_iterator<char>;
  return std::vector<char>{iter_type{is}, iter_type{}};
}

} // namespace paracl::bytecode_vm::decl_vm
