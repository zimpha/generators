# Trees Generation

A class `Tree` with some useful methods and a set of generators are provided in `tree.h`. Here is an example to generate a random tree, relabel the vertices, and convert it to rooted tree.

```cpp
auto tree = gen::tree::random(n);
tree.relabel();
auto parents = tree.parents(0 /* root */, true /* relabel */);
```

## Generators

### `Tree random(int n)`

This function will generate a tree with $n$ vertex using Prüfer sequence.

An array with length $n-2$ will be generated randomly and it will be converted to a tree using the algorithm described in [Wikipedia](https://en.wikipedia.org/wiki/Pr%C3%BCfer_sequence#Algorithm_to_convert_a_Pr%C3%BCfer_sequence_into_a_tree).

### `rand_with_prim(int n, int elongation = 0)`

This function will generate a tree using Prim-like process. The most classical tree generator ever.

The vertex $0$ will be chosen as a root vertex. And for each vertex from $1$ to $n-1$, its parent will be selected as $wnext(i, elongation)$. With $elongation = -10^6$, the generated tree will like a star. With $elongation = 10^6$, the generated tree will like a path.

### `rand_with_kruskal(int n, int elongation = 1)`

This function will generate a tree using Kruskal-like process.

Initially, there are no edges among the $n$ vertex. We will try to run the below process $n-1$ times to generate a tree:

+ Randomly choose a vertex $u$ from $0$ to $n-1$, and choose a vertex $v = (u + wnext(1, n - 1, elongation)) \bmod n$;
+ If $u$ and $v$ are not in the same connected component, add an edge between. Otherwise, repeated the steps.

With $elongation = 1$, the expectation of diamater is $O(\sqrt{n})$. With $elongation = n$, the expectation of diamater may be $O(n)$.

### `rand_binary_inversion_table(int n, std::vector<int> &label)`

This function will generate a fully random binary tree with $n$ nodes such that

+ The root of the binary tree is $0$.
+ each node has at most two childrens.
+ for each edge $(u, v)$, if $label[u] == label[v]$, $v$ is right child of $u$; otherwise $v$ is left child of $u$.

See the paper *A random binary tree generator* for more details.

### `random_binary_dyck(int n)`

This function will generate a fully random full binary tree with $n + 1$ leaves and $n$ internal nodes such that 

+ The root of the binary tree is $0$.
+ For each edge $(u, v)$, the first occurrence of $(u, \cdot)$ is the left edge and the second is right edge.
+ The average height is $2 \sqrt{\pi n} + O(n^{1/4+\epsilon})$.

### `rand_caterpillar(int n, int m)`

This function will generate a caterpillar with $n$ vertices based on a path of length $m$.

Firat, a path of length $m$ is generated. Vertices of the path are numbered in order. Next, the other $n - m$ vertices are connected to random vertices of the path.

### `rand_skew_tree(int n, int m)`

This function will generate a skewed tree with $n$ vertices based on a path of length $m$.

Firat, a path of length $m$ is generated. Vertices of the path are numbered in order. Next, for each vertex $i$ from $m$ to $n-1$, it will be connected to a vertex $j$ randomly from $0$ to $i-1$.

### `rand_path(int n, int w)`

This function will generate a path-like tree with $n$ vertices.

For each vertex $i$ from $1$ to $n-1$, it will be connected to a vertex $j$ randomly from $\max(0, i - w)$ to $i - 1$.


### `rand_kary(int n, int k)`

This function will generate a complete $k$-ary tree with $n$ vertices.

The parent of vertex $i$ will be $\lfloor \frac{i-1}{k} \rfloor$ and the root vertex is $0$.

### `rand_wheel(int n, int m)`

This function will generate a wheel-like tree with $n$ vertices based on the number of branches $m$.

For each vertex $i$ from $1$ to $m$, it is connected to vertex $0$. For each of other vertices $i$, it is connected to vertex $i-m$.

With $m=n-1$, the generated tree will be a star.

## Methods of `Tree`

### `void relabel()`

+ Relabel vertices in random order.
+ Random shuffle the order of edges.
+ Randomly swap the endpoints of each edge.

### `std::vector<int> parents(int root, bool relabel = false)`

This function will generate a rooted tree using the $edges$ of the tree with the given root vertex $root$.

If $relabel = 1$, the result $parent$ array will satisfy: $parent[i] < i$. Otherwise, $parent[root]$ will be $-1$.

### `void link(int u, const Tree &rhs, int v)`

### `void glue(int u, const Tree &rhs, int v)`