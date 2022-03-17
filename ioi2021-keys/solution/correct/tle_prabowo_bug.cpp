#include "keys.h"

#include <functional>
#include <queue>
#include <vector>

std::vector<int> find_reachable(std::vector<int> r, std::vector<int> u,
                                std::vector<int> v, std::vector<int> c) {
  int n = r.size(), m = u.size();

  // DSU
  std::vector<int> roots(n);
  std::function<int(int)> findRoot = [&](int u) {
    if (u == roots[u]) return u;
    return roots[u] = findRoot(roots[u]);
  };
  std::function<void(int, int)> merge = [&](int u, int v) {
    if (findRoot(u) != findRoot(v)) {
      roots[roots[u]] = roots[v];
    }
  };
  for (int i = 0; i < n; ++i) {
    roots[i] = i;
  }

  // Adj list init
  std::vector<std::vector<std::pair<int, int>>> adj(n);
  for (int j = 0; j < m; ++j) {
    adj[u[j]].emplace_back(v[j], c[j]);
    adj[v[j]].emplace_back(u[j], c[j]);
  }

  std::vector<bool> vis(n), keys(n);
  std::vector<std::vector<int>> pendingCorridors(n);
  std::function<std::vector<int>(int)> bfs = [&](int st) {
    std::vector<int> locks;
    std::vector<int> reach;

    [&]() {
      std::queue<int> q;
      q.push(st);

      while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (vis[u]) continue;
        vis[u] = true;

        reach.push_back(u);
        keys[r[u]] = true;
        for (int v : pendingCorridors[r[u]]) q.push(v);
        pendingCorridors[r[u]].clear();

        if (findRoot(u) != findRoot(st)) return;

        for (auto [v, c] : adj[u]) {
          if (keys[c]) {
            q.push(v);
          } else {
            locks.push_back(c);
            pendingCorridors[c].push_back(v);
          }
        }
      }
    }();

    for (int u : reach) vis[u] = false, keys[r[u]] = false;
    for (int c : locks) pendingCorridors[c].clear();
    return reach;
  };

  std::vector<std::vector<int>> sinks;

  bool hasMerge;
  do {
    hasMerge = false;

    for (int st = 0; st < n; ++st) {
      if (findRoot(st) != st) continue;
      std::vector<int> component = bfs(st);
      if (findRoot(component.back()) != st) {
        merge(st, roots[component.back()]);
        hasMerge = true;
      } else {
        sinks.emplace_back(component);
      }
    }
  } while (hasMerge);

  int minimum_reachable = n;
  for (const std::vector<int> &sink : sinks) {
    if (static_cast<int>(sink.size()) < minimum_reachable) {
      minimum_reachable = sink.size();
    }
  }

  std::vector<int> p(n);
  for (const std::vector<int> &sink : sinks) {
    if (static_cast<int>(sink.size()) != minimum_reachable) continue;
    for (int node : sink) p[node] = 1;
  }
  return p;
}
