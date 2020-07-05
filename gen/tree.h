#include "testlib.h"
#include <cmath>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

namespace gen {
namespace tree {

class Tree {
 public:
  explicit Tree(int n): n(n) {}

  void add_edge(int u, int v) {
    ensuref(0 <= u && u < n, "index out of bound");
    ensuref(0 <= v && v < n, "index out of bound");
    edges.emplace_back(u, v);
  }

  void relabel() {
    std::vector<int> label(n);
    for (int i = 0; i < n; ++i) label[i] = i;
    shuffle(label.begin(), label.end());
    for (auto &e: edges) {
      e.first = label[e.first];
      e.second = label[e.second];
      if (rnd.next(2)) std::swap(e.first, e.second);
    }
  }

  // Convert an unrooted tree to a rooted tree
  // If `relabel = 1`, the result `parent` array will satisfy: `parent[i] < i`.
  // Otherwise, `parent[root]` will be `-1`.
  std::vector<int> parents(int root, bool relabel = false) {
    ensuref(0 <= root && root < n, "index out of bound");
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

  // Merge `this` and `rhs` such that vertices `u` and `v` are connected with an extra edge.
  void link(int u, const Tree &rhs, int v) {
    ensuref(0 <= u && u < n, "index out of bound");
    ensuref(0 <= v && v < rhs.n, "index out of bound");
    for (auto &e: rhs.edges) {
      add_edge(n + e.first, n + e.second);
    }
    add_edge(u, v + n);
    n += rhs.n;
  }

  // Merge `this` and `rhs` such that vertices `u` and `v` are glued into one.
  void glue(int u, const Tree &rhs, int v) {
    ensuref(0 <= u && u < n, "index out of bound");
    ensuref(0 <= v && v < rhs.n, "index out of bound");
    for (auto &e: rhs.edges) {
      int a = e.first, b = e.second;
      if (e.first == v) a = u;
      else if (e.first > v) a = e.first + n - 1;
      else a = e.first + n;
      if (e.second == v) b = u;
      else if (e.second > v) b = e.second + n - 1;
      else b = e.second + n;
      add_edge(a, b);
    }
    n += rhs.n - 1;
  }

  static Tree from_prufer(const std::vector<int>& a) {
    int n = a.size() + 2;
    Tree res(n);
    std::vector<int> deg(n, 1);
    for (auto &x: a) deg[x]++;
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 0; i < n; ++i) {
      if (deg[i] == 1) pq.push(i);
    }
    for (auto &u: a) if (!pq.empty()) {
      int v = pq.top(); pq.pop();
      --deg[v]; --deg[u];
      res.add_edge(u, v);
      if (deg[u] == 1) pq.push(u);
    }
    int u = -1, v = -1;
    for (int i = 0; i < n && v == -1; ++i) if (deg[i] == 1) {
      if (u == -1) u = i;
      else v = i;
    }
    res.add_edge(u, v);
    return res;
  }

 private:
  int n;
  std::vector<std::pair<int, int>> edges;
};

// Generate a random tree with $n$ nodes using prufer sequence.
Tree random(int n) {
  Tree res(n);
  if (n == 1) return res;
  if (n == 2) {
    res.add_edge(0, 1);
    return res;
  }
  std::vector<int> a(n - 2);
  for (int i = 0; i < n - 2; ++i) a[i] = rnd.next(n);
  return Tree::from_prufer(a);
}

// Generate a random tree with $n$ nodes builting with Prim-like process.
Tree rand_with_prim(int n, int elongation = 0) {
  Tree res(n);
  for (int i = 1; i < n; ++i) {
    int p = rnd.wnext(i, elongation);
    res.add_edge(p, i);
  }
  return res;
}

// Generate a random tree with $n$ nodes builting with Kruskal-like process.
// If $elongation = 1$, the expectation of diamater is $O(\sqrt{n})$.
// If $elongation = n$, the expectation of diamater may be $O(n)$.
Tree rand_with_kruskal(int n, int elongation = 1) {
  std::vector<int> parent(n, -1);
  std::function<int(int)> find = [&](int u) {
    return ~parent[u] ? parent[u] = find(parent[u]) : u;
  };
  Tree res(n);
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
        res.add_edge(a, b);
        parent[u] = v;
        break;
      }
    }
  }
  return res;
}

// Generate a random binary tree using: A random binary tree generator, by Martin, H. W., & Orr, B. J
// It will generate a binary tree with $n$ nodes such that
//   1. the root of the binary tree is $0$.
//   2. each node has at most two childrens.
//   3. for each edge $(u, v)$, if $label[u] == label[v]$, $v$ is right child of $u$; otherwise $v$ is left child of $u$.
Tree rand_binary_inversion_table(int n, std::vector<int> &label) {
  label.resize(n); label[0] = 0;
  for (int i = 1; i < n; ++i) {
    label[i] = rnd.next(0, label[i - 1] + 1);
  }
  std::vector<int> stack = {0};
  Tree res(n);
  for (int i = 1; i < n; ++i) {
    if (label[stack.back()] < label[i]) {
      res.add_edge(stack.back(), i);
      stack.push_back(i);
    } else {
      while (!stack.empty() && label[stack.back()] > label[i]) stack.pop_back();
      ensuref(!stack.empty(), "rand_binary_inversion_table failed");
      res.add_edge(stack.back(), i);
      stack.push_back(i);
    }
  }
  return res;
}

// Generate a random binary tree using: Fast Generation of Big Random Binary Trees, by W. B. Langdon
// It will generate a full binary tree with $n + 1$ leaves and $n$ internal nodes. And the root is $0$.
// For each edge $(u, v)$, the first occurrence of $(u, \cdot)$ is the left edge and the second is right edge.
// The average height is $2 \sqrt{\pi n} + O(n^{1/4+\epsilon})$.
Tree random_binary_dyck(int n) {
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
  Tree res(n);
  for (int i = 1; i <= n * 2; ++i) {
    int o = dyck[(pos - i + n * 2 + 1) % (n * 2 + 1)];
    res.add_edge(stack.back(), i);
    cnt[stack.back()]++;
    if (o == 1) stack.push_back(i);
    while (!stack.empty() && cnt[stack.back()] == 2) stack.pop_back();
  }
  ensuref(stack.empty(), "random_binary_dyck failed: stack should be empty");
  return res;
}

// Generate a random caterpillar with $n$ nodes, where $m$ is the number of nodes on the main chain.
// If $m = 3$, it is a star.
Tree rand_caterpillar(int n, int m) {
  Tree res(n);
  if (n == 1) return res;
  if (n == 2) {
    res.add_edge(0, 1);
    return res;
  }
  ensuref(m >= 3, "number of nodes on the main chain >= 3");
  for (int i = 0; i + 1 < m; ++i) {
    res.add_edge(i, i + 1);
  }
  for (int i = m; i < n; ++i) {
    res.add_edge(i, rnd.next(1, m - 1));
  }
  return res;
}

// Generate a random tree with $n$ vertices and a long path of $m$ vertices.
Tree rand_skew_tree(int n, int m) {
  ensuref(1 <= m && m <= n, "rand_skew_tree failed: 1 <= m <= n");
  Tree res(n);
  for (int i = 1; i < m; ++i) {
    res.add_edge(i - 1, i);
  }
  for (int i = m; i < n; ++i) {
    res.add_edge(rnd.next(0, i - 1), i);
  }
  return res;
}

// If `w = 1`, generate a path with $n$ vertices.
Tree rand_path(int n, int w) {
  Tree res(n);
  for (int i = 1; i < n; ++i) {
    res.add_edge(rnd.next(std::max(0, i - w), i - 1), i);
  }
  return res;
}

// Generate k-ary tree with $n$ vertices.
Tree rand_kary(int n, int k) {
  ensuref(k >= 1, "rand_kary failed: k >= 2");
  Tree res(n);
  for (int i = 1; i < n; ++i) {
    res.add_edge(i, i / k);
  }
  return res;
}

// Generate an $m$-wheel with $n$ vertices.
Tree rand_wheel(int n, int m) {
  Tree res(n);
  for (int i = 1; i < n; ++i) {
    if (i <= m) res.add_edge(0, i);
    else res.add_edge(i - m, i);
  }
  return res;
}

}
}
