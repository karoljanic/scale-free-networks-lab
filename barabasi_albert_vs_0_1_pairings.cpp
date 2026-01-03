#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <vector>


struct Endpoint {
    double x{0.0};
    size_t chord_id{0};
    bool is_right{false};
};

struct GraphEdge {
    size_t u{0};
    size_t v{0};
};

struct DegreesCounter {
    size_t cnt{0};
    size_t num{0};
};


std::vector<GraphEdge> generate_ba_pairing_01(const size_t n, const size_t m, std::mt19937& gen) {
    std::uniform_real_distribution<> dis(0.0, 1.0);

    const size_t N = m * n;
    std::vector<Endpoint> points(2 * N);
    for(size_t i = 0; i < N; i++) {
        const double r1 = dis(gen);
        const double r2 = dis(gen);

        if(r1 < r2) {
            points.emplace_back(r1, i, false);
            points.emplace_back(r2, i, true);
        }
        else {
            points.emplace_back(r2, i, false);
            points.emplace_back(r1, i, true);
        }
    }

    std::sort(points.begin(), points.end(), [](const Endpoint& a, const Endpoint& b) {
        return a.x < b.x;
    });

    for(size_t i = 0; i < 2 * N; i++) {
        points[i].chord_id = i + 1;
    }

    std::vector<GraphEdge> edges;
    edges.reserve(N);

    std::vector<ssize_t> pair_first_vertex(N, -1);
    size_t current_vertex_id = 1;

    for(const auto& p : points) {
        if (pair_first_vertex[p.chord_id] == -1) {
            pair_first_vertex[p.chord_id] = current_vertex_id;
        } 
        else {
            if(pair_first_vertex[p.chord_id] < current_vertex_id) {
                edges.emplace_back(pair_first_vertex[p.chord_id], current_vertex_id);
            }
            else {
                edges.emplace_back(current_vertex_id, pair_first_vertex[p.chord_id]);
            }
        }

        if (p.is_right) {
            current_vertex_id++;
        }
    }

    return edges;
}

std::vector<GraphEdge> generate_ba_batagelj_urlika_brandes(const size_t n, const size_t m, std::mt19937& gen) {
    const size_t N = m * n;
    std::vector<size_t> points(2 * N);

    for(size_t i = 0; i < N; i++) {        
        size_t v = i / m;
        points[2 * i] = v;

        std::uniform_int_distribution<> dis(0, 2 * i);
        const size_t r = dis(gen);

        points[2 * i + 1] = points[r];
    }

    std::vector<GraphEdge> edges(N);

    for(size_t i = 0; i < N; i++) {
        edges.emplace_back(points[2 * i], points[2 * i + 1]);
    }

    return edges;
}

std::map<size_t, size_t> generate_histogram(const std::vector<GraphEdge>& edges, const size_t n) {
    std::vector<size_t> deg(n, 0);
    for(const auto& edge: edges) {
        deg[edge.u]++;
        deg[edge.v]++;
    }

    std::map<size_t, size_t> histogram;
    for (size_t i = 0; i < n; i++) {
        histogram[deg[i]]++;
    }

    return histogram;
}

size_t count_loops(const std::vector<GraphEdge>& edges) {
    size_t loops_cnt = 0;
    for(const auto& edge: edges) {
        if(edge.u == edge.v) {
            loops_cnt++;
        }
    }

    return loops_cnt;
}

size_t count_multiple_edges(const std::vector<GraphEdge>& edges) {
    size_t multiple_edge_cnt = 0;
    
    
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <n> <m> <reps>\n";
        return 1;
    }

    const size_t n = std::stoi(argv[1]);
    const size_t m = std::stoi(argv[2]);
    const size_t reps = std::stoi(argv[3]);

    if (n == 0 || m == 0) {
        std::cerr << "n and m must be positive integers\n";
        return 1;
    }
    if (m > n) {
        std::cerr << "m must be <= n\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::map<size_t, DegreesCounter> histogram_pairing_01;
    std::map<size_t, DegreesCounter> histogram_batagelj_urlika_brandes;

    for(size_t rep = 0; rep < reps; rep++) {
        const std::vector<GraphEdge> edges_pairing_01 = generate_ba_pairing_01(n, m, gen);
        const std::vector<GraphEdge> edges_batagelj_urlika_brandes = generate_ba_batagelj_urlika_brandes(n, m, gen);

        const std::map<size_t, size_t> hist_pairing_01(edges_pairing_01, n);
        const std::map<size_t, size_t> hist_batagelj_urlika_brandes(edges_batagelj_urlika_brandes, n);

        for (const auto& kv : hist_pairing_01) {
            const size_t degree = kv.first;
            const size_t count = kv.second;
            auto& dc = histogram_pairing_01[degree];
            dc.cnt += count;
            dc.num += 1;
        }

        for (const auto& kv : hist_batagelj_urlika_brandes) {
            const size_t degree = kv.first;
            const size_t count = kv.second;
            auto& dc = histogram_batagelj_urlika_brandes[degree];
            dc.cnt += count;
            dc.num += 1;
        }
    }
}