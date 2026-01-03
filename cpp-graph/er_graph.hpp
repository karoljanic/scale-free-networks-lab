#ifndef GRAPH_ER_GRAPH_HPP
#define GRAPH_ER_GRAPH_HPP

#include <cstring>

#include "sparse_graph.hpp"

namespace graph {

struct ERNode {
  size_t id{0};
};

struct EREdge {
  size_t source{0};
  size_t target{0};
};

typedef SparseGraph<ERNode, EREdge> ERGraph;

}  // namespace graph

#endif  // GRAPH_ER_GRAPH_HPP