// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "ezvis/ezvis.hpp"

#include <gtest/gtest.h>

namespace {

struct i_base : ezvis::visitable_base<i_base> {
  virtual ~i_base() {}
};

struct derived1 : i_base {
  static constexpr auto msg = "hello from derived1";
  EZVIS_VISITABLE();
};

struct derived2 : i_base {
  static constexpr auto msg = "hello from derived2";
  EZVIS_VISITABLE();
};

TEST(test_ezvis, test_lambda_visit) {
  std::unique_ptr<i_base> base;
  base = std::make_unique<derived1>();

  const auto get_str = [](const auto &base) {
    auto res = ezvis::visit<std::string, derived1, derived2>(
        [](const auto &type) {
          return std::remove_reference_t<decltype(type)>::msg;
        },
        *base.get());
    return res;
  };

  auto res = get_str(base);
  EXPECT_EQ(res, derived1::msg);

  base = std::make_unique<derived2>();
  res = get_str(base);
  EXPECT_EQ(res, derived2::msg);
}

} // namespace
