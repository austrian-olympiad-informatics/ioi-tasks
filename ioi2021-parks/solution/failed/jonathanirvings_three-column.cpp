#include "parks.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int kMinX = 2;
constexpr int kMidX = 4;
constexpr int kMaxX = 6;

int construct_roads(std::vector<int> X, std::vector<int> Y) {
  int N = X.size();

  vector<map<int, int>> pos(kMaxX + 1);
  for (int i = 0; i < N; ++i) {
    pos[X[i]][Y[i]] = i;
  }

  vector<int> parent(N);
  iota(parent.begin(), parent.end(), 0);

  function<int(int)> find_root = [&] (int u) {
    return parent[u] = u == parent[u] ? u : find_root(parent[u]);
  };

  for (int i = 0; i < N; ++i) {
    if (pos[X[i]].count(Y[i] + 2)) {
      parent[find_root(i)] = find_root(pos[X[i]][Y[i] + 2]);
    }
    if (X[i] < kMaxX && pos[X[i] + 2].count(Y[i])) {
      parent[find_root(i)] = find_root(pos[X[i] + 2][Y[i]]);
    }
  }

  for (int i = 0; i < N; ++i) {
    if (find_root(i) != find_root(0)) {
      return 0;
    }
  }

  vector<int> U, V, A, B;
  
  for (int x : {kMinX, kMaxX}) {
    for (map<int, int>::iterator it = pos[x].begin();
         it != pos[x].end();
         ++it) {
      int y = it->first;
      if (pos[x].count(y + 2)) {
        U.push_back(pos[x][y]);
        V.push_back(pos[x][y + 2]);
        A.push_back(x == kMinX ? x - 1 : x + 1);
        B.push_back(y + 1);
      }
    }
  }

  for (map<int, int>::iterator it = pos[kMidX].begin();
       it != pos[kMidX].end();
       ++it) {
    int x = kMidX;
    int y = it->first;
    if (pos[x].count(y + 2)) {
      U.push_back(pos[x][y]);
      V.push_back(pos[x][y + 2]);
      B.push_back(y + 1);
      if (y & 2) {
        A.push_back(x + 1);

        if (pos[x - 2].count(y)) {
          U.push_back(pos[x][y]);
          V.push_back(pos[x - 2][y]);
          A.push_back(x - 1);
          B.push_back(y + 1);
        }

        if (pos[x + 2].count(y)) {
          if (pos[x + 2].count(y - 2) == 0 || pos[x].count(y - 2) == 0) {
            U.push_back(pos[x][y]);
            V.push_back(pos[x + 2][y]);
            A.push_back(x + 1);
            B.push_back(y - 1);
          }
        }
      } else {
        A.push_back(x - 1);

        if (pos[x - 2].count(y)) {
          if (pos[x - 2].count(y - 2) == 0 || pos[x].count(y - 2) == 0) {
            U.push_back(pos[x][y]);
            V.push_back(pos[x - 2][y]);
            A.push_back(x - 1);
            B.push_back(y - 1);
          }
        }

        if (pos[x + 2].count(y)) {
          U.push_back(pos[x][y]);
          V.push_back(pos[x + 2][y]);
          A.push_back(x + 1);
          B.push_back(y + 1); 
        }
      }
    } else {
      for (int d : {-1, 1}) {
        if (pos[x + d * 2].count(y)) {
          U.push_back(pos[x][y]);
          V.push_back(pos[x + d * 2][y]);
          A.push_back(x + d);
          B.push_back(y + 1);
        }
      }
    }
  }

  build(U, V, A, B);
  return 1;
}
