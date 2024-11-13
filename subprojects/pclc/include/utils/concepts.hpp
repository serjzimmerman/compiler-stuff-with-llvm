// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include <concepts>
#include <fstream>
#include <string_view>

namespace utils {

// clang-format off
template <typename T>
concept is_ifstream = requires (T stream) {
  { [] <typename CharType, typename Traits> (std::basic_ifstream<CharType, Traits> &) {} (stream) };
};

template <typename T>
concept is_ofstream = requires (T stream) {
  { [] <typename CharType, typename Traits> (std::basic_ofstream<CharType, Traits> &) {} (stream) };
};

template <typename T>
concept is_fstream = requires (std::remove_cvref_t<T> stream) {
  requires is_ifstream<T> || is_ofstream<T>;
};
// clang-format on

template <typename T>
concept integral_or_floating = std::integral<T> || std::floating_point<T>;

// clang-format off
template <typename T, typename D>
concept coercible_to = requires(T object) {
  { static_cast<D>(object) };
};
// clang-format on

// clang-format off
template <typename T>
concept convertible_to_string_view = coercible_to<T, std::string_view>;

} // namespace utils