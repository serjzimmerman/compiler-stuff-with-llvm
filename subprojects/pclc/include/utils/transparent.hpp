// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "utils/concepts.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace utils::transparent {

struct string_equal {
  using is_transparent = void;
  bool operator()(const convertible_to_string_view auto &lhs,
                  const convertible_to_string_view auto &rhs) const {
    return static_cast<std::string_view>(lhs) ==
           static_cast<std::string_view>(rhs);
  }
};

struct string_hash {
  using is_transparent = string_equal;
  bool operator()(const convertible_to_string_view auto &val) const {
    return std::hash<std::string_view>{}(val);
  }
};

template <typename T>
using string_unordered_map =
    std::unordered_map<std::string, T, string_hash, string_equal>;

} // namespace utils::transparent
