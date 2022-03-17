#include <bits/stdc++.h>
using namespace std;

// uses the faster randomized
// should still fail as the runtime is
// theoretically n^1.5
const int n_max = 300500;

int visited_list[n_max];
bool has_key[n_max];
bool visited[n_max];
int homekey[n_max];
int a[n_max];
pair<int,int> adj[2*n_max];
pair<int,int>* z[n_max];
bool finished[n_max];

// essentially, this stores the blocked position
// vector<int> blocked[max_keys] with push/pop is too slow
// we need to manage this ourself instead of relying on library
int blocked_counter[n_max];
int blocked3[n_max]; // stores the actual values
int* blocked2[n_max]; // stores pointers


int counter = 0;

void cleanup() {
    for(int j=0; j<counter; j++) {
        int i = visited_list[j];
        visited[i] = false;
        has_key[homekey[i]] = false;
        for(pair<int,int>* p = z[i]; p<z[i+1]; p++) {
            blocked_counter[p->first] = 0;
        }
    }
}

int dfs_stack[n_max];
int explore(int x) { // returns min(limit, size of component)
    int* pt = dfs_stack;
    *pt = x;
    pt++; // stack always points to 1 higher
    counter = 1;
    visited_list[0] = x;

    visited[x] = true;
    while(pt > dfs_stack) {
        pt--; // pop item
        int next = *pt;

        if(!has_key[homekey[next]]) {
            has_key[homekey[next]] = true;
            for(int j=0; j<blocked_counter[homekey[next]]; j++) {
                int i = blocked2[homekey[next]][j];
                if(!visited[i]) {
                    if(finished[i]) {
                        return 1e6;
                    }
                    visited[i] = true;
                    visited_list[counter] = i;
                    counter++;
                    *pt = i; // push item
                    pt++;
                }
            }
            blocked_counter[homekey[next]] = 0;
        }
        for(pair<int,int>* p = z[next]; p<z[next+1]; p++) {
            if(visited[p->second]) continue;
            if(has_key[p->first]) {
                if(finished[p->second]) {
                    return 1e6;
                }
                visited_list[counter] = p->second;
                visited[p->second] = true;

                counter++;
                *pt = p->second; // push item
                pt++;
            } else {
                blocked2[p->first][blocked_counter[p->first]] = p->second;
                blocked_counter[p->first]++;
            }
        }
    }
    for(int j=0; j<counter; j++) {
        int i = visited_list[j];
        a[i] = min(a[i], counter);
    }
    return counter;
}

tuple<int,int,int> t[2*n_max];
int deg[n_max];
vector<int> find_reachable(vector<int> r, vector<int> u, vector<int> v, vector<int> c) {
    int m = c.size();
    int n = r.size();
    for(int i=0; i<n; i++) {
        a[i] = 1e6;
    }

    for(int i=0; i<m; i++) {
        //adj[u[i]].emplace_back(c[i], v[i]);
        //adj[v[i]].emplace_back(c[i], u[i]);
        blocked_counter[c[i]]++;
        t[2*i] = make_tuple(u[i], v[i], c[i]);
        t[2*i+1] = make_tuple(v[i], u[i], c[i]);
        deg[u[i]]++;
        deg[v[i]]++;
    }
    sort(t, t+2*m);
    z[0] = adj;
    for(int i=0; i<2*m; i++) {
        adj[i] = make_pair(get<2>(t[i]), get<1>(t[i]));
        if(i!=0 && get<0>(t[i])!=get<0>(t[i-1])) {
            for(int j=get<0>(t[i-1])+1; j<=get<0>(t[i]); j++) {
                z[j] = adj + i;
            }
        }
    }
    z[get<0>(t[2*m-1])+1] = adj + 2*m;

    blocked2[0] = blocked3;
    for(int i=1; i<n_max; i++) {
        blocked2[i] = blocked2[i-1] + blocked_counter[i-1];
    }
    for(int i=0; i<n_max; i++) {
        blocked_counter[i] = 0;
    }
    for(int i=0; i<n; i++) {
        homekey[i] = r[i];
    }
    int s = n+1;
    vector<pair<int,int>> deg_order(n);
    for(int i=0; i<n; i++) {
        deg_order[i] = make_pair(-deg[i], i);
    }
    sort(deg_order.begin(), deg_order.end()); // large degree appears first
    vector<int> p(n);
    for(int i=0; i<n; i++) {
        p[i] = deg_order[i].second;
    }

    for(int i: p) {
        if(!finished[i]) {
            s = min(s, explore(i));
            finished[i] = true;
            cleanup();
        }
    }
    for(int i=0; i<n; i++) {
        if(!finished[i]) {
            s = min(s, explore(i));
            finished[i] = true;
            cleanup();
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
