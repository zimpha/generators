#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

namespace gen {
namespace graph {

class Graph {
 public:

 private:
  int n;
  std::vector<std::pair<int, int>> edges;
};

Graph random(int n, int m) {
  throw std::runtime_error("not implemented yet");
}

Graph cactus(int n, int m) {
  throw std::runtime_error("not implemented yet");
}

Graph planar_graph(int n, int m) {
  throw std::runtime_error("not implemented yet");
}

Graph bipartite_graph(int left, int right, int m) {
  throw std::runtime_error("not implemented yet");
}

Graph chordal_graph(int n, int m) {
  throw std::runtime_error("not implemented yet");
}

}
}