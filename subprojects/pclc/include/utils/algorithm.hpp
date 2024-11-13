// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include <concepts>
#include <iterator>

namespace utils {

template <std::input_iterator input_it>
input_it copy_while(
    input_it first, input_it last,
    std::output_iterator<
        typename std::iterator_traits<input_it>::value_type> auto o_first,
    std::invocable<typename std::iterator_traits<input_it>::value_type> auto
        pred) {
  for (; first != last; ++first) {
    if (!pred(*first))
      break;
    *o_first = *first;
    ++o_first;
  }
  return first;
}

} // namespace utils
