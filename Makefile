CXX = clang++
# CXXFLAGS = -std=c++20 -O3
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -O3

all: cpp-graph-test cpp-generate-graph

cpp-format: 
	clang-format -i cpp-graph/*.cpp cpp-graph/*.hpp

cpp-tidy:
	clang-tidy cpp-graph/*.cpp cpp-graph/*.hpp -- -std=c++20 -I/usr/lib/llvm-16/lib/clang/16/include

cpp-graph-test: cpp-graph/test.cpp
	$(CXX) $(CXXFLAGS) cpp-graph/test.cpp -o cpp-graph-test

cpp-generate-graph: cpp-graph/graph_factory.cpp cpp-graph/generate_graph.cpp
	cd stats/include && ln -s ../../gcem/include/gcem.hpp && ln -s ../../gcem/include/gcem_incl
	$(CXX) $(CXXFLAGS) cpp-graph/graph_factory.cpp cpp-graph/generate_graph.cpp -o cpp-generate-graph
	cd stats/include && rm gcem.hpp && rm -r gcem_incl

clean:
	rm -f cpp-graph-test cpp-generate-graph
	rm -f ba_graph.txt
