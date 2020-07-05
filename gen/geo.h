#pragma

#include "testlib.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

using int64 = long long;

namespace gen {
namespace geo {

struct Point {
  int64 x, y;
};

// Generate a simple polygon with $n$ vertices whose coordinators are in $[-m, m]$.
// No three consecutive vertices lie on the same line, no two points coincide.
// No self intersection is allowed.
std::vector<Point> simple_polygon(int n, int64 m) {
  throw std::runtime_error("not implemented yet");
}

// Generate a random convex hull with $n$ vertices whose coordinators are in $[-m, m]$
// No three consecutive vertices lie on the same line, no two points coincide.
std::vector<Point> convex_polygon(int n, int64 m) {
  throw std::runtime_error("not implemented yet");
}

// Generate $n$ random points whose coordinators are in $[-m, m]$. 
// No two coincide and no three lie on the same line.
std::vector<Point> random_points(int n, int64 m) {
  throw std::runtime_error("not implemented yet");
}

}
}