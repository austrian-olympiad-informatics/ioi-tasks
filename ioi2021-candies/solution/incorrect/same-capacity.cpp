#include <bits/stdc++.h>

using namespace std;
#define low(i) (i<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)
#define high(i) ((i+1)<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)-1

const int n_bits=20;
const long long inf = 1e18;
long long minseg[1<<(n_bits+1)];
long long maxseg[1<<(n_bits+1)];
long long lazyadd[1<<(n_bits+1)];

struct segtree {
    segtree(int q) {
        for(int i=q+1; i<(1<<n_bits); i++) {
            minseg[i+(1<<n_bits)] = inf;
            maxseg[i+(1<<n_bits)] = -inf;
        }
        for(int i=(1<<n_bits)-1; i>=1; i--) {
            minseg[i] = min(minseg[2*i], minseg[2*i+1]);
            maxseg[i] = max(maxseg[2*i], maxseg[2*i+1]);
        }
        for(int i=0; i<(1<<(n_bits+1)); i++) {
            lazyadd[i] = 0;
        }
    }
    void value(int node) {
        minseg[node] += lazyadd[node];
        maxseg[node] += lazyadd[node];
        if(node<(1<<n_bits)) {
            lazyadd[2*node] += lazyadd[node];
            lazyadd[2*node+1] += lazyadd[node];
        }
        lazyadd[node] = 0;
    }

    void update(int node, int left, int right, long long change) {
        value(node);
        if(right>=high(node) && left<=low(node)) {
            lazyadd[node] += change;
        } else if(right<low(node) || left>high(node)) {
            return;
        } else {
            update(2*node, left, right, change);
            update(2*node+1, left, right, change);
            value(node*2);
            value(node*2+1);
            minseg[node] = min(minseg[node*2], minseg[node*2+1]);
            maxseg[node] = max(maxseg[node*2], maxseg[node*2+1]);
        }
    }

    void update(int left, int right, long long change) {
        update(1, left, right, change);
    }

    long long minquery(int node, int left, int right) {
        value(node);
        if(right>=high(node) && left<=low(node)) {
            return minseg[node];
        } else if(right<low(node) || left>high(node)) {
            return inf;
        } else {
            return min(minquery(node*2, left, right), minquery(node*2+1, left, right));
        }
    }

    long long maxquery(int node, int left, int right) {
        value(node);
        if(right>=high(node) && left<=low(node)) {
            return maxseg[node];
        } else if(right<low(node) || left>high(node)) {
            return -inf;
        } else {
            return max(maxquery(node*2, left, right), maxquery(node*2+1, left, right));
        }
    }

    long long range(int left, int right) {
        return maxquery(1, left, right) - minquery(1, left, right); // gives the difference between max and min
    }

    void reset_negative(int node) {
        value(node);
        if(maxseg[node]<0) {
            update(low(node), high(node), -maxseg[node]); // increment by corresponding amount
            assert(maxseg[node]==0);
        }
        if(minseg[node] >= 0) {
            return;
        }
        reset_negative(2*node);
        reset_negative(2*node+1);
    }
    void reset_overflow(int node, int cap) {
        value(node);
        assert(lazyadd[node]==0);
        if(minseg[node] > cap) {
            update(low(node), high(node), cap - minseg[node]); // decrease value
            assert(minseg[node]==cap);
        }
        if(maxseg[node] <= cap) {
            return;
        }
        reset_overflow(2*node, cap);
        reset_overflow(2*node+1, cap);
    }
};

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    int q = L.size();

    segtree s(n);

    for(int i = 0; i < q; ++i) {
        s.update(1, L[i], R[i], V[i]); // segtree stores values as if the boxes have infinite capacity
        if(V[i]<0) {
            s.reset_negative(1);
        } else {
            s.reset_overflow(1, C[0]);
        }
    }
    vector<int> ans(n);
    for(int i=0; i<n; i++) {
        ans[i] = min(s.maxquery(1, i, i), (long long)C[i]);
    }
    return ans;
}
