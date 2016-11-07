#include "testlib.h"

#include <functional>
#include <numeric>
#include <utility>
#include <vector>

// w = 1 时，直径期望是 $O(\sqrt{n})$
// w = n 时，直径期望是 $O(n)$ （我瞎说的）

std::vector<std::pair<int, int>> random_tree(int n, int w)
{
    std::vector<int> parent(n, -1);
    std::function<int(int)> find = [&](int u) {
        return ~parent[u] ? parent[u] = find(parent[u]) : u;
    };
    std::vector<std::pair<int, int>> edges;
    for (auto _ = 0; _ < n - 1; ++ _) {
        while (true) {
            auto a = rnd.next(0, n - 1);
            auto b = (a + rnd.wnext(1, n - 1, w)) % n;
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

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);
    auto n = std::atoi(argv[1]);
    auto w = std::atoi(argv[2]);
    for (auto&& e : random_tree(n, w)) {
        printf("%d %d\n", e.first, e.second);
    }
}
