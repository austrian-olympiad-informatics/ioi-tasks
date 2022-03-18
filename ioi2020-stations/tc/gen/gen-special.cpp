#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// gen-special [r] [k] [type] [n] [seed] [is_partial]
// types:
// 0 - stars
// 1 - k-ary trees
// 2 - caterpillars
// 3 - porcupines

vector< pair<int, int> > generate_star(int n) {
	int rt;
	if (rnd.next(2)) rt = 0;
	else rt = n - 1;
	vector< pair<int, int> > edgelist;
	for (int i = 0; i < n; i++) {
		if (rt != i) {
			edgelist.push_back(make_pair(i, rt));
			if (rnd.next(2)) {
				swap(edgelist.back().first, edgelist.back().second);
			}
		}
	}
	shuffle(edgelist.begin(), edgelist.end());
	return edgelist;
}

vector< pair<int, int> > generate_kary_tree(int n) {
	vector< pair<int, int> > edgelist;
	vector<int> order;
	for (int i = 0; i < n; i++) order.push_back(i);
	shuffle(order.begin(), order.end());
	int k = rnd.next(2, 10);
	for (int i = 1; i < n; i++) {
		edgelist.push_back(make_pair(order[i], order[i/k]));
		if (rnd.next(2)) {
			swap(edgelist.back().first, edgelist.back().second);
		}
	}
	shuffle(edgelist.begin(), edgelist.end());
	return edgelist;
}

vector< pair<int, int> > generate_caterpillar(int n) {
	vector< pair<int, int> > edgelist;
	vector<int> order;
	for (int i = 0; i < n; i++) order.push_back(i);
	shuffle(order.begin(), order.end());
	for (int i = 1; i < n; i++) {
		if (i % 2 == 0) edgelist.push_back(make_pair(order[i], order[i-2]));
		else edgelist.push_back(make_pair(order[i], order[i-1]));
		if (rnd.next(2)) {
			swap(edgelist.back().first, edgelist.back().second);
		}
	}
	shuffle(edgelist.begin(), edgelist.end());
	return edgelist;
}

vector< pair<int, int> > generate_porcupine(int n) {
	vector< pair<int, int> > edgelist;
	vector<int> order;
	for (int i = 0; i < n; i++) order.push_back(i);
	shuffle(order.begin(), order.end());
	int k = rnd.next(2, min(20, n));
	for (int i = 1; i < k; i++) edgelist.push_back(make_pair(order[i], order[rnd.next(i)]));
	for (int i = k; i < n; i++) edgelist.push_back(make_pair(order[i], order[rnd.next(k)]));
	for (int i = 0; i < (int)edgelist.size(); i++) if (rnd.next(2)) swap(edgelist[i].first, edgelist[i].second);
	shuffle(edgelist.begin(), edgelist.end());
	return edgelist;
}

vector<int> adjlist[1005];

int dfs(int x, int t, int p) {
	if (x == t) return x;
	for (int i = 0; i < (int)adjlist[x].size(); i++) {
		if (adjlist[x][i] == p) continue;
		if (dfs(adjlist[x][i], t, x) != -1) return adjlist[x][i];
	}
	return -1;
}

int main(int argc, char **argv) {
	registerGen(argc, argv, 1);
	int r = atoi(argv[1]);
	int k = atoi(argv[2]);
	int type = atoi(argv[3]);
	int n = atoi(argv[4]);
	int seed = atoi(argv[5]);
	int is_partial = atoi(argv[6]);
	printf("%d\n", r);
	for (int r1 = 0; r1 < r; r1++) {
		vector< pair<int, int> > edgelist;
		if (type == 0) edgelist = generate_star(n);
		else if (type == 1) edgelist = generate_kary_tree(n);
		else if (type == 2) edgelist = generate_caterpillar(n);
		else edgelist = generate_porcupine(n);
		
		for (int i = 0; i < n; i++) adjlist[i].clear();
		for (int i = 0; i < (int)edgelist.size(); i++) {
			adjlist[edgelist[i].first].push_back(edgelist[i].second);
			adjlist[edgelist[i].second].push_back(edgelist[i].first);
		}
		
		vector< pair< pair<int, int>, int > > queries;
		
		int sum = 0, too_little = 0, too_much = 0;
		int error_limit = 30, query_limit = 100000 / r;
		while (1) {
			int a = rnd.next(n), b = rnd.next(n);
			if (a == b) continue;
			if (adjlist[a].size() <= 1 && too_little < error_limit) {
				too_little++;
				continue;
			}
			if (sum + (int)adjlist[a].size() > query_limit) {
				if (too_much >= error_limit) break;
				too_much++;
				continue;
			}
			too_little = 0;
			too_much = 0;
			sum += adjlist[a].size();
			
			queries.push_back(make_pair(make_pair(a, b), dfs(a, b, -1)));
		}
		assert(sum <= query_limit);
		printf("%d %d\n", n, k);
		for (int i = 0; i < (int)edgelist.size(); i++) printf("%d %d\n", edgelist[i].first, edgelist[i].second);
		printf("%d\n", (int)queries.size());
		for (int i = 0; i < (int)queries.size(); i++) printf("%d %d %d\n", queries[i].first.first, queries[i].first.second, queries[i].second);
	}
	printf("%d %d\n", is_partial, seed);
}
