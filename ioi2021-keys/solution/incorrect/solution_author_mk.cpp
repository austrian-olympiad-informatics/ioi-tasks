#include <bits/stdc++.h>
#include "keys.h"

using namespace std;

// Model solution is author's solution for now

#define pii pair<int,int>
#define pb push_back

const int N=300050;

struct DSU{
	int p[N];
	DSU(){}
	void init(int n){for(int i=0;i<n;i++)p[i]=i;}
	int Find(int x){return p[x]==x?x:p[x]=Find(p[x]);}
	void Union(int x,int y){p[Find(x)]=Find(y);}
}DS;

vector<pii> E[N];

int last[N],ans,has[N];
bool was[N],done[N];

vector<int> Xs;

void BFS(int special,int iteration){
	last[special]=iteration;
	vector<pii> locked;
	queue<int> q;
	q.push(special);
	int keys=0;
	vector<int> reached;
	while(q.size()){
		int u=q.front();
		q.pop();

		if(DS.Find(u)!=special){
			DS.p[special]=DS.Find(u);
			last[DS.Find(u)]=iteration;
			return;
		}
		if(was[u])continue;
		was[u]=true;
		reached.pb(u);
		int pre=keys;
		keys|=has[u];
		if(keys!=pre){
			vector<pii> tmp;
			for(pii e:locked){
                if((e.second&keys)==e.second){
					q.push(e.first);
                }else tmp.pb(e);
			}
			locked=tmp;
		}
		for(pii e:E[u]){
			if((e.second&keys)==e.second){
				q.push(e.first);
			}else locked.pb(e);
		}
	}
	done[special]=true;
	if(reached.size()<ans)Xs.clear(),ans=reached.size();
	if(ans==reached.size()){
		for(int X:reached)Xs.pb(X);
	}
}

vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
	int n = r.size(), m = c.size();
	DS.init(n);

	for (int i = 0; i < n; ++i){
		has[i] = (1 << r[i]);
	}

	for (int j = 0; j < m; ++j){
		E[u[j]].pb({v[j],(1 << c[j])});
		E[v[j]].pb({u[j],(1 << c[j])});
	}

	ans=n+1;

	for(int iteration=1;;iteration++){
		bool work=false;
		for(int i=0;i<n;i++)was[i]=false;
		for(int i=0;i<n;i++){
			if(DS.Find(i)==i&&!done[i]){ //i is special vertex
				if(last[i]!=iteration){
					BFS(i,iteration);
					work=true;
				}
			}
		}
		if(!work)break;
	}

	vector<int> ret(n);
	for(int X:Xs) ret[X] = 1;

	return ret;
}


