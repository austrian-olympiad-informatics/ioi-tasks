#include <bits/stdc++.h>
using namespace std;

vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
    // look for isolated vertices
    int m = c.size();
    int n = r.size();
    bool isolated[n];
    for(int i=0; i<n; i++) {
        isolated[i] = true;
    }

    for(int i=0; i<m; i++) {
        if(r[u[i]]==0) {
            isolated[u[i]] = false;
        }
        if(r[v[i]]==0) {
            isolated[v[i]] = false;
        }
    }

    vector<int> iso;
    for(int i=0; i<n; i++) {
        if(isolated[i]) {
            iso.push_back(i);
        }
    }
    vector<int> ans(n);
    if(!iso.empty()) {
        for(int i: iso) {
            ans[i] = true;
        }
        return ans;
    }
    // else, look for connected component
    vector<int> adj[n];
    for(int i=0; i<m; i++) {
        adj[u[i]].push_back(v[i]);
        adj[v[i]].push_back(u[i]);
    }
    // now do a bfs on every vertex
    int sz[n];
    bool visited[n];
    int smallest = n;
    for(int i=0; i<n; i++) {
        sz[i] = 0;
        for(int j=0; j<n; j++) {
            visited[j] = false;
        }
        visited[i] = true;
        queue<int> bfs;
        bfs.push(i);
        while(!bfs.empty()) {
            int next = bfs.front();
            bfs.pop();
            sz[i]++;
            for(int j: adj[next]) {
                if(!visited[j]) {
                    bfs.push(j);
                    visited[j] = true;
                }
            }
        }
        smallest = min(smallest, sz[i]);
    }

    for(int i=0; i<n; i++) {
        if(sz[i]==smallest) {
            ans[i] = 1;
        }
    }
    return ans;
}
