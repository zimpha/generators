#include <cassert>

// binom{[n]}{k} : O(k \log^2 n)
template<typename Int>
std::vector<Int> random_subset(Int n, int k)
{
    assert(n >= k);
    std::vector<Int> result;
    std::map<Int, int> count;
    auto at = [&count](Int index)
    {
        auto iterator = count.find(index);
        return iterator == count.end() ? 0 : iterator->second;
    };
    for (auto i = 0; i < k; ++ i) {
        auto rank = rnd.next(Int(1), n - i);
        auto low = Int();
        auto high = n - 1;
        while (low < high) {
            auto middle = low + high >> 1;
            auto c = middle + 1;
            for (auto p = middle; ~p; p -= ~p & p + 1) {
                c -= at(p);
            }
            if (c >= rank) {
                high = middle;
            } else {
                low = middle + 1;
            }
        }
        for (auto p = high; p < n; p += ~p & p + 1) {
            count[p] ++;
        }
        result.push_back(high);
    }
    std::sort(result.begin(), result.end());
    return result;
}

int main()
{
}
