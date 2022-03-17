#include <bits/stdc++.h>
using namespace std;

const int n_max = 300500;

vector<int> keylist;
vector<int> visited_list;
vector<int> blocked_list;
bool has_key[n_max];
bool visited[n_max];
int homekey[n_max];
int a[n_max];
vector<pair<int,int>> adj[n_max];
bool finished[n_max];

vector<int> blocked[n_max];

void cleanup() {

    for(int i: blocked_list) {
        blocked[i].clear();
    }
    blocked_list.clear();

    for(int i: visited_list) {
        visited[i] = false;
        has_key[homekey[i]] = false;
    }
    visited_list.clear();
    keylist.clear();
}

stack<int> st;
void reverse_search(int x) {
    st.push(x);
    while(!st.empty()) {
        x = st.top();
        st.pop();
        for(auto p: adj[x]) {
            if(homekey[p.second] == p.first and !finished[p.second]) {
                finished[p.second] = true;
                st.push(p.second);
            }
        }
    }
}

int explore(int x) {
    queue<int> bfs;
    bfs.push(x);
    int counter = 1;

    visited[x] = true;
    while(!bfs.empty()) {
        int next = bfs.front();
        bfs.pop();
        if(!has_key[homekey[next]]) {
            has_key[homekey[next]] = true;
            for(int i: blocked[homekey[next]]) {
                if(!visited[i]) {
                    visited_list.push_back(i);
                    visited[i] = true;
                    if(finished[i]) {
                        return 1e6;
                    }
                    counter++;
                    bfs.push(i);
                }
            }
            blocked[homekey[next]].clear();
        }
        for(pair<int,int> p: adj[next]) {
            if(visited[p.second]) continue;
            if(has_key[p.first]) {
                visited_list.push_back(p.second);
                visited[p.second] = true;
                if(finished[p.second]) {
                    return 1e6;
                }
                counter++;
                bfs.push(p.second);
            } else {
                blocked_list.push_back(p.first);
                blocked[p.first].push_back(p.second);
            }
        }
    }
    for(int i: visited_list) {
        a[i] = min(a[i], counter);
    }
    reverse_search(x);
    return counter;
}

vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int m = c.size();
    int n = r.size();
    for(int i=0; i<n; i++) {
        a[i] = 1e6;
    }
    for(int i=0; i<m; i++) {
        adj[u[i]].emplace_back(c[i], v[i]);
        adj[v[i]].emplace_back(c[i], u[i]);
    }
    for(int i=0; i<n; i++) {
        homekey[i] = r[i];
    }
    int s = n+1;

    // bug: instead of distributing them according to their degrees
    // make a uniformly random permutation instead
    vector<int> p(n);
    for(int i=0; i<n; i++) {
        p[i] = i;
    }
    shuffle(p.begin(), p.end(), rng);
    for(int i: p) {
        if(!finished[i]) {
            a[i] = min(a[i], explore(i));
            finished[i] = true;
            visited[i] = false;
            has_key[homekey[i]] = false;
            cleanup();
            s = min(s, a[i]);
        }
    }

    vector<int> ans(n);

    for(int i=0; i<n; i++) {
        if(a[i]==s) {
            ans[i] = 1;
        }
    }
    return ans;
}
