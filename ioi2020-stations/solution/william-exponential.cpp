#include "stations.h"
#define MAXN 1000

std::vector<int> adj[MAXN], labels;

int cnt;

void dfs(int v, int p, int d) {
  labels[v] = d;
	for (int i : adj[v]) if (i != p) dfs(i, v, (d << 3) + (++cnt));
}

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
	cnt = 0;
	for (int i = 0; i < n; i++) {
		adj[i].clear();
	}
	for (int i = 0; i < n - 1; i++) {
		adj[u[i]].push_back(v[i]);
		adj[v[i]].push_back(u[i]);
	}
	labels.resize(n);
	dfs(0, -1, 0);
	return labels;
}

int find_next_station(int s, int t, std::vector<int> c) {
	if (t < s) {
		return s >> 3;
	}
	while (s < t) {
		if ((t >> 3) == s) {
			return t;
		}
		t >>= 3;
	}
	return s >> 3;
}
