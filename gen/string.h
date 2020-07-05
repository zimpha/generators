#pragma once

#include "testlib.h"
#include "util.h"
#include <string>
#include <algorithm>
#include <stdexcept>

namespace gen {
namespace str {

// random
std::string random(int n, int c) {
  std::string s(n, 'a');
  for (int i = 0; i < n; ++i) {
    s[i] = 'a' + rnd.next(0, c - 1);
  }
  return s;
}

// thue morse word in base $m$
// a -> ab, b -> ba
std::string thue_morse(int n, int m) {
  std::string s(n, 'a');
  for (int i = 0; i < n; ++i) {
    int w = 0, j = i;
    while (j) {
      w = (w + j) % m;
      j /= m;
    }
    s[i] = w + 'a';
  }
  return s;
}

// prefix of fibonacci word
// a -> ab, b -> a
std::string fibonacci(int n) {
  std::string s(n, 'a');
  s[0] = s[1] = rnd.next(0, 25) + 'a';
  while (s[0] == s[1]) {
    s[1] = rnd.next(0, 25) + 'a';
  }
  int a = 1, b = 2;
  for (int i = 3; i <= n; ++i) {
    if (a + b < i) {
      a += b;
      std::swap(a, b);
    }
    s[i - 1] = s[i - b - 1];
  }
  return s;
}

// prefix of the Tribonacci word
// a -> ab, b -> ac, c -> a
std::string tribonacci(int n) {
  auto letters = gen::util::random_subset(26, 3);
  std::string s(std::max(4, n));
  s[0] = 'a' + letters[0];
  s[1] = 'a' + letters[1];
  s[2] = 'a' + letters[0];
  s[3] = 'a' + letters[2];
  for (int i = 4, j = 2; i < n; ++j) {
      s[i++] = s[0];
      if (i < n && s[j] != s[3]) {
          s[i++] = (s[j] == s[0]) ? s[1] : s[3];
      }
  }
  return s.substr(0, n);
}

// prefix of The Rudin-Shapiro word
// a -> ab, b -> ac, c -> db, d -> dc
std::string rudin_shapiro(int n) {
  throw std::runtime_error("not implemented yet");
}

// prefix of The regular paperfolding word
// a -> ab, b -> cb, c - > ad, d - > cd
std::string regular_paperfolding(int n) {
  throw std::runtime_error("not implemented yet");
}

// string with period p
std::string period(int n, const std::string &p) {
  std::string s(n, 'a');
  for (int i = 0; i < n; ++i) s[i] = p[i % p.size()];
  return s;
}

// the probability of letter p is \frac{1}{m^p}
std::string geometric(int n, int m) {
  std::vector<char> letters;
  for (char i = 'a'; i <= 'z'; ++i) {
    letters.push_back(i);
  }
  shuffle(letters.begin(), letters.end());
  std::string s(n, 'a');
  for (int i = 1; i <= n; ++i) {
    int p = 0;
    while (p < 25 && !rnd.next(0, m - 1)) ++p;
    s[i - 1] = letters[p];
  }
  return s;
}

// i-th character is a + ctz(i)
std::string abacaba(int n, int c) {
  std::vector<char> letters;
  for (char i = 'a';  i<= 'z'; ++i) {
    letters.push_back(i);
  }
  shuffle(letters.begin(), letters.end());
  std::string s(n, 'a');
  for (int i = 1; i <= n; ++i) {
    s[i - 1] = letters[std::min(__builtin_ctz(i), c)];
  }
  return s;
}

// string with many borders
std::string border(const std::vector<int>& diff) {
  std::string s = "a";
  for (auto d: diff) {
    std::string p = s;
    s = "";
    for (int j = 0; j < d; ++j) s += p;
    s.back()++;
  }
  s.pop_back();
  return s;
}

}
}
