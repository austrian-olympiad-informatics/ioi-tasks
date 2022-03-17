#include <bits/stdc++.h>

// this solves the subtask when L=0 and R=N-1 for all queries
// actually, we dont need a segment tree
// we only need to store static prefix sums
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

    void update(int node, int left, int right, int change) {
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

    void update(int left, int right, int change) {
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
};

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    int q = L.size();

    segtree s(q);

    for(int i = 0; i < q; ++i) {
        s.update(1, i+1, q+1, V[i]); // segtree stores values as if the boxes have infinite capacity
    }
    vector<int> ans;
    ans.resize(n);
    for(int i=0; i<n; i++) {
        int lo = 0;
        int hi = q;
        // step 1: binary search for the point x in which the range is greater than c
        // at the end of this, lo would be the answer
        if(s.range(0, q) < C[i]) { // easy case: range is small
            assert(s.minquery(1,0,q)<=0);
            ans[i] = s.minquery(1, q, q) - s.minquery(1, 0, q);
            assert(ans[i]<C[i]);
            continue;
        }
        while(hi-lo>1) {
            int mid = (lo+hi)/2;
            if(s.range(mid, q) > C[i]) {
                lo = mid;
            } else {
                hi = mid;
            }
        }
        assert(s.range(q, q) < C[i]);
        assert(s.range(hi, q) <= C[i]);
        assert(s.range(lo, q) >= C[i]);

        long long tmp1 = s.minquery(1, lo, lo);
        long long tmp2 = s.minquery(1, q, q);
        assert(tmp1 != tmp2);
        if(tmp1 < tmp2) {
            // box was empty at time lo
            // figure out when the box was full
            long long tmp3 = s.maxquery(1, lo, q);
            assert(tmp3 - tmp1 >= C[i]);
            ans[i] = C[i] - (tmp3-tmp2);
            assert(ans[i]>=0);
        } else {
            // box was full at time lo
            // figure out when the box was empty
            long long tmp3 = s.minquery(1, lo, q);
            assert(tmp1 - tmp3 >= C[i]);
            ans[i] = tmp2 - tmp3;
            assert(ans[i]<=C[i]);
            assert(ans[i]>=0);
        }
    }
    return ans;
}
