#include "candies.h"

#include <algorithm>
#include <vector>

std::vector<int> distribute_candies(std::vector<int> c, std::vector<int> l,
                                    std::vector<int> r, std::vector<int> v) {
  int n = c.size(), q = v.size();
  std::vector<std::vector<std::pair<int, int>>> queriesL(n), queriesR(n);
  for (int i = 0; i < q; ++i) {
    queriesL[l[i]].emplace_back(i, v[i]);
    queriesR[r[i]].emplace_back(i, v[i]);
  }

  std::vector<int> s(n), vals(q);
  for (int i = 0; i < n; ++i) {
    for (auto [idx, val] : queriesL[i]) {
      vals[idx] = -val;
    }

    int mini = 0, maxi = 0, sum = 0, idx = 0;
    for (int j = q - 1; j >= 0; --j) {
      sum += vals[j];
      mini = std::min(mini, sum);
      maxi = std::max(maxi, sum);
      if (maxi - mini > c[i]) {
        idx = j;
        break;
      }
    }

    if (maxi - mini <= c[i]) {
      s[i] = -mini;
    } else {
      if (vals[idx] < 0) {
        s[i] = c[i] - maxi;
      } else {
        s[i] = -mini;
      }
    }

    for (auto [idx, val] : queriesR[i]) {
      vals[idx] = 0;
    }
  }

  return s;
}
