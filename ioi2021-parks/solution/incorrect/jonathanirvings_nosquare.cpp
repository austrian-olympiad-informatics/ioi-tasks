#include "parks.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int kDirX[] = {0, 2, 0, -2};
constexpr int kDirY[] = {2, 0, -2, 0};

int construct_roads(std::vector<int> X, std::vector<int> Y) {
  int N = X.size();

  map<pair<int, int>, int> pos;
  for (int i = 0; i < N; ++i) {
    pos[make_pair(X[i], Y[i])] = i;
  }

  vector<bool> visited(N, false);
  vector<int> U, V, A, B;

  function<void (int)> dfs = [&] (int u) {
    for (int dir = 0; dir < 4; ++dir) {
      if (pos.count(make_pair(X[u] + kDirX[dir], Y[u] + kDirY[dir]))) {
        int v = pos[make_pair(X[u] + kDirX[dir], Y[u] + kDirY[dir])];
        if (!visited[v]) {
          U.push_back(u);
          V.push_back(v);

          //    |
          //  ^ |->
          //  | |
          // ---0---
          //    | |
          //  <-| v
          //    |
          bool odd = (X[u] + Y[u]) & 2;

          if (X[u] == X[v]) {
            B.push_back(Y[u] + (Y[u] < Y[v] ? 1 : -1));
            A.push_back(X[u] + (odd ^ (Y[u] < Y[v]) ? 1 : -1));
          } else {
            A.push_back(X[u] + (X[u] < X[v] ? 1 : -1));
            B.push_back(Y[u] + (odd ^ (X[u] < X[v]) ? -1 : 1));
          }
          visited[v] = true;
          dfs(v);
        }
      }
    }
  };

  visited[0] = true;
  dfs(0);

  if (all_of(visited.begin(), visited.end(), [](bool b) { return b; })) {
    build(U, V, A, B);
    return 1;  
  }
  return 0;
}
