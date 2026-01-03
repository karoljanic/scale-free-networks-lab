#include "test_graph.hpp"

#include <iostream>

int main(int /*argc*/, char* /*argv*/[]) {
  graph::TestGraph k3;

  const size_t node0 = k3.addNode();
  const size_t node1 = k3.addNode();
  const size_t node2 = k3.addNode();
  const size_t node3 = k3.addNode();

  k3.addEdge(node0, node1);
  k3.addEdge(node1, node2);
  k3.addEdge(node2, node0);

  std::cout << "Graph k3 has " << k3.getNodesNumber() << " nodes and " << k3.getEdgesNumber() << " edges and " << k3.getDensity()
            << " density" << std::endl;
  std::cout << "Edge (0,1) exists: " << k3.edgeExists(node0, node1) << std::endl;
  std::cout << "Edge (1,3) exists: " << k3.edgeExists(node1, node3) << std::endl;

  std::cout << "Get node0: " << k3.getNode(node0).id << std::endl;
  std::cout << "Get last added node: " << k3.getLastAddedNode().id << std::endl;
  std::cout << "Nodes: ";
  for (const auto& node : k3.getNodes()) {
    std::cout << node.id << " ";
  }
  std::cout << std::endl;

  std::cout << "Get edge (node0,node1): (" << k3.getEdge(node0, node1).source << "," << k3.getEdge(node0, node1).target << ")"
            << std::endl;
  std::cout << "Get last added edge: (" << k3.getLastAddedEdge().source << "," << k3.getLastAddedEdge().target << ")"
            << std::endl;
  std::cout << "Edges: ";
  for (const auto& edge : k3.getEdges()) {
    std::cout << "(" << edge.source << "," << edge.target << ") ";
  }
  std::cout << std::endl;

  std::cout << "Degree of node0: " << k3.getDegree(node0) << std::endl;
  std::cout << "Degrees histogram: ";
  for (const auto& [degree, count] : k3.getDegreesHistogram()) {
    std::cout << "(" << degree << "," << count << ") ";
  }
  std::cout << std::endl;

  std::cout << "Neighbours of node0: ";
  for (const auto& node : k3.getNeighbours(node0)) {
    std::cout << node.id << " ";
  }
  std::cout << std::endl;

  std::cout << "Adjacent edges of node0: ";
  for (const auto& edge : k3.getAdjacentEdges(node0)) {
    std::cout << "(" << edge.source << "," << edge.target << ") ";
  }
  std::cout << std::endl;

  k3.removeEdge(node0, node1);
  std::cout << "Edge (0,1) removed. Edge (0,1) exists: " << k3.edgeExists(node0, node1) << std::endl;

  return 0;
}