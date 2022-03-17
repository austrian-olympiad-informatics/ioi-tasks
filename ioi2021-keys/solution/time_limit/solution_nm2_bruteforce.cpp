#include <bits/stdc++.h>

#define MAXN 300000
#define MAXM 300000
#define MAXK 2000

using namespace std;

typedef pair<int,int> pi;


int n, m, p[MAXN + 5], l;
int vis[MAXN + 5];
int hasKey[MAXK + 5];

vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
	n = r.size();
	m = c.size();

	for (int i = 0; i < n; ++i){

		for (int j = 0; j < MAXK; ++j) hasKey[j] = 0;
		for (int j = 0; j < n; ++j) vis[j] = 0;

		vis[i] = 1;
		hasKey[r[i]] = 1;

		int round = 0;
		while (1){
            round++;
			int check = 0;
			for (int j = 0; j < m; ++j){
				if (hasKey[c[j]]){
					if (vis[u[j]] == 1 && vis[v[j]] == 0){
						vis[v[j]] = 1;
						hasKey[r[v[j]]] = 1;
						check++;
					}
					else if (vis[v[j]] == 1 && vis[u[j]] == 0){
						vis[u[j]] = 1;
						hasKey[r[u[j]]] = 1;
						check++;
					}
				}
			}
			if (!check) break;
		}
		//printf("i=%d round=%d\n", i, round);

		for (int j = 0; j < n; ++j) p[i] += vis[j];
	}

	int minval = n;
	vector<int> ans, vals;
	for (int i = 0; i < n; ++i){
		if (p[i] < minval){
			vals.clear();
			minval = p[i];
		}
		if (p[i] == minval){
			vals.push_back(i);
		}
	}

	ans.resize(n);
	for (int i = 0; i < vals.size(); ++i){
		ans[vals[i]] = 1;
	}

	return ans;
}


