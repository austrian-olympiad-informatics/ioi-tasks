#include <bits/stdc++.h>
using namespace std;
const int n_max = 301000;
const int k_max = 301000;

vector<pair<int,int>> adj[n_max];
bool visited[n_max];
int original_key[n_max];
int has_key[k_max];
vector<int> blocked[k_max];

int n;
int explore(int x) {
    for(int i=0; i<n; i++) {
        visited[i] = false;
        has_key[i] = false;
        blocked[i].clear();
    }
    visited[x] = true;
    queue<int> q;
    q.push(x);
    int ans = 0;
    while(!q.empty()) {
        const int next = q.front();
        q.pop();
        ans++;

        int new_key = original_key[next];
        if(!has_key[new_key]) {
            has_key[new_key] = true;
            for(int i: blocked[new_key]) {
                if(!visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        for(pair<int,int> p: adj[next]) {
            if(has_key[p.first]) { // i have the key
                if(!visited[p.second]) { // put in the queue
                    visited[p.second] = true;
                    q.push(p.second);
                }
            } else { // it may be unblocked later
                //assert(p.first < k_max);
                blocked[p.first].push_back(p.second);
            }
        }
    }
    return ans;
}

vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
    int m = c.size();
    n = r.size();
    for(int i=0; i<n; i++) {
        original_key[i] = r[i];
    }
    for(int i=0; i<m; i++) {
        adj[u[i]].emplace_back(c[i], v[i]);
        adj[v[i]].emplace_back(c[i], u[i]);
    }
    int a[n];
    int s = n+1;
    for(int i=0; i<n; i++) {
        a[i] = explore(i);
        s = min(s, a[i]);
    }
    vector<int> ans(n);

    for(int i=0; i<n; i++) {
        if(a[i]==s) {
            ans[i] = 1;
        }
    }
    return ans;
}
