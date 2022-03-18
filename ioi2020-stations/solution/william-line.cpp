#include "stations.h"
#define MAXN 1000

std::vector<int> adj[MAXN], labels;

void dfs(int v, int p, int d) {
  labels[v] = d;
	for (int i : adj[v]) if (i != p) dfs(i, v, d + 1);
}

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
	for (int i = 0; i < n; i++) {
		adj[i].clear();
	}
	for (int i = 0; i < n - 1; i++) {
		adj[u[i]].push_back(v[i]);
		adj[v[i]].push_back(u[i]);
	}
	labels.resize(n);
	for (int i = 0; i < n; i++) {
	  if(adj[i].size() == 1) {
	    dfs(i, -1, 0);
	    return labels;
	  }
	}
	return labels;
}

int find_next_station(int s, int t, std::vector<int> c) {
  return s < t ? s + 1 : s - 1;
}
