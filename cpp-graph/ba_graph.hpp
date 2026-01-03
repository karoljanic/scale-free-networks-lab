#ifndef GRAPH_BA_GRAPH_HPP
#define GRAPH_BA_GRAPH_HPP

#include <cstring>

#include "sparse_graph.hpp"

namespace graph {

struct BANode {
  size_t id{0};
};

struct BAEdge {
  size_t source{0};
  size_t target{0};
};

typedef SparseGraph<BANode, BAEdge> BAGraph;

}  // namespace graph

#endif  // GRAPH_BA_GRAPH_HPP