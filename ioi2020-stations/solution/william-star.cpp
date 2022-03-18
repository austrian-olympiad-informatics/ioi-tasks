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
	int centre = 0;
	for (int i = 0; i < n; i++) {
	  if(adj[i].size() > 2) {
	    centre = i;
	    break;
	  }
	}
	labels[centre] = 0;
	for (int i = 0; i < (int)adj[centre].size(); i++) {
	  dfs(adj[centre][i], centre, i * MAXN + 1); // encode with branch * MAXN + index
	}
	return labels;
}

int find_next_station(int s, int t, std::vector<int> c) {
  if (s == 0) { // special case for centre
    return t / MAXN * MAXN + 1;
  } else if (s / MAXN == t / MAXN) { // same branch
    return s < t ? s + 1 : s - 1;
  } else return c[0]; // different branch, move towards smaller index or centre
}
