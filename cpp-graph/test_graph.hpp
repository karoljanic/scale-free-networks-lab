#ifndef GRAPH_TEST_GRAPH_HPP
#define GRAPH_TEST_GRAPH_HPP

#include <cstring>

#include "sparse_graph.hpp"

namespace graph {

struct TestNode {
  size_t id{0};
};

struct TestEdge {
  size_t source{0};
  size_t target{0};
};

typedef SparseGraph<TestNode, TestEdge> TestGraph;
}  // namespace graph

#endif  // GRAPH_TEST_GRAPH_HPP
