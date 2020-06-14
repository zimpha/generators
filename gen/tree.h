#include "testlib.h"
#include <cmath>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

namespace gen {
namespace tree {

using edge_t = std::vector<std::pair<int, int>>;

// Generate a random tree with $n$ nodes builting with Prim-like process.
edge_t rand_with_prim(int n, int elongation = 0) {
  edge_t edges;
  for (int i = 1; i < n; ++i) {
    int p = rnd.wnext(i, elongation);
    edges.emplace_back(p, i);
  }
  return edges;
}

// Generate a random tree with $n$ nodes builting with Kruskal-like process.
// If $elongation = 1$, the expectation of diamater is $O(\sqrt{n})$.
// If $elongation = n$, the expectation of diamater may be $O(n)$.
edge_t rand_with_kruskal(int n, int elongation = 1) {
  std::vector<int> parent(n, -1);
  std::function<int(int)> find = [&](int u) {
    return ~parent[u] ? parent[u] = find(parent[u]) : u;
  };
  edge_t edges;
  for (auto _ = 0; _ < n - 1; ++ _) {
    while (true) {
      auto a = rnd.next(0, n - 1);
      auto b = (a + rnd.wnext(1, n - 1, elongation)) % n;
      auto u = find(a);
      auto v = find(b);
      if (u != v) {
        if (rnd.next(0, 1)) {
          std::swap(a, b);
        }
        edges.emplace_back(a, b);
        parent[u] = v;
        break;
      }
    }
  }
  return edges;
}

// Generate a random tree with $n$ nodes using prufer sequence.
edge_t rand_with_prufer(int n) {
  if (n == 1) return {};
  if (n == 2) return {{0, 1}};
  std::vector<int> a(n - 2), deg(n, 1);
  for (int i = 0; i < n - 2; ++i) a[i] = rnd.next(n);
  for (auto &x: a) deg[x]++;
  std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
  for (int i = 0; i < n; ++i) {
    if (deg[i] == 1) pq.push(i);
  }
  edge_t edges;
  for (auto &u: a) if (!pq.empty()) {
    int v = pq.top(); pq.pop();
    --deg[v]; --deg[u];
    edges.emplace_back(u, v);
    if (deg[u] == 1) pq.push(u);
  }
  int u = -1, v = -1;
  for (int i = 0; i < n && v == -1; ++i) if (deg[i] == 1) {
    if (u == -1) u = i;
    else v = i;
  }
  edges.emplace_back(u, v);
  return edges;
}

// Generate a random binary tree using: A random binary tree generator, by Martin, H. W., & Orr, B. J
// It will generate a binary tree with $n$ nodes such that
//   1. the root of the binary tree is $0$.
//   2. each node has at most two childrens.
//   3. for each edge $(u, v)$, if $label[u] == label[v]$, $v$ is right child of $u$; otherwise $v$ is left child of $u$.
edge_t rand_binary_inversion_table(int n, std::vector<int> &label) {
  label.resize(n); label[0] = 0;
  for (int i = 1; i < n; ++i) {
    label[i] = rnd.next(0, label[i - 1] + 1);
  }
  std::vector<int> stack = {0};
  edge_t edges;
  for (int i = 1; i < n; ++i) {
    if (label[stack.back()] < label[i]) {
      edges.emplace_back(stack.back(), i);
      stack.push_back(i);
    } else {
      while (!stack.empty() && label[stack.back()] > label[i]) stack.pop_back();
      ensuref(!stack.empty(), "rand_binary_inversion_table failed");
      edges.emplace_back(stack.back(), i);
      stack.push_back(i);
    }
  }
  return edges;
}

// Generate a random binary tree using: Fast Generation of Big Random Binary Trees, by W. B. Langdon
// It will generate a full binary tree with $n + 1$ leaves and $n$ internal nodes. And the root is $0$.
// For each edge $(u, v)$, the first occurrence of $(u, \cdot)$ is the left edge and the second is right edge.
// The average height is $2 \sqrt{\pi n} + O(n^{1/4+\epsilon})$.
edge_t random_binary_dyck(int n) {
  std::vector<int> dyck(n * 2 + 1);
  for (int i = 0; i <= n * 2; ++i) dyck[i] = i % 2;
  shuffle(dyck.begin(), dyck.end());
  int x = 0, y = 0, best = 1, pos = -1;
  for (int i = 0; i <= n * 2; ++i) {
    if (dyck[i] == 0) ++x; else ++y;
    if (x - y <= best) best = x - y, pos = i;
  }
  ensuref(dyck[pos] == 1, "random_binary_dyck failed: invalid start position");
  std::vector<int> cnt(n * 2 + 1);
  std::vector<int> stack = {0};
  edge_t edges;
  for (int i = 1; i <= n * 2; ++i) {
    int o = dyck[(pos - i + n * 2 + 1) % (n * 2 + 1)];
    edges.emplace_back(stack.back(), i);
    cnt[stack.back()]++;
    if (o == 1) stack.push_back(i);
    while (!stack.empty() && cnt[stack.back()] == 2) stack.pop_back();
  }
  ensuref(stack.empty(), "random_binary_dyck failed: stack should be empty");
  return edges;
}

// Generate a random caterpillar with $n$ nodes, where $m$ is the number of nodes on the main chain.
// If $m = 3$, it is a star.
edge_t rand_caterpillar(int n, int m) {
  if (n == 1) return {};
  if (n == 2) return {{0, 1}};
  ensuref(m >= 3, "number of nodes on the main chain >= 3");
  edge_t edges;
  for (int i = 0; i + 1 < m; ++i) edges.emplace_back(i, i + 1);
  for (int i = m; i < n; ++i) {
    edges.emplace_back(i, rnd.next(1, m - 1));
  }
  return edges;
}

// Generate a random tree with $n$ vertices and a long path of $m$ vertices.
edge_t rand_skew_tree(int n, int m) {
  ensuref(1 <= m && m <= n, "rand_skew_tree failed: 1 <= m <= n");
  edge_t edges;
  for (int i = 1; i < m; ++i) {
    edges.emplace_back(i - 1, i);
  }
  for (int i = m; i < n; ++i) {
    edges.emplace_back(rnd.next(0, i - 1), i);
  }
  return edges;
}

// If `w = 1`, generate a path with $n$ vertices.
edge_t rand_path(int n, int w) {
  edge_t edges;
  for (int i = 1; i < n; ++i) {
    edges.emplace_back(rnd.next(std::max(0, i - w), i - 1), i);
  }
  return edges;
}

// Generate k-ary tree with $n$ vertices.
edge_t rand_kary(int n, int k) {
  ensuref(k >= 1, "rand_kary failed: k >= 2");
  edge_t edges;
  for (int i = 1; i < n; ++i) {
    edges.emplace_back(i, i / k);
  }
  return edges;
}

// Generate an $m$-wheel with $n$ vertices.
edge_t rand_wheel(int n, int m) {
  edge_t edges;
  for (int i = 1; i < n; ++i) {
    if (i <= m) edges.emplace_back(0, i);
    else edges.emplace_back(i - m, i);
  }
  return edges;
}

// Relabel the vertices
void relabel_vertices(int n, edge_t& edges) {
  std::vector<int> label(n);
  for (int i = 0; i < n; ++i) label[i] = i;
  shuffle(label.begin(), label.end());
  for (auto &e: edges) {
    e.first = label[e.first];
    e.second = label[e.second];
  }
}

// Convert an unrooted tree to a rooted tree
// If `relabel = 1`, the result `parent` array will satisfy: `parent[i] < i`.
// Otherwise, `parent[root]` will be `-1`.`
std::vector<int> convert_rooted(int n, const edge_t& edges, int root, bool relabel = false) {
  std::vector<int> label(n);
  std::vector<std::vector<int>> adj(n);
  for (auto &e: edges) {
    adj[e.first].push_back(e.second);
    adj[e.second].push_back(e.first);
  }

  std::vector<int> parent(n, -1);
  int m = 0;
  std::function<void(int, int)> dfs = [&](int u, int p) {
    label[u] = m++;
    if (p != -1) {
      if (relabel) parent[label[u]] = label[p];
      else parent[u] = p;
    }
    for (auto &v: adj[u]) if (v != p) {
      dfs(v, u);
    }
  };

  dfs(root, -1);

  return parent;
}
}
}