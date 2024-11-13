// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "graphs/dag.hpp"

#include <gtest/gtest.h>

using dag = graphs::dag<int>;

TEST(test_dag, test_insert) {
  dag A;

  A.insert(1);
  A.insert(2);

  EXPECT_TRUE(A.insert(1, 2));
  EXPECT_TRUE(A.insert(1, 3));
  EXPECT_TRUE(A.insert(1, 4));
  EXPECT_TRUE(A.insert(2, 4));
  EXPECT_TRUE(A.insert(4, 3));

  EXPECT_EQ(A.edges(), 5);
  EXPECT_EQ(A.vertices(), 4);

  EXPECT_TRUE(A.contains(1));
  EXPECT_TRUE(A.contains(2));
  EXPECT_TRUE(A.contains(3));
  EXPECT_TRUE(A.contains(4));
  EXPECT_FALSE(A.contains(11));

  EXPECT_TRUE(A.connected(1, 2));
  EXPECT_TRUE(A.connected(1, 3));
  EXPECT_TRUE(A.connected(1, 4));
  EXPECT_FALSE(A.connected(3, 2));
}
