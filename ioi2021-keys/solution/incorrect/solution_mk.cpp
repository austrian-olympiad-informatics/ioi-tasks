#include <bits/stdc++.h>
/*
 basic idea: mask[i] stores all the keys you can possibly grab, starting at i
 initially, mask[i] = (1<<r[i]), you only have your own key
 the main body is done in the dfs function
 firstly, i grab all keys from my neighbors
 once i do so, i check my neighbors
 if any of my neighbors can gain a key from me
 i recursively pass this key over
 once we solve for mask[i] for all i
 it becomes a very simple directed graph
 time complexity O(mk)
*/
using namespace std;

const int N=300050;
vector<pair<int,int>> adj[N];

int mask[N];

void bfs(int z) {
    queue<int> q;
    q.push(z);
    while(!q.empty()) {
        int x = q.front();
        q.pop();
        int mask_init;
        assert(x<N);
        do {
            mask_init = mask[x];
            for(auto p: adj[x]) {
                if((mask[x] & (1<<p.second))) {
                    mask[x] |= mask[p.first];
                }
            }
        } while(mask[x]!=mask_init);

        for(auto p: adj[x]) {
            if((mask[p.first] & (1<<p.second))!=0 and (mask[p.first]|mask[x])!=mask[p.first]) {
                mask[p.first] = (mask[p.first] | mask[x]);
                q.push(p.first);
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
        mask[i] = (1<<r[i]); // initialize the mask to your own key
    }

    for(int i=0; i<n; i++) {
        bfs(i);
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
