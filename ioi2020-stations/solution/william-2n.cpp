#include "stations.h"
#include <algorithm>
#define MAXN 1000

int cnt;
std::vector<int> adj[MAXN], labels;

void dfs(int v, int p, int d) {
	if(!d) labels[v] = cnt;
	++cnt;
	for (int i : adj[v]) if (i != p) dfs(i, v, d ^ 1);
	if(d) labels[v] = cnt;
	++cnt;
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
	if (s < c[0]) {
		if (t < s || t >= c.back()) return c.back();
		return *std::lower_bound(c.begin(), c.end(), t);
	}
	if (t > s || t <= c[0]) return c[0];
	return *--std::upper_bound(c.begin(), c.end(), t);
}
