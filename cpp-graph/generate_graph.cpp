#include <fstream>
#include <iostream>

#include "graph_factory.hpp"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <graph_type> <output_file> <arg1> ...\n";
    return 1;
  }

  const int graph_type = std::stoi(argv[1]);
  const std::string output_filename = argv[2];
  std::string command_args = argv[0];
  int command_extra_args_count = 0;

  if (graph_type == 1) {  // Erdos-Renyi
    if (argc != 5) {
      command_args += " 1 <output_file> <final_nodes> <edge_probability>";
      std::cerr << "Usage: " << command_args << "\n";
      return 1;
    }

    const size_t final_nodes = std::stoul(argv[3]);
    const double edge_probability = std::stod(argv[4]);

    graph::GraphFactory factory;
    graph::ERGraph graph = factory.generateERGraph(final_nodes, edge_probability);

    std::fstream output_file(output_filename, std::ios::out);
    for (const auto& edge : graph.getEdges()) {
      output_file << edge.source << " " << edge.target << "\n";
    }
  }
  else if (graph_type == 2) {  // standard BA
    if (argc != 6) {
      command_args += " 2 <output_file> <initial_nodes> <final_nodes> <edges_per_node>";
      std::cerr << "Usage: " << command_args << "\n";
      return 1;
    }

    const size_t initial_nodes = std::stoul(argv[3]);
    const size_t final_nodes = std::stoul(argv[4]);
    const size_t edges_per_node = std::stoul(argv[5]);

    graph::GraphFactory factory;
    graph::BAGraph graph = factory.generateBAGraph(initial_nodes, final_nodes, edges_per_node);

    std::fstream output_file(output_filename, std::ios::out);
    for (const auto& edge : graph.getEdges()) {
      output_file << edge.source << " " << edge.target << "\n";
    }
  }
  else if (graph_type == 3) {  // node and edge distributed BA
    if (argc < 8) {
      command_args +=
          " 3 <output_file> <initial_nodes> <final_nodes> <edges_per_node> <node_prob_dist_type> <edge_prob_dist_type>";
      std::cerr << "Usage: " << command_args << " <node_prob_dist_params> <edge_prob_dist_params>\n";
      return 1;
    }

    const size_t initial_nodes = std::stoul(argv[3]);
    const size_t final_nodes = std::stoul(argv[4]);
    const size_t edges_per_node = std::stoul(argv[5]);
    const int node_prob_dist_type = std::stoi(argv[6]);
    const int edge_prob_dist_type = std::stoi(argv[7]);

    std::unique_ptr<Distribution> node_dist;
    if (node_prob_dist_type == 0) {
      node_dist = std::make_unique<ConstantDistribution>(1.0);
    }
    else if (node_prob_dist_type == 1) {
      if (argc < 9) {
        command_args += " <node_prob_dist_prob>";
        command_extra_args_count = 1;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double prob = std::stod(argv[8]);
      node_dist = std::make_unique<ConstantDistribution>(prob);
    }
    else if (node_prob_dist_type == 2) {
      node_dist = std::make_unique<UniformDistribution>();
    }
    else if (node_prob_dist_type == 3) {
      node_dist = std::make_unique<ProportionalDistribution>();
    }
    else if (node_prob_dist_type == 4) {
      if (argc < 9) {
        command_args += " <node_prob_dist_exponent>";
        command_extra_args_count = 1;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double exponent = std::stod(argv[8]);
      node_dist = std::make_unique<PowerDistribution>(exponent);
    }
    else if (node_prob_dist_type == 5) {
      if (argc < 10) {
        command_args += " <node_prob_dist_mean> <node_prob_dist_stddev>";
        command_extra_args_count = 2;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double mean = std::stod(argv[8]);
      const double stddev = std::stod(argv[9]);
      node_dist = std::make_unique<NormalDistribution>(mean, stddev);
    }
    else if (node_prob_dist_type == 6) {
      if (argc < 10) {
        command_args += " <node_prob_dist_mean> <node_prob_dist_stddev>";
        command_extra_args_count = 2;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double mean = std::stod(argv[8]);
      const double stddev = std::stod(argv[9]);
      node_dist = std::make_unique<LogNormalDistribution>(mean, stddev);
    }
    else if (node_prob_dist_type == 7) {
      if (argc < 9) {
        command_args += " <node_prob_dist_lambda>";
        command_extra_args_count = 1;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double lambda = std::stod(argv[8]);
      node_dist = std::make_unique<ExponentialDistribution>(lambda);
    }
    else if (node_prob_dist_type == 8) {
      if (argc < 9) {
        command_args += " <node_prob_dist_base>";
        command_extra_args_count = 1;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double base = std::stod(argv[8]);
      node_dist = std::make_unique<LogDistribution>(base);
    }
    else if (node_prob_dist_type == 9) {
      if (argc < 10) {
        command_args += " <node_prob_dist_shape> <node_prob_dist_scale>";
        command_extra_args_count = 2;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double shape = std::stod(argv[8]);
      const double scale = std::stod(argv[9]);
      node_dist = std::make_unique<WeibullDistribution>(shape, scale);
    }
    else if (node_prob_dist_type == 10) {
      if (argc < 10) {
        command_args += " <node_prob_dist_shape> <node_prob_dist_scale>";
        command_extra_args_count = 2;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double shape = std::stod(argv[8]);
      const double scale = std::stod(argv[9]);
      node_dist = std::make_unique<GammaDistribution>(shape, scale);
    }
    else if (node_prob_dist_type == 11) {
      if (argc < 9) {
        command_args += " <node_prob_dist_freedom_degrees>";
        command_extra_args_count = 1;
        std::cerr << "Usage: " << command_args << " <edge_prob_dist_params>\n";
        return 1;
      }

      const double freedom_degrees = std::stod(argv[8]);
      node_dist = std::make_unique<ChiSquaredDistribution>(freedom_degrees);
    }
    else {
      std::cerr << "Undefined node probability distribution type!\n";
      return 1;
    }

    std::unique_ptr<Distribution> edge_dist;
    if (edge_prob_dist_type == 0) {
      edge_dist = std::make_unique<ConstantDistribution>(1.0);
    }
    else if (edge_prob_dist_type == 1) {
      if (argc < 9 + command_extra_args_count) {
        command_args += " <edge_prob_dist_prob>";
        std::cerr << "Usage: " << command_args << "\n";
        return 1;
      }

      const double prob = std::stod(argv[8 + command_extra_args_count]);
      edge_dist = std::make_unique<ConstantDistribution>(prob);
    }
    else if (edge_prob_dist_type == 2) {
      edge_dist = std::make_unique<UniformDistribution>();
    }
    else if (edge_prob_dist_type == 3) {
      if (argc < 11 + command_extra_args_count) {
        command_args += " <edge_prob_dist_a> <edge_prob_dist_b> <edge_prob_dist_c>";
        std::cerr << "Usage: " << command_args << "\n";
        return 1;
      }

      const double a = std::stod(argv[8 + command_extra_args_count]);
      const double b = std::stod(argv[9 + command_extra_args_count]);
      const double c = std::stod(argv[10 + command_extra_args_count]);
      edge_dist = std::make_unique<ConstantDistribution>([a, b, c](size_t k) { return a / pow(k, b) + c; });
    }
    else if (edge_prob_dist_type == 4) {
      if (argc < 11 + command_extra_args_count) {
        command_args += " <edge_prob_dist_a> <edge_prob_dist_b> <edge_prob_dist_c>";
        std::cerr << "Usage: " << command_args << "\n";
        return 1;
      }

      const double a = std::stod(argv[8 + command_extra_args_count]);
      const double b = std::stod(argv[9 + command_extra_args_count]);
      const double c = std::stod(argv[10 + command_extra_args_count]);
      edge_dist = std::make_unique<ConstantDistribution>([a, b, c](size_t k) { return a / pow(log(k), b) + c; });
    }
    else {
      std::cerr << "Undefined edge probability distribution type!\n";
      return 1;
    }

    graph::GraphFactory factory;
    graph::BAGraph graph = factory.generateBAVariateGraph(initial_nodes, final_nodes, edges_per_node, node_dist, edge_dist);

    std::fstream output_file(output_filename, std::ios::out);
    for (const auto& edge : graph.getEdges()) {
      output_file << edge.source << " " << edge.target << "\n";
    }
  }
  else {
    std::cerr << "Undefined graph type!\n";
    return 1;
  }

  return 0;
}
