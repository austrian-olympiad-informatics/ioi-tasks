#include "candies.h"

#include <algorithm>
#include <vector>

struct SegTree {
  int n;
  std::vector<long long> mini, maxi, sums, vals;

  SegTree(int _n): n(_n) {
    mini.assign(n * 2, 0);
    maxi.assign(n * 2, 0);
    sums.assign(n * 2, 0);
    vals.assign(n, 0);
  }

  void update(int idx, int l, int r, int x, int val) {
    if (l == x && r == x + 1) {
      mini[idx] = maxi[idx] = sums[idx] = val;
      return;
    }

    int mid = (l + r) >> 1;
    int lidx = idx + 1, ridx = idx + (mid - l) * 2;
    if (x < mid) update(lidx, l, mid, x, val);
    else update(ridx, mid, r, x, val);
    mini[idx] = std::min(mini[lidx] + sums[ridx], mini[ridx]);
    maxi[idx] = std::max(maxi[lidx] + sums[ridx], maxi[ridx]);
    sums[idx] = sums[lidx] + sums[ridx];
  }

  std::tuple<int, int, int> query(int idx, int l, int r,
                                  int sufmin, int sufmax, int sufsum, int c) {
    if (l + 1 == r) {
      return std::make_tuple(std::min(1LL * sufmin, sufsum + mini[idx]),
                             std::max(1LL * sufmax, sufsum + maxi[idx]), l);
    }

    int mid = (l + r) >> 1;
    int lidx = idx + 1, ridx = idx + (mid - l) * 2;
    long long nsufmin = std::min(1LL * sufmin, sufsum + mini[ridx]), 
              nsufmax = std::max(1LL * sufmax, sufsum + maxi[ridx]),
              nsufsum = sufsum + sums[ridx];

    if (nsufmax - nsufmin > c) return query(ridx, mid, r, sufmin, sufmax, sufsum, c);
    return query(lidx, l, mid, nsufmin, nsufmax, nsufsum, c);
  }

  void update(int x, int val) {
    vals[x] = val;
    update(0, 0, n, x, val);
  }

  // Find smallest suffix such that max - min > c.
  // If no such suffix, the whole range is returned.
  // Returns (max, min, i)
  std::tuple<int, int, int> query(int c) {
    return query(0, 0, n, 0, 0, 0, c);
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
  SegTree tree(q);
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
