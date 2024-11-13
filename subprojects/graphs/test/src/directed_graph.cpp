// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#include "graphs/directed_graph.hpp"

#include <gtest/gtest.h>

using directed_graph = graphs::basic_directed_graph<int, void, void>;
using graphs::breadth_first_search;

TEST(test_directed_graph, test_insert_vertex) {
  directed_graph A;

  EXPECT_EQ(A.vertices(), 0);
  EXPECT_TRUE(A.insert(1));
  EXPECT_TRUE(A.insert(2));
  EXPECT_FALSE(A.insert(1));

  EXPECT_EQ(A.vertices(), 2);
  EXPECT_TRUE(A.contains(1));
  EXPECT_TRUE(A.contains(2));
  EXPECT_FALSE(A.contains(4));
}

TEST(test_directed_graph, test_insert_edge) {
  directed_graph A;

  EXPECT_TRUE(A.insert(1, 2));
  EXPECT_TRUE(A.insert(1, 3));
  EXPECT_TRUE(A.insert(3, 2));
  EXPECT_TRUE(A.insert(3, 3));
  EXPECT_FALSE(A.insert(1, 2));

  EXPECT_EQ(A.edges(), 4);
  EXPECT_EQ(A.vertices(), 3);

  EXPECT_TRUE(A.contains(1));
  EXPECT_TRUE(A.contains(2));
  EXPECT_TRUE(A.contains(3));

  EXPECT_TRUE(A.connected(1, 2));
  EXPECT_TRUE(A.connected(1, 3));
  EXPECT_TRUE(A.connected(3, 2));
  EXPECT_TRUE(A.connected(3, 3));
}

TEST(test_directed_graph, test_connected) {
  directed_graph A;

  A.insert(3, 6);
  A.insert(3, 5);
  A.insert(5, 4);
  A.insert(4, 2);
  A.insert(2, 5);
  A.insert(1, 2);
  A.insert(1, 4);
  A.insert(1, 1);

  EXPECT_TRUE(A.connected(3, 5));
  EXPECT_TRUE(A.connected(1, 2));
  EXPECT_TRUE(A.connected(1, 1));
  EXPECT_FALSE(A.connected(1, 6));
  EXPECT_FALSE(A.connected(1, 3));
  EXPECT_FALSE(A.connected(5, 3));
}

TEST(test_directed_graph, test_custom_hash_connected) {
  const auto hash_int = [](int v) { return static_cast<unsigned>(v); };
  graphs::basic_directed_graph<int, void, void, decltype(hash_int)> A;

  A.insert(3, 6);
  A.insert(3, 5);
  A.insert(5, 4);
  A.insert(4, 2);
  A.insert(2, 5);
  A.insert(1, 2);
  A.insert(1, 4);
  A.insert(1, 1);

  EXPECT_TRUE(A.connected(3, 5));
  EXPECT_TRUE(A.connected(1, 2));
  EXPECT_TRUE(A.connected(1, 1));
  EXPECT_FALSE(A.connected(1, 6));
  EXPECT_FALSE(A.connected(1, 3));
  EXPECT_FALSE(A.connected(5, 3));
}

TEST(test_directed_graph, test_reachable) {
  directed_graph A;

  A.insert(3, 6);
  A.insert(3, 5);
  A.insert(5, 4);
  A.insert(4, 2);
  A.insert(2, 5);
  A.insert(1, 2);
  A.insert(1, 4);
  A.insert(1, 1);

  EXPECT_TRUE(A.reachable(3, 6));
  EXPECT_TRUE(A.reachable(3, 2));
  EXPECT_FALSE(A.reachable(3, 1));
}

TEST(test_directed_graph, test_BFT_search) {
  directed_graph A;

  A.insert(3, 6);
  A.insert(3, 5);
  A.insert(5, 4);
  A.insert(4, 2);
  A.insert(2, 5);
  A.insert(1, 2);
  A.insert(1, 4);

  const auto bfs = [&A](auto &&key, auto &&func) {
    return breadth_first_search(A, key, func);
  };

  EXPECT_TRUE(bfs(3, [](auto &&node) { return node->key == 2; }));
  EXPECT_TRUE(bfs(2, [](auto &&node) { return node->key == 5; }));
  EXPECT_FALSE(bfs(2, [](auto &&node) { return node->key == 3; }));
  EXPECT_FALSE(bfs(4, [](auto &&node) { return node->key == 11; }));
}

TEST(test_directed_graph, test_BFT_schedule) {
  directed_graph A;

  A.insert(3, 6);
  A.insert(3, 5);
  A.insert(5, 4);
  A.insert(4, 2);
  A.insert(2, 5);
  A.insert(1, 2);
  A.insert(1, 4);

  const auto bfs = [&A](auto &&key, auto &&func) {
    return breadth_first_search(A, key, func);
  };
  std::vector<int> res, ans = {3, 6, 5, 4, 2};
  bfs(3, [&res](auto &&node) { res.push_back(node->key); });
  EXPECT_EQ(res, ans);
}

TEST(test_directed_graph, test_custom_hash_BFT_search) {
  const auto hash_int = [](int v) { return static_cast<unsigned>(v); };
  graphs::basic_directed_graph<int, void, void, decltype(hash_int)> A;

  A.insert(3, 6);
  A.insert(3, 5);
  A.insert(5, 4);
  A.insert(4, 2);
  A.insert(2, 5);
  A.insert(1, 2);
  A.insert(1, 4);

  const auto bfs = [&A](auto &&key, auto &&func) {
    return breadth_first_search(A, key, func);
  };
  std::vector<int> res;
  bfs(3, [&res](auto &&node) { res.push_back(node->key); });

  std::vector<int> ans = {3, 6, 5, 4, 2};
  EXPECT_EQ(res, ans);
}

TEST(test_directed_graph, test_topological_sort) {
  directed_graph A;

  A.insert(1, 2);
  A.insert(1, 3);
  A.insert(1, 4);
  A.insert(2, 4);
  A.insert(4, 3);

  const auto res = graphs::recursive_topo_sort(A);
  std::vector ans = {3, 4, 2, 1};

  const auto match =
      std::equal(res.begin(), res.end(), ans.begin(), ans.end(),
                 [](auto &&lhs, auto &&rhs) { return lhs.key == rhs; });

  EXPECT_TRUE(match);
}

TEST(test_directed_graph, test_custom_hash_topological_sort) {
  const auto hash_int = [](int v) { return static_cast<unsigned>(v); };
  graphs::basic_directed_graph<int, void, void, decltype(hash_int)> A;

  A.insert(1, 2);
  A.insert(1, 3);
  A.insert(1, 4);
  A.insert(2, 4);
  A.insert(4, 3);

  const auto res = graphs::recursive_topo_sort(A);
  std::vector ans = {3, 4, 2, 1};

  const auto match =
      std::equal(res.begin(), res.end(), ans.begin(), ans.end(),
                 [](auto &&lhs, auto &&rhs) { return lhs.key == rhs; });

  EXPECT_TRUE(match);
}

TEST(test_directed_graph, test_edge_attributes) {
  graphs::basic_directed_graph<std::string, int, int> A;

  EXPECT_TRUE(A.insert({"1", 1}));
  EXPECT_TRUE(A.insert({"2", 0}));

  EXPECT_TRUE(A.create_link("1", "2", -5));
  EXPECT_TRUE(A.connected("1", "2"));
}

TEST(test_directed_graph, test_attributes_1) {
  graphs::basic_directed_graph<std::string, int, int> A;

  EXPECT_TRUE(A.insert({"1", 1}));
  EXPECT_TRUE(A.insert({"2", 0}));

  EXPECT_TRUE(A.create_link("1", "2", -5));
  EXPECT_TRUE(A.connected("1", "2"));

  auto found = A.find("1");
  auto [key, attr] = *found->second;
  EXPECT_EQ(key, "1");
  EXPECT_EQ(attr, 1);

  using graphs::breadth_first_search;
  EXPECT_TRUE(breadth_first_search(
      A, "1", [](auto &&node) -> bool { return node->key == "2"; }));
}

TEST(test_directed_graph, test_attributes_2) {
  graphs::basic_directed_graph<std::string, int, void> A;

  EXPECT_TRUE(A.insert({"1", 1}));
  EXPECT_TRUE(A.insert({"2", 0}));
  EXPECT_TRUE(A.insert({"3", 2}));

  EXPECT_TRUE(A.create_link("1", "2"));
  EXPECT_TRUE(A.create_link("2", "3"));
  EXPECT_TRUE(A.connected("1", "2"));

  auto found = A.find("1");
  auto [key, attr] = *found->second;
  EXPECT_EQ(key, "1");
  EXPECT_EQ(attr, 1);

  using graphs::breadth_first_search;
  EXPECT_TRUE(breadth_first_search(
      A, "1", [](auto &&node) -> bool { return node->attr == 2; }));
}

TEST(test_directed_graph, test_attributes_3) {
  graphs::basic_directed_graph<std::string, void, char> A;

  EXPECT_TRUE(A.insert("1"));
  EXPECT_TRUE(A.insert("2"));

  EXPECT_TRUE(A.create_link("1", "2", 'c'));
  EXPECT_TRUE(A.connected("1", "2"));

  auto found = A.find("1");
  auto key = found->second->key;
  EXPECT_EQ(key, "1");

  using graphs::breadth_first_search;
  EXPECT_TRUE(breadth_first_search(
      A, "1", [](auto &&node) -> bool { return node->key == "2"; }));
}
