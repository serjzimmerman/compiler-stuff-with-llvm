// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "decl_vm.hpp"
#include "utils/files.hpp"
#include "utils/misc.hpp"

#include <array>
#include <concepts>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

namespace paracl::bytecode_vm::builder {

template <typename t_desc> struct encoded_instruction {
  using attribute_types = typename t_desc::attribute_types;
  using description_type = t_desc;

  attribute_types m_attr;

  template <auto I>
  void encode_attributes(std::output_iterator<char> auto iter) const {
    ::utils::write_little_endian<std::tuple_element_t<I, attribute_types>>(
        std::get<I>(m_attr), iter);
  }

  // [[maybe_unused]] to silence false-positive errors by GCC
  template <auto... I>
  void encode_attributes([[maybe_unused]] std::output_iterator<char> auto iter,
                         std::index_sequence<I...>) const {
    (encode_attributes<I>(iter), ...);
  }

public:
  auto get_size() const { return t_desc::get_size(); }
  auto get_opcode() const { return t_desc::get_opcode(); }

  void encode(std::output_iterator<char> auto iter) const {
    *iter = t_desc::get_opcode();
    auto seq = std::make_index_sequence<std::tuple_size_v<attribute_types>>{};
    encode_attributes(iter, seq);
  }

  encoded_instruction(t_desc)
    requires(std::tuple_size_v<attribute_types> == 0)
  {}

  template <typename... Ts>
  encoded_instruction(t_desc, Ts &&...p_args)
      : m_attr{std::forward<Ts>(p_args)...} {}
};

template <typename t_tuple> struct encoded_tuple_from_desc_tuple;
template <typename... Ts>
struct encoded_tuple_from_desc_tuple<std::tuple<Ts...>> {
  using type =
      std::tuple<encoded_instruction<typename Ts::description_type>...>;
};

template <typename t_tuple>
using encoded_tuple_from_desc_tuple_t =
    typename encoded_tuple_from_desc_tuple<t_tuple>::type;

template <typename t_instruction_set> class bytecode_builder {
public:
  using instruction_variant_type =
      ::utils::variant_from_tuple_t<encoded_tuple_from_desc_tuple_t<
          typename t_instruction_set::instruction_tuple_type>>;

private:
  using instruction_vec = std::vector<instruction_variant_type>;
  instruction_vec m_code;
  unsigned m_cur_loc = 0;

public:
  bytecode_builder() = default;

  template <typename T>
  unsigned emit_operation(encoded_instruction<T> instruction) {
    m_code.push_back(instruction_variant_type{instruction});
    m_cur_loc += instruction.get_size();
    return m_code.size() - 1;
  }

  unsigned emit_operation(auto description) {
    return emit_operation(encoded_instruction{description});
  }

  template <typename as_desc> auto &get_as(as_desc, std::size_t index) & {
    return std::get<encoded_instruction<as_desc>>(m_code.at(index));
  }

  decl_vm::chunk to_chunk() const {
    decl_vm::chunk ch;

    std::output_iterator<char> auto iter = std::back_inserter(ch);
    for (const auto &v : m_code) {
      std::visit([iter](auto &&var) { var.encode(iter); }, v);
    }

    return ch;
  }

  auto current_loc() const { return m_cur_loc; }
};

} // namespace paracl::bytecode_vm::builder
