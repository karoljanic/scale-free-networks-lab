#include "graph_factory.hpp"

#include <set>
#include <vector>

namespace graph {

ERGraph GraphFactory::generateERGraph(size_t final_nodes_number, double edge_probability) {
  if (final_nodes_number < 1) {
    throw std::invalid_argument("Final nodes number must be greater than 0");
  }

  if (edge_probability < 0.0 || edge_probability > 1.0) {
    throw std::invalid_argument("Edge probability must be between 0 and 1");
  }

  ERGraph graph;

  // add nodes to graph
  for (size_t node = 0; node < final_nodes_number; ++node) {
    graph.addNode();  // expected consecutive node ids
  }

  // add edges to graph
  std::uniform_real_distribution<double> dist{0.0, 1.0};
  for (size_t source = 0; source < final_nodes_number; ++source) {
    for (size_t target = source + 1; target < final_nodes_number; ++target) {
      if (dist(generator_) < edge_probability) {
        graph.addEdge(source, target);
      }
    }
  }

  return graph;
}

BAGraph GraphFactory::generateBAGraph(size_t initial_nodes_number, size_t final_nodes_number, size_t edges_per_new_node_number) {
  if (edges_per_new_node_number < 1 || edges_per_new_node_number >= final_nodes_number) {
    throw std::invalid_argument("Edges per new node number must be greater than 0 and less than final nodes number");
  }

  if (initial_nodes_number < 1 || initial_nodes_number > final_nodes_number) {
    throw std::invalid_argument("Initial nodes number must be greater than 0 and less than final nodes number");
  }

  if (initial_nodes_number < edges_per_new_node_number) {
    throw std::invalid_argument("Initial nodes number must be greater than or equal to edges per new node number");
  }

  if ((final_nodes_number - initial_nodes_number) % edges_per_new_node_number != 0) {
    throw std::invalid_argument("The number of edges per new node must evenly divide the number of new nodes to be added");
  }

  std::vector<size_t> links;
  links.reserve(2 * initial_nodes_number + 2 * (final_nodes_number - initial_nodes_number) * edges_per_new_node_number);

  // initialize n0 connected nodes
  for (size_t node = 0; node < initial_nodes_number; ++node) {
    links.push_back(node);
    links.push_back((node + 1) % initial_nodes_number);
  }

  // add new nodes up to final_nodes_number
  for (size_t node = initial_nodes_number; node < final_nodes_number; ++node) {
    std::uniform_int_distribution<size_t> dist{0, links.size() - 1};
    std::set<size_t> new_neighbors;
    while (new_neighbors.size() < edges_per_new_node_number) {
      size_t link_index = dist(generator_);
      size_t neighbor = links[link_index];
      new_neighbors.insert(neighbor);
    }

    for (const auto neighbor : new_neighbors) {
      links.push_back(node);
      links.push_back(neighbor);
    }
  }

  BAGraph graph;

  // add nodes to graph
  for (size_t node = 0; node < final_nodes_number; ++node) {
    graph.addNode();  // expected consecutive node ids
  }

  // add links to graph
  for (size_t node = 0; node < links.size() / 2; ++node) {
    graph.addEdge(links[2 * node], links[2 * node + 1]);
  }

  return graph;
}

BAGraph GraphFactory::generateBAVariateGraph(size_t initial_nodes_number, size_t final_nodes_number,
                                             size_t edges_per_new_node_number, std::unique_ptr<Distribution>& node_prob_dist,
                                             std::unique_ptr<Distribution>& edge_prob_dist) {
  if (edges_per_new_node_number < 1 || edges_per_new_node_number >= final_nodes_number) {
    throw std::invalid_argument("Edges per new node number must be greater than 0 and less than final nodes number");
  }

  if (initial_nodes_number < 1 || initial_nodes_number > final_nodes_number) {
    throw std::invalid_argument("Initial nodes number must be greater than 0 and less than final nodes number");
  }

  if (initial_nodes_number < edges_per_new_node_number) {
    throw std::invalid_argument("Initial nodes number must be greater than or equal to edges per new node number");
  }

  // if ((final_nodes_number - initial_nodes_number) % edges_per_new_node_number != 0) {
  //   throw std::invalid_argument("The number of edges per new node must evenly divide the number of new nodes to be added");
  // }

  BAGraph graph;
  std::vector<size_t> degrees;
  degrees.reserve(final_nodes_number);

  // initialize n0 connected nodes
  for (size_t node = 0; node < initial_nodes_number; ++node) {
    graph.addNode();       // expected consecutive node ids
    degrees.push_back(2);  // each node has degree 2
  }

  for (size_t node = 0; node < initial_nodes_number; ++node) {
    graph.addEdge(node, (node + 1) % initial_nodes_number);
  }

  // add new nodes up to final_nodes_number
  for (size_t node = initial_nodes_number; node < final_nodes_number; ++node) {
    node_prob_dist->update(degrees);
    edge_prob_dist->update(degrees);

    graph.addNode();
    degrees.push_back(0);

    std::vector<double> node_probs;
    node_probs.reserve(graph.getNodes().size());
    for (size_t i = 0; i < graph.getNodes().size(); ++i) {
      node_probs.push_back((*node_prob_dist)(degrees[i]));
    }

    std::discrete_distribution<> node_dist{node_probs.begin(), node_probs.end()};
    std::set<size_t> new_neighbors;
    while (new_neighbors.size() < edges_per_new_node_number) {
      new_neighbors.insert(node_dist(generator_));
    }

    for (const auto neighbor : new_neighbors) {
      std::uniform_real_distribution<double> rand_dist{0.0, 1.0};
      const auto rand = rand_dist(generator_);
      if (rand > (*edge_prob_dist)(degrees[neighbor])) {
        continue;
      }

      graph.addEdge(node, neighbor);
      degrees[node]++;
      degrees[neighbor]++;
    }
  }

  return graph;
}

}  // namespace graph
