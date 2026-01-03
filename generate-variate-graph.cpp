#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
#include <random>


void save_graph(const std::set<std::pair<size_t, size_t>>& edges, std::ofstream& outfile) {
    for(const auto& edge : edges) {
        outfile << edge.first + 1<< " " << edge.second + 1 << "\n";
    }
}

void merge_edges(const std::vector<size_t>& edges, size_t m, std::set<std::pair<size_t, size_t>>& merged_edges) {
    for(size_t i = 0; i < edges.size(); i += 2) {
        size_t u = edges[i] / m;
        size_t v = edges[i + 1] / m;

        if(u == v) { // prevent self-loops
            continue;
        }

        if(u > v) {
            std::swap(u, v);
        }
        merged_edges.insert({u, v});
    }
}

void linear_preferential_attachment(size_t n, size_t m, double p, std::ofstream& outfile) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    std::vector<size_t> edges;
    edges.reserve(2 * n * m);
    edges.push_back(0);
    
    for(size_t v = 1; v < n * m; ++v) {
        if(prob_dist(gen) > p) {
            continue;
        }

        std::uniform_int_distribution<size_t> edge_dist(0, edges.size() - 1);
        size_t random_edge_index = edge_dist(gen);
        size_t target = edges[random_edge_index];

        edges.push_back(v);
        edges.push_back(target);
    }

    std::set<std::pair<size_t, size_t>> merged_edges;
    merge_edges(edges, m, merged_edges);

    save_graph(merged_edges, outfile);
}

void power_preferential_attachment(size_t n, size_t m, double alpha, double p, std::ofstream& outfile) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    std::vector<size_t> edges;
    edges.reserve(2 * n * m);

    std::unordered_map<size_t, size_t> degree_count;
    degree_count[0] = 1;

    for(size_t v = 1; v < n * m; ++v) {
        if(prob_dist(gen) > p) {
            continue;
        }

        // select target based on power-law distribution
        double total_weight = 0.0;
        for(const auto& [node, degree] : degree_count) {
            total_weight += std::pow(degree, alpha);
        }

        std::uniform_real_distribution<double> weight_dist(0.0, total_weight);
        double random_weight = weight_dist(gen);
        size_t target = 0;
        double cumulative_weight = 0.0;
        for(const auto& [node, degree] : degree_count) {
            cumulative_weight += std::pow(degree, alpha);
            if(cumulative_weight >= random_weight) {
                target = node;
                break;
            }
        }

        edges.push_back(v);
        edges.push_back(target);
        degree_count[target]++;
        degree_count[v] = 1;
    }

    std::set<std::pair<size_t, size_t>> merged_edges;
    merge_edges(edges, m, merged_edges);

    save_graph(merged_edges, outfile);
}

void log_based_preferential_attachment(size_t n, size_t m, double p, std::ofstream& outfile) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    std::vector<size_t> edges;
    edges.reserve(2 * n * m);

    std::unordered_map<size_t, size_t> degree_count;
    degree_count[0] = 1;

    for(size_t v = 1; v < n * m; ++v) {
        if(prob_dist(gen) > p) {
            continue;
        }

        // select target based on logarithmic distribution
        double total_weight = 0.0;
        for(const auto& [node, degree] : degree_count) {
            total_weight += std::log(degree + 1); // +1 to avoid log(0)
        }

        std::uniform_real_distribution<double> weight_dist(0.0, total_weight);
        double random_weight = weight_dist(gen);
        size_t target = 0;
        double cumulative_weight = 0.0;
        for(const auto& [node, degree] : degree_count) {
            cumulative_weight += std::log(degree + 1);
            if(cumulative_weight >= random_weight) {
                target = node;
                break;
            }
        }

        edges.push_back(v);
        edges.push_back(target);
        degree_count[target]++;
        degree_count[v] = 1;
    }

    std::set<std::pair<size_t, size_t>> merged_edges;
    merge_edges(edges, m, merged_edges);

    save_graph(merged_edges, outfile);
}


int main(int argc, char** argv) {
    if(argc < 6) {
        std::cerr << "Usage: " << argv[0] << " <n> <m> <p> <variate_type> <output_file>\n";
        return 1;
    }

    const size_t n = std::stoul(argv[1]);
    const size_t m = std::stoul(argv[2]);
    const double p = std::stod(argv[3]);
    const int variate_type = std::atoi(argv[4]);
    const std::string output_file = argv[5];

    std::ofstream outfile(output_file);
    if(!outfile.is_open()) {
        std::cerr << "Error opening file: " << output_file << "\n";
        return 1;
    }

    if(variate_type == 0) {
        linear_preferential_attachment(n, m, p, outfile);
    } else if(variate_type == 1) {
        if(argc < 7) {
            std::cerr << "Power variate requires an additional alpha parameter.\n";
            return 1;
        }

        const double alpha = std::stod(argv[6]);

        power_preferential_attachment(n, m, alpha, p, outfile);
    } else if(variate_type == 2) {
        log_based_preferential_attachment(n, m, p, outfile);
    } else {
        std::cerr << "Invalid variate type. Use 0 for linear, 1 for power, 2 for log-based.\n";
        return 1;
    }

    return 0;
}