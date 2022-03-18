#include "stations.h"
#define MAXN 1000

int cnt;
std::vector<int> adj[MAXN], labels;

void dfs(int v, int p) {
	labels[v] = (++cnt) * MAXN;
	for (int i : adj[v]) if (i != p) dfs(i, v);
	labels[v] += cnt;
}

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
	cnt = -1;
	for (int i = 0; i < n; i++) {
		adj[i].clear();
	}
	for (int i = 0; i < n - 1; i++) {
		adj[u[i]].push_back(v[i]);
		adj[v[i]].push_back(u[i]);
	}
	labels.resize(n);
	dfs(0, -1);
	return labels;
}

int find_next_station(int s, int t, std::vector<int> c) {
	int s_in = s / MAXN, s_out = s % MAXN, t_in = t / MAXN;
	if (t_in < s_in || t_in > s_out) return c[0];
	for (int i = 0; i < (int)c.size(); i++){
		if (c[i] / MAXN < s_in) continue; // do not process parent
		if (t_in <= c[i] % MAXN) return c[i];
	}
	return -1;
}
