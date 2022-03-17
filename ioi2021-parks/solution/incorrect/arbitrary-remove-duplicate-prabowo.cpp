#include "parks.h"

#include <algorithm>
#include <queue>
#include <vector>

const int kMax = 200005;

std::vector<std::pair<int, int>> fountains[kMax];

bool isConnected(std::vector<std::vector<int>> adj) {
  int n = adj.size();
  std::vector<bool> vis(n);
  std::queue<int> q;
  q.push(0); vis[0] = true;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int v : adj[u]) {
      if (vis[v]) continue;
      vis[v] = true;
      q.push(v);
    }
  }

  return all_of(vis.begin(), vis.end(), [](bool visited) { return visited; });
}

int construct_roads(std::vector<int> x, std::vector<int> y) {
  int n = x.size();
  for (int i = 0; i < n; ++i) {
    fountains[x[i]].emplace_back(y[i], i);
  }
  for (int i = 0; i < kMax; ++i) {
    std::sort(fountains[i].begin(), fountains[i].end());
  }
  
  std::vector<std::tuple<int, int, int, int>> pathways;
  std::vector<std::vector<int>> adj(n);
  for (int i = 0; i < n; ++i) {
    auto it = std::lower_bound(fountains[x[i] + 2].begin(), fountains[x[i] + 2].end(),
                               std::make_pair(y[i], -1));
    if (it != fountains[x[i] + 2].end() && it->first == y[i]) {
      int ny = x[i] + y[i] & 2 ? y[i] + 1 : y[i] - 1;
      pathways.emplace_back(x[i] + 1, ny, i, it->second);
      adj[i].emplace_back(it->second);
      adj[it->second].emplace_back(i);
    }
    it = std::lower_bound(fountains[x[i]].begin(), fountains[x[i]].end(),
                          std::make_pair(y[i] + 2, -1));
    if (it != fountains[x[i]].end() && it->first == y[i] + 2) {
      int nx = x[i] + y[i] & 2 ? x[i] - 1 : x[i] + 1;
      pathways.emplace_back(nx, y[i] + 1, i, it->second);
      adj[i].emplace_back(it->second);
      adj[it->second].emplace_back(i);
    }
  }

  if (!isConnected(adj)) return 0;

  std::sort(pathways.begin(), pathways.end());

  std::vector<int> u, v, a, b;
  for (int i = 0; i < static_cast<int>(pathways.size()); ++i) {
    if (i > 0 && std::get<0>(pathways[i]) == std::get<0>(pathways[i - 1]) &&
        std::get<1>(pathways[i]) == std::get<1>(pathways[i - 1])) {
      continue;
    }
    
    u.emplace_back(std::get<2>(pathways[i]));
    v.emplace_back(std::get<3>(pathways[i]));
    a.emplace_back(std::get<0>(pathways[i]));
    b.emplace_back(std::get<1>(pathways[i]));
  }

  build(u, v, a, b);
  return 1;
}
