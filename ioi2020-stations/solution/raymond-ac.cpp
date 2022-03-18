#include "stations.h"
#include <bits/stdc++.h>
using namespace std;
vector<int> sz, labels;
vector<vector<int>> E;
int dfsSiz(int x, int p = -1) {
	sz[x] = 1;
	for (int y : E[x]) {
		if (y == p) continue;
		sz[x] += dfsSiz(y, x);
	}
	return sz[x];
}
void dfs(int x, int s, int e, int p = -1, int h = 0) {
	if (h & 1) {
		labels[x] = e--;
	} else {
		labels[x] = s++;
	}
	for (int y : E[x]) {
		if (y == p) continue;
		dfs(y, s, s + sz[y] - 1, x, h + 1);
		s += sz[y];
	}
	assert(s == e + 1);
}
vector<int> label(int n, int k, vector<int> u, vector<int> v) {
	assert(u.size() == n - 1 && v.size() == n - 1);
	sz = vector<int>(n);
	labels = vector<int>(n);
	E = vector<vector<int>>(n);
	for (int i = 0; i < n - 1; ++i) {
		E[u[i]].push_back(v[i]);
		E[v[i]].push_back(u[i]);
	}
	dfsSiz(0);
	dfs(0, 0, n - 1);
	return labels;
}

int find_next_station(int s, int t, vector<int> c) {
	vector<int> sub{s, s};
	int m = c.size();
	if (c[0] < s) {
		if (m > 1) sub[0] = c[1];
		if (t < sub[0] || t > sub[1]) return c[0];
		for (int i = 1; i + 1 < m; ++i)
			if (c[i + 1] > t) return c[i];
		return c[m - 1];
	} else {
		if (s != 0 && m > 1) sub[1] = c[m - 2];
		else if (s == 0) sub[1] = c[m - 1];
		if (t < sub[0] || t > sub[1]) return c[m - 1];
		for (int i = 0;; ++i)
			if (c[i] >= t) return c[i];
	}
}
