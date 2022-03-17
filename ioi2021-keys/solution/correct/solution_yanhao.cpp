#include <bits/stdc++.h>
using namespace std;
const int n_max = 300500;
const int k_max = 300500;

vector<pair<int,int>> adj[n_max];
bool visited[n_max];
int original_key[n_max];
int has_key[k_max];
int cc[n_max];
vector<int> blocked[k_max];

int ufds[n_max];
int findset(int x) {
    while(ufds[x] != x) {
        x = ufds[x] = ufds[ufds[x]];
    }
    return x;
}

void unionset(int x, int y) {
    ufds[findset(x)] = findset(y);
}

void reset() {
    for(int i=0; i<n_max; i++) {
        visited[i] = false;
    }
}

vector<int> ans2;
vector<int> keylist;
vector<int> v2;
vector<pair<int,int>> delayed_merge;

bool change = false;

void compress(int a, int b) {
    change = true;
    //unionset(a, b);
    delayed_merge.emplace_back(a,b);
    for(int k: keylist) {
        has_key[k] = false;
        blocked[k].clear();
    }
    keylist.clear();
}
int zz = 1e9;
void explore(int x) {
    keylist.clear();
    x = findset(x);
    if(visited[x]) return;
    visited[x] = true;
    queue<int> q;
    q.push(x);
    v2.push_back(x);
    int cc_count = 1;
    while(!q.empty()) {
        const int next = q.front();
        ans2.push_back(next);
        if(findset(next) != findset(x)) { // different component, merge, cleanup and bye
            compress(x, next);
            //assert(findset(x)!=x);
            return;
        }
        q.pop();
        for(pair<int,int> p: adj[next]) {
            if(has_key[p.first]) { // i have the key
                if(!visited[p.second]) { // put in the queue
                    visited[p.second] = true;
                    q.push(p.second);
                    v2.push_back(p.second);
                    cc_count++;
                } else if(findset(p.second) != findset(x)) {
                    compress(x, p.second);
                    return;
                }
            } else { // it may be unblocked later
                //assert(p.first < k_max);
                blocked[p.first].push_back(p.second);
                keylist.push_back(p.first);
            }
        }
        int new_key = original_key[next];
        has_key[new_key] = true;
        keylist.push_back(new_key);
        for(int i: blocked[new_key]) {
            if(!visited[i]) {
                visited[i] = true;
                q.push(i);
                v2.push_back(i);
                cc_count++;
            } else if(findset(i) != findset(x)) {
                compress(x, i);
                return;
            }
        }
        blocked[new_key].clear();
    }
    cc[x] = cc_count;
    zz = min(zz, cc_count);
    for(int k: keylist) {
        has_key[k] = false;
        blocked[k].clear();
    }
    keylist.clear();

    return;
}

vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
    int m = c.size();
    int n = r.size();
    for(int i=0; i<n; i++) {
        original_key[i] = r[i];
        ufds[i] = i;
    }
    for(int i=0; i<m; i++) {
        adj[u[i]].emplace_back(c[i], v[i]);
        adj[v[i]].emplace_back(c[i], u[i]);
    }


    for(int i=0; i<20; i++) {
        reset();
        change = false;
        for(int j=0; j<n; j++) {
            if(j == findset(j)) {
                explore(j);
                for(int a: v2) {
                    visited[a] = false;
                }
                v2.clear();
            }
        }
        for(pair<int,int> pp: delayed_merge) {
            unionset(pp.first, pp.second);
        }
        if(!change) break;
    }
    int a = n+1;
    for(int i=0; i<n; i++) {
        if(i==findset(i)) {
            a = min(a, cc[i]);
            assert(a>0);
        }
    }
    vector<int> ans(n);
    ans2.clear();
    reset();
    for(int i=0; i<n; i++) {
        if(i==findset(i) and cc[findset(i)] == a) {
            explore(i);
        }
    }
    for(int i: ans2) {
        ans[i] = 1;
    }
    return ans;
}
