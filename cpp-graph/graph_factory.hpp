#ifndef GRAPH_GRAPH_FACTORY_HPP
#define GRAPH_GRAPH_FACTORY_HPP

#include <functional>
#include <memory>
#include <random>
#include <vector>

#include "ba_graph.hpp"
#include "distribution.hpp"
#include "er_graph.hpp"

namespace graph {

class GraphFactory {
 public:
  GraphFactory() = default;

  GraphFactory(const GraphFactory&) = default;
  GraphFactory(GraphFactory&&) = default;

  GraphFactory& operator=(const GraphFactory&) = default;
  GraphFactory& operator=(GraphFactory&&) = default;

  ~GraphFactory() = default;

  [[nodiscard]] ERGraph generateERGraph(size_t final_nodes_number, double edge_probability);
  [[nodiscard]] BAGraph generateBAGraph(size_t initial_nodes_number, size_t final_nodes_number, size_t edges_per_new_node_number);
  [[nodiscard]] BAGraph generateBAVariateGraph(size_t initial_nodes_number, size_t final_nodes_number,
                                               size_t edges_per_new_node_number, std::unique_ptr<Distribution>& node_prob_dist,
                                               std::unique_ptr<Distribution>& edge_prob_dist);

 private:
  std::mt19937 generator_{std::random_device{}()};
};

}  // namespace graph

#endif  // GRAPH_GRAPH_FACTORY_HPP