// O((n + q) sqrt(q))

#include "candies.h"

#include <algorithm>
#include <cmath>
#include <vector>

struct Buckets {
  int n;
  int bucket_size, bucket_cnt;
  std::vector<int> vals;
  std::vector<long long> mini, maxi, sums;
  Buckets(int _n): n(_n) {
    bucket_size = sqrt(n);
    bucket_cnt = (n + bucket_size - 1) / bucket_size;
    vals.assign(n, 0);
    mini.assign(bucket_cnt, 0);
    maxi.assign(bucket_cnt, 0);
    sums.assign(bucket_cnt, 0);
  }

  void update(int x, int val) {
    vals[x] = val;
    int bucket = x / bucket_size;

    mini[bucket] = maxi[bucket] = sums[bucket] = 0;
    for (int i = std::min(n, (bucket + 1) * bucket_size) - 1; 
         i >= bucket * bucket_size; --i) {
      sums[bucket] += vals[i];
      maxi[bucket] = std::max(maxi[bucket], sums[bucket]);
      mini[bucket] = std::min(mini[bucket], sums[bucket]);
    }
  }

  std::tuple<int, int, int> query(int c) {
    int sufsum = 0, sufmin = 0, sufmax = 0;
    for (int bucket = bucket_cnt - 1; bucket >= 0; --bucket) {
      long long nsufsum = sufsum + sums[bucket],
                nsufmin = std::min(1LL * sufmin, sufsum + mini[bucket]),
                nsufmax = std::max(1LL * sufmax, sufsum + maxi[bucket]);
      if (nsufmax - nsufmin > c) {
        for (int i = std::min(n, (bucket + 1) * bucket_size) - 1; 
             i >= bucket * bucket_size; --i) {
          sufsum += vals[i];
          sufmin = std::min(sufmin, sufsum);
          sufmax = std::max(sufmax, sufsum);
          if (sufmax - sufmin > c) return std::make_tuple(sufmin, sufmax, i);
        }
      }
      sufsum = nsufsum; sufmax = nsufmax; sufmin = nsufmin;
    }
    return std::make_tuple(sufmin, sufmax, 0);
  }

  inline int at(int x) { return vals[x]; }
};

std::vector<int> distribute_candies(std::vector<int> c, std::vector<int> l,
                                    std::vector<int> r, std::vector<int> v) {
  int n = c.size(), q = v.size();
  std::vector<std::vector<std::pair<int, int>>> queriesL(n), queriesR(n);
  for (int i = 0; i < q; ++i) {
    queriesL[l[i]].emplace_back(i, v[i]);
    queriesR[r[i]].emplace_back(i, v[i]);
  }

  std::vector<int> s(n);
  Buckets tree(q);
  for (int i = 0; i < n; ++i) {
    for (auto [idx, val] : queriesL[i]) {
      tree.update(idx, -val);
    }

    auto [mini, maxi, idx] = tree.query(c[i]);
    if (maxi - mini <= c[i]) {
      s[i] = -mini;
    } else {
      if (tree.at(idx) < 0) {
        s[i] = c[i] - maxi;
      } else {
        s[i] = -mini;
      }
    }

    for (auto [idx, val] : queriesR[i]) {
      tree.update(idx, 0);
    }
  }

  return s;
}
