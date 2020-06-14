#pragma once

#include "testlib.h"
#include <vector>

namespace gen {
namespace util {
// Random distribute $n$ undistinguishable item into $T$ different boxes.
std::vector<int> distribute(int n, int T) {
  std::vector<int> cuts, result;
  cuts.push_back(0);
  for (auto i = 0; i < T - 1; ++ i) {
    cuts.push_back(rnd.next(0, n - T));
  }
  cuts.push_back(n - T);
  std::sort(cuts.begin(), cuts.end());
  for (auto i = 1; i <= T; ++ i) {
    result.push_back(cuts.at(i) - cuts.at(i - 1) + 1);
  }
  return result;
}

// Random sample $k$ elements from $n$ elements with equal probability.
// Time Complexity: $O(k \log k)$
template<typename Int>
std::vector<Int> random_subset(Int n, int k) {
  ensuref(n >= k, "random_subset failed: n >= k");
  std::set<Int> set;
  for (Int i = n - k; i < n; ++ i) {
    Int p = rnd.next(static_cast<Int>(0), i);
    if (set.count(p)) {
      set.insert(i);
    } else {
      set.insert(p);
    }
  }
  std::vector<Int> result;
  for (auto elem: set) {
    result.push_back(elem);
  }
  return result;
}
}
}
