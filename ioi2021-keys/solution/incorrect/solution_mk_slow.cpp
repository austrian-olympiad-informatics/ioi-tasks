#include <bits/stdc++.h>

using namespace std;

// bitset solution but without bitsets
// does the copying the "bad" way
// should still pass
const int N=300500;
const int k_max = 30;
vector<pair<int,int>> adj[N];

//bitset<k_max> mask[N];
bool mask[N][k_max];

void bfs(int z) {
    queue<int> q;
    q.push(z);
    while(!q.empty()) {
        int x = q.front();
        q.pop();
        assert(x<N);
        bool updated = false;
        do {
            updated = false;
            for(auto p: adj[x]) {
                if(mask[x][p.second]) {
                    for(int i=0; i<k_max; i++) {
                        if(mask[p.first][i] && !mask[x][i]) {
                            mask[x][i] = true;
                            updated = true;
                        }
                    }
                }
            }
        } while(updated);

        for(auto p: adj[x]) {
            bool add_to_queue = false;
            if(mask[p.first][p.second]) {
                for(int i=0; i<k_max; i++) {
                    if(mask[x][i] && !mask[p.first][i]) {
                        add_to_queue = true;
                    }
                }
                if(add_to_queue) {
                    q.push(p.first);
                }
            }
        }
    }
}

int pt[N];
int findset(int x) {
    while(pt[x]^x) {
        x = pt[x] = pt[pt[x]];
    }
    return x;
}
void unionset(int x, int y) {
    x = findset(x);
    y = findset(y);
    pt[x] = y;
}
vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
    int m = c.size();
    int n = r.size();
    for(int i=0; i<m; i++) {
        adj[u[i]].emplace_back(v[i], c[i]);
        adj[v[i]].emplace_back(u[i], c[i]);
    }
    for(int i=0; i<n; i++) {
        mask[i][r[i]] = true;
    }

    for(int i=0; i<n; i++) {
        bfs(i);
    }

    // n is the "dummy" node
    for(int i=0; i<=n; i++) {
        pt[i] = i;
    }

    for(int i=0; i<m; i++) {
        if(mask[u[i]][c[i]]) { // can pass through
            // case 1, different mask
            // then vertex u is "dead"
            bool different = false;
            for(int a=0; a<k_max; a++) {
                if(mask[u[i]][a]!=mask[v[i]][a]) {
                    different = true;
                }
            }
            if(different) {
                unionset(u[i], n);
            } else {
                // case 2: same mask
                // then they can inter-travel
                unionset(u[i], v[i]);
            }
        }
        if(mask[v[i]][c[i]]) { // can pass through
            // case 1, different mask
            // then vertex u is "dead"
            bool different = false;
            for(int a=0; a<k_max; a++) {
                if(mask[u[i]][a]!=mask[v[i]][a]) {
                    different = true;
                }
            }
            if(different) {
                unionset(v[i], n);
            } else {
                // case 2: same mask
                // then they can inter-travel
                unionset(u[i], v[i]);
            }
        }
    }
    // record down component sizes
    int component_size[n+1];
    for(int i=0; i<=n; i++) {
        component_size[i] = 0;
    }
    int smallest = n;
    for(int i=0; i<n; i++) {
        component_size[findset(i)]++;
    }


    for(int i=0; i<n; i++) {
        if(findset(i)!=findset(n)) { // is actually a sink scc
            smallest = min(smallest, component_size[findset(i)]);
        }
    }
    vector<int> ans(n);
    for(int i=0; i<n; i++) {
        if(findset(i) != findset(n) and component_size[findset(i)]==smallest) {
            ans[i] = 1;
        }
    }
    return ans;
}
