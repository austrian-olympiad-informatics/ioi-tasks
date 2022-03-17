/* Keep finding SCCs on edges we have keys for and DSU unioning the SCCs. */
#include <bits/stdc++.h>
using namespace std;
pair<vector<int>, vector<int>> calcSccs(const vector<vector<int>>& E) {
	int n = E.size();
	vector<bool> used(n);
	vector<int> comp(n, -1);
	stack<int> S;
	vector<vector<int>> ER(n);
	function<void(int)> dfs = [&](int x) {
		if (used[x]) return;
		used[x] = true;
		for (int y : E[x]) dfs(y);
		S.push(x);
	};
	function<void(int, int)> dfs2 = [&](int x, int c) {
		if (comp[x] != -1) return;
		comp[x] = c;
		for (int y : ER[x]) dfs2(y, c);
	};
	for (int x = 0; x < n; ++x) dfs(x);
	for (int x = 0; x < n; ++x)
		for (int y : E[x]) ER[y].push_back(x);
	vector<int> sccRoots;
	while (!S.empty()) {
		int x = S.top();
		S.pop();
		dfs2(x, x);
		if (comp[x] == x) sccRoots.push_back(x);
	}
	return {comp, sccRoots};
}
struct DisjointSet {
	vector<int> par;
	DisjointSet(int n) : par(n, -1) {}
	int get(int a) {
		if (par[a] < 0) return a;
		return par[a] = get(par[a]);
	}
	int getSize(int a) { return -par[get(a)]; }
	bool join(int a, int b) {
		int pa = get(a), pb = get(b);
		if (pa == pb) return false;
		par[pb] += par[pa];
		par[pa] = pb;
		return true;
	}
};
vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
	int m = u.size();
	int n = r.size();
	vector<set<int>> keys(n);
	for (int i = 0; i < n; ++i) keys[i].insert(r[i]);
	DisjointSet dsu(n);
	bool update = true;
	while (update) {
		update = false;
		vector<vector<int>> E(n);
		for (int i = 0; i < m; ++i) {
			int a = dsu.get(u[i]), b = dsu.get(v[i]);
			if (keys[a].count(c[i])) E[a].push_back(b);
			if (keys[b].count(c[i])) E[b].push_back(a);
		}
		vector<int> comp, sccRoots;
		tie(comp, sccRoots) = calcSccs(E);
		for (int i = 0; i < n; ++i) {
			int a = dsu.get(i), b = dsu.get(comp[i]);
			if (dsu.join(a, b)) {
				update = true;
				if (keys[a].size() > keys[b].size()) swap(a, b);
				keys[b].insert(keys[a].begin(), keys[a].end());
				swap(keys[b], keys[dsu.get(a)]);
			}
		}
	}
	vector<bool> isLeafScc(n, true);
	for (int i = 0; i < m; ++i) {
		int a = dsu.get(u[i]), b = dsu.get(v[i]);
		if (a != b) {
			if (keys[a].count(c[i])) isLeafScc[a] = false;
			if (keys[b].count(c[i])) isLeafScc[b] = false;
		}
	}
	int mnScc = INT_MAX;
	for (int i = 0; i < n; ++i) {
		if (dsu.get(i) != i) continue;
		if (!isLeafScc[i]) continue;
		mnScc = min(mnScc, dsu.getSize(i));
	}
	assert(mnScc <= n);
	vector<int> ans(n);
	for (int i = 0; i < n; ++i) {
		ans[i] = isLeafScc[dsu.get(i)] && dsu.getSize(i) == mnScc;
	}
	return ans;
}
