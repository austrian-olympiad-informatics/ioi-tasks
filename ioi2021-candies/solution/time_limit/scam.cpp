#include <bits/stdc++.h>

using namespace std;
#define low(i) (i<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)
#define high(i) ((i+1)<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)-1

const int n_bits=20;
const long long inf = 1e18;
long long minseg[2][1<<(n_bits+1)];
long long maxseg[2][1<<(n_bits+1)];
long long lazyadd[2][1<<(n_bits+1)];

struct segtree {
    int id;
    segtree(int q, int _id) {
        id = _id;
        for(int i=q+1; i<(1<<n_bits); i++) {
            minseg[id][i+(1<<n_bits)] = inf;
            maxseg[id][i+(1<<n_bits)] = 0;
        }
        for(int i=0; i<=q; i++) {
            minseg[id][i+(1<<n_bits)] = 0;
            maxseg[id][i+(1<<n_bits)] = 0;
        }
        for(int i=(1<<n_bits)-1; i>=1; i--) {
            minseg[id][i] = min(minseg[id][2*i], minseg[id][2*i+1]);
            maxseg[id][i] = max(maxseg[id][2*i], maxseg[id][2*i+1]);
        }
        for(int i=0; i<(1<<(n_bits+1)); i++) {
            lazyadd[id][i] = 0;
        }
    }
    void value(int node) {
        minseg[id][node] += lazyadd[id][node];
        maxseg[id][node] += lazyadd[id][node];
        if(node<(1<<n_bits)) {
            lazyadd[id][2*node] += lazyadd[id][node];
            lazyadd[id][2*node+1] += lazyadd[id][node];
        }
        lazyadd[id][node] = 0;
    }

    void update(int node, int left, int right, long long change) {
        value(node);
        if(right>=high(node) && left<=low(node)) {
            lazyadd[id][node] += change;
        } else if(right<low(node) || left>high(node)) {
            return;
        } else {
            update(2*node, left, right, change);
            update(2*node+1, left, right, change);
            value(node*2);
            value(node*2+1);
            minseg[id][node] = min(minseg[id][node*2], minseg[id][node*2+1]);
            maxseg[id][node] = max(maxseg[id][node*2], maxseg[id][node*2+1]);
        }
    }

    void update(int left, int right, long long change) {
        update(1, left, right, change);
    }

    long long minquery(int node, int left, int right) {
        value(node);
        if(right>=high(node) && left<=low(node)) {
            return minseg[id][node];
        } else if(right<low(node) || left>high(node)) {
            return inf;
        } else {
            return min(minquery(node*2, left, right), minquery(node*2+1, left, right));
        }
    }

    long long maxquery(int node, int left, int right) {
        value(node);
        if(right>=high(node) && left<=low(node)) {
            return maxseg[id][node];
        } else if(right<low(node) || left>high(node)) {
            return -inf;
        } else {
            return max(maxquery(node*2, left, right), maxquery(node*2+1, left, right));
        }
    }

    long long range(int left, int right) {
        return maxquery(1, left, right) - minquery(1, left, right); // gives the difference between max and min
    }

    void reset_negative(int node, segtree *other) {
        value(node);
        if(maxseg[id][node]<0) {
            other -> update(low(node), high(node), maxseg[id][node]);
            update(low(node), high(node), -maxseg[id][node]); // increment by corresponding amount
            assert(maxseg[id][node]==0);
        }
        if(minseg[id][node] >= 0) {
            return;
        }
        reset_negative(2*node, other);
        reset_negative(2*node+1, other);
    }
    void reset_overflow(int node, int cap) {
        value(node);
        assert(lazyadd[id][node]==0);
        if(minseg[id][node] > cap) {
            update(low(node), high(node), cap - minseg[id][node]); // decrease value
            assert(minseg[id][node]==cap);
        }
        if(maxseg[id][node] <= cap) {
            return;
        }
        reset_overflow(2*node, cap);
        reset_overflow(2*node+1, cap);
    }
};

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    int q = L.size();

    segtree s0(n, 0); // s0 stores normal
    segtree s1(n, 1); // s1 stores "inverted"
    // fill all boxes
    for(int i=0; i<n; i++) {
        s1.update(i, i, C[i]); // in s1, all boxes are initially full
    }

    for(int i = 0; i < q; ++i) {
        s0.update(L[i], R[i], V[i]); // segtree stores values as if the boxes have infinite capacity
        s1.update(L[i], R[i], -V[i]);
        if(i%100==0 || q-i<5000) {
            if(V[i]<0) {
                s0.reset_negative(1, &s1);
            } else {
                s1.reset_negative(1, &s0);
            }
        }
    }
    vector<int> ans(n);

    for(int i=0; i<n; i++) {
        ans[i] = min(s0.maxquery(1, i, i), (long long)C[i]);
    }
    return ans;
}
