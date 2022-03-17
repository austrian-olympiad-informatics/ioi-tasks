#include <bits/stdc++.h>
using namespace std;

static const int n_bits=20;
const long long inf = 1e18;
long long arr[1<<(n_bits+1)];
int low[1<<(n_bits+1)];
int high[1<<(n_bits+1)];
long long lazyadd[1<<(n_bits+1)];
struct segtree {
    segtree(){}
    int depth(int x) {
        return __builtin_clz(x)-11;
    }
    void init() {
        for(int i=0; i<(1<<n_bits); i++) {
            low[i+(1<<n_bits)] = i;
            high[i+(1<<n_bits)] = i;
        }
        for(int i=(1<<n_bits)-1; i>=1; i--) {
            low[i] = min(low[2*i], low[2*i+1]);
            high[i] = max(high[2*i], high[2*i+1]);
        }
        for(int i=1; i<(1<<(n_bits+1)); i++) {
            lazyadd[i] = 0;
            assert((i<<depth(i))-(1<<n_bits) == low[i]);
        }
    }

    long long value(int node) {
        arr[node] += lazyadd[node];
        if(node<(1<<n_bits)) {
            lazyadd[2*node] += lazyadd[node];
            lazyadd[2*node+1] += lazyadd[node];
        }
        lazyadd[node] = 0;
        return arr[node];
    }
    void update(int node, int left, int right, int change) {
        if(right>=high[node] && left<=low[node]) {
            lazyadd[node] += change;
        } else if(right<low[node] || left>high[node]) {
            return;
        } else {
            update(2*node, left, right, change);
            update(2*node+1, left, right, change);
            arr[node] = min(value(node*2), value(node*2+1));
        }
    }
    long long query(int node, int left, int right) {
        value(node);
        if(right>=high[node] && left<=low[node]) {
            return arr[node];
        } else if(right<low[node] || left>high[node]) {
            return inf;
        } else {
            return min(query(node*2, left, right), query(node*2+1, left, right));
        }
    }
};

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    // since v[i] is always positive
    // just do a naive segtree update
    int n = C.size();
    segtree s;
    s.init();
    int q = L.size();
    for(int i=0; i<q; i++) {
        //assert(V[i]>0);
        s.update(1, L[i], R[i], V[i]);
    }
    // do a point query to get the answer
    vector<int> ans(n);
    for(int i=0; i<n; i++) {
        ans[i] = min((long long)C[i], s.query(1, i, i));
    }
    return ans;
}
