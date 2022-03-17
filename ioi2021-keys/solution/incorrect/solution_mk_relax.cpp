#include <bits/stdc++.h>

// bellman ford style relax
// shouldnt pass
// even with random
// should use bfs
using namespace std;

const int N=300050;
vector<pair<int,int>> adj[N];

int mask[N];

void relax(mt19937 rng) {
    vector<int> p(N);
    for(int i=0; i<N; i++) {
        p[i] = i;
    }
    shuffle(p.begin(), p.end(), rng);
    for(int i=0; i<N; i++) {
        int j = p[i];
        for(auto p: adj[j]) {
            if(mask[j]&(1<<p.second)) {
                mask[j] |= mask[p.first];
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
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int m = c.size();
    int n = r.size();
    for(int i=0; i<m; i++) {
        adj[u[i]].emplace_back(v[i], c[i]);
        adj[v[i]].emplace_back(u[i], c[i]);
    }
    for(int i=0; i<n; i++) {
        mask[i] = (1<<r[i]); // initialize the mask to your own key
    }

    // bellman ford style relax
    // probably wont work
    for(int i=0; i<100; i++) {
        relax(rng);
    }

    for(int i=0; i<m; i++) {
        if(mask[u[i]] &(1<<c[i])) { // if u->v, then u's mask should be a supermask of v
            assert((mask[u[i]] | mask[v[i]]) == mask[u[i]]);
        }
        if(mask[v[i]] &(1<<c[i])) {
            assert((mask[u[i]] | mask[v[i]]) == mask[v[i]]);
        }
    }

    // n is the "dummy" node
    for(int i=0; i<=n; i++) {
        pt[i] = i;
    }

    for(int i=0; i<m; i++) {
        if(mask[u[i]] &(1<<c[i])) { // can pass through
            // case 1, different mask
            // then vertex u is "dead"
            if(mask[u[i]] != mask[v[i]]) {
                unionset(u[i], n);
            } else {
                // case 2: same mask
                // then they can inter-travel
                unionset(u[i], v[i]);
            }
        }
        if(mask[v[i]] &(1<<c[i])) { // can pass through
            // case 1, different mask
            // then vertex u is "dead"
            if(mask[u[i]] != mask[v[i]]) {
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
