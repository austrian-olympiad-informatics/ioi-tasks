#include <bits/stdc++.h>
using namespace std;

vector<int> find_reachable(
    vector<int> R, vector<int> U, vector<int> V, vector<int> C) {
  int N = R.size();
  int M = U.size();
  
  vector<vector<pair<int, int>>> adj(N);
  for (int i = 0; i < M; ++i) {
    adj[U[i]].push_back(make_pair(V[i], C[i]));
    adj[V[i]].push_back(make_pair(U[i], C[i]));
  }

  vector<int> P(N);
  for (int i = 0; i < N; ++i) {
    vector<bool> keys_collected(N, false);
    while (true) {
      bool new_key_collected = false;
      vector<bool> visited(N, false);
      queue<int> bfs;
      bfs.push(i);

      while (bfs.size() > 0) {
        int u = bfs.front();
        bfs.pop();
        if (visited[u]) {
          continue;
        }
        visited[u] = true;
        if (!keys_collected[R[u]]) {
          keys_collected[R[u]] = new_key_collected = true;
        }
        for (auto [v, k] : adj[u]) {
          if (keys_collected[k]) {
            bfs.push(v);
          }
        }
      }
      if (!new_key_collected) {
        P[i] = count(visited.begin(), visited.end(), true);
        break;
      }
    }
  }

  vector<int> res(N);
  for (int i = 0; i < N; ++i) {
    res[i] = (P[i] == *min_element(P.begin(), P.end())) ? 1 : 0;
  }
  return res;
}
