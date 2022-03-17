#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> pi;

#define MAXN 300000
#define MAXM 300000
#define MAXK 300000

// BEGIN UFDS
int par[MAXN + 5], sz[MAXN + 5];

void init(){
	for (int i = 0; i < MAXN; ++i){
		par[i] = i;
		sz[i] = 0;
	}
}

int findPar(int x){
	if (par[x] != x) par[x] = findPar(par[x]);
	return par[x];
}

void merg(int x, int y){ // we can't merge by rank here
	par[findPar(x)] = findPar(y);
}
// END UFDS

vector<pi> adjList[MAXN + 5];
vector<int> locked[MAXK + 5];
vector<int> vals;

int homekeys[MAXN + 5], vis[MAXN+5], dead[MAXN + 5], last[MAXN + 5];
int keys[MAXK + 5];
int ans;

vector<int> reach;
vector<int> haslock;

void cleanup(){
	for (auto it : reach) keys[homekeys[it]] = 0;
	for (auto it : haslock) locked[it].clear();

	reach.clear();
	haslock.clear();
}

void runBFS(int rep, int iter){
	last[rep] = iter;
	queue<int> q;
	q.push(rep);


	while (!q.empty()){
		int u = q.front();
		q.pop();

		if (findPar(u) != rep){
			merg(rep,findPar(u));
			last[findPar(u)] = iter;
			cleanup();
			return;
		}
		if (vis[u]) continue;
		vis[u] = 1;

		reach.push_back(u);
		int nk = homekeys[u];

		if (keys[nk] == 0){
			keys[nk] = 1;
			while (!locked[nk].empty()){
				q.push(locked[nk].back());
				locked[nk].pop_back();
			}
		}

		for (auto it : adjList[u]){
			if (keys[it.second]) q.push(it.first);
			else{
				haslock.push_back(it.second);
				locked[it.second].push_back(it.first);
			}
		}
	}

	dead[rep] = 1;
	if (reach.size() < ans){
		vals.clear();
		ans = reach.size();
	}
	if (reach.size() == ans){
		for (auto it : reach) vals.push_back(it);
	}
	cleanup();
}

vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
	int n = r.size(), m = c.size();
	init();

	for (int i = 0; i < n; ++i) homekeys[i] = r[i];

	for (int i = 0; i < m; ++i){
		adjList[u[i]].push_back(pi(v[i],c[i]));
		adjList[v[i]].push_back(pi(u[i],c[i]));
	}

	ans = n+1;

	for (int iter = 1;; ++iter){
		for (int i = 0; i < n; ++i) vis[i] = 0;
		int check = 0;
		for (int i = 0; i < n; ++i){
			if (findPar(i) == i && !dead[i]){
				if (last[i] != iter){
					runBFS(i,iter);
					check = 1;
				}
			}
		}
		if (!check) break;
		//if(iter>=18) break;
	}

	vector<int> ret(n);
	for(auto it : vals) ret[it] = 1;

	return ret;
}
