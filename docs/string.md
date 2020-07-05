# Strings Generation

## Generators

### `std::string random(int n, int c)`

This function will generate a random string whose characters are from first $c$ lowercase English letters.

### `std::string thue_morse(int n, int m)`

This function will generate a prefix of length $n$ of the Thue-Morse string in base $m$.

The $i$-th character is the sum of digits modulo $m$ of $i$ in base $m$.

Note: this string is useful for breaking hashes modulo $2^{64}$ when $n \ge 2048$.

### `std::string fibonacci(int n)`

This function will generate a prefix of length $n$ of the Fibonacci word.

Start from `b`, and apply the substitutes: `a -> ab`, `b -> a`.

### `std::string tribonacci(int n)`

This function will generate a prefix of length $n$ of the Tribonacci word.

Apply the substitutes: `a -> ab`, `b -> ac`, and `c -> a`.

### `std::string period(int n, const std::string &p)`

This function will generate a string of length $n$ whose period is $p$.

### `std::string geometric(int n, int m)`

This function will generate a string of length $n$ such that the probability of letter $p$ is $\frac{1}{m^p}$.

### `std::string abacaba(int n, int c)`

This function will generate a prefix of length $n$ of the string of form `abacabadabacaba...`. The $i$-th character is `a + ctz(i)`, where $ctz(i)$ is the number of trailing zeros.

### `std::string border(const std::vector<int>& diff)`

This function will generate a string in kmp-like process. The borders of the generated string will form $|diff|$ arithmetic progressions.

## TODO

+ Rudin-Shapiro word generator
+ Regular paperfolding word generator
+ Antihash Generator
+ Generators for palindromic related algorithm