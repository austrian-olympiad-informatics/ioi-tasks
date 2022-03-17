#include <bits/stdc++.h>

using namespace std;
#define low(i) (i<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)
#define high(i) ((i+1)<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)-1

const int n_bits=20;
const long long inf = 1e18;
long long minseg[1<<(n_bits+1)];
long long maxseg[1<<(n_bits+1)];
long long lazyadd[1<<(n_bits+1)];

// a standard lazy propagation segment tree
// here we need to support both min and max
// so it is essentially 2 segtrees combined together
// but we only need 1 copy of lazy add
struct segtree {
    segtree() {}
    void value(int node) {
        minseg[node] += lazyadd[node];
        maxseg[node] += lazyadd[node];
        if(node<(1<<n_bits)) {
            lazyadd[2*node] += lazyadd[node];
            lazyadd[2*node+1] += lazyadd[node];
        }
        lazyadd[node] = 0;
    }

    void update(int node, int left, int change) { // treated as a suffix update
        if(left<=low(node)) {
            lazyadd[node] += change;
        } else if(left>high(node)) {
            return;
        } else {
            update(2*node, left, change);
            update(2*node+1, left, change);
            value(node*2);
            value(node*2+1);
            minseg[node] = min(minseg[node*2], minseg[node*2+1]);
            maxseg[node] = max(maxseg[node*2], maxseg[node*2+1]);
        }
    }

    void update(int left, int change) {
        update(1, left, change);
    }


    long long minquery(int node, int left) {
        value(node);
        if(left<=low(node)) {
            return minseg[node];
        } else if(left>high(node)) {
            return inf;
        } else {
            return min(minquery(node*2, left), minquery(node*2+1, left));
        }
    }

    long long maxquery(int node, int left) {
        value(node);
        if(left<=low(node)) {
            return maxseg[node];
        } else if(left>high(node)) {
            return -inf;
        } else {
            return max(maxquery(node*2, left), maxquery(node*2+1, left));
        }
    }

    long long range(int left) {
        return maxquery(1, left) - minquery(1, left); // gives the difference between max and min
    }

    long long pointquery(int x) {
        int node = x + (1<<n_bits);
        long long ans = minseg[node] + lazyadd[node];
        while(node>1) {
            node = node/2;
            ans += lazyadd[node];
        }
        return ans;
    }
};

vector<pair<int,int>> toggle[(int)6e5];
// this tells you what you need to toggle on/off as you move across the boxes
// stores a pair indicating the query id and the change in number of candies
vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    int q = L.size();
    segtree s;

    for(int i=0; i<q; i++) {
        toggle[L[i]].push_back(make_pair(i, V[i]));
        toggle[R[i]+1].push_back(make_pair(i, -V[i]));
    }


    vector<int> ans;
    ans.resize(n);
    for(int i=0; i<n; i++) {
        for(pair<int,int> p: toggle[i]) {
            s.update(p.first+2, p.second); // segtree stores values as if the boxes have infinite capacity
        }
        int lo = 0;
        int hi = q+1;

        // step 1: binary search for the point x in which the range is greater than c
        // at the end of this, lo would be the answer
        if(s.range(0) < C[i]) { // easy case: range is small
            ans[i] = s.pointquery(q+1) - s.minquery(1, 0);
            assert(ans[i]<C[i]);
            continue;
        }
        while(hi-lo>1) {
            int mid = (lo+hi)/2;
            if(s.range(mid) > C[i]) {
                lo = mid;
            } else {
                hi = mid;
            }
        }
        assert(s.range(q+1) < C[i]);
        assert(s.range(hi) <= C[i]);
        assert(s.range(lo) >= C[i]);

        long long tmp1 = s.pointquery(lo);
        long long tmp2 = s.pointquery(q+1);
        assert(tmp1 != tmp2);
        if(tmp1 < tmp2) {
            // box was empty at time lo
            // figure out when the box was full
            long long tmp3 = s.maxquery(1, lo);
            assert(tmp3 - tmp1 >= C[i]);
            ans[i] = C[i] - (tmp3-tmp2);
            assert(ans[i]>=0);
        } else {
            // box was full at time lo
            // figure out when the box was empty
            long long tmp3 = s.minquery(1, lo);
            assert(tmp1 - tmp3 >= C[i]);
            ans[i] = tmp2 - tmp3;
            assert(ans[i]<=C[i]);
            assert(ans[i]>=0);
        }
    }
    return ans;
}
