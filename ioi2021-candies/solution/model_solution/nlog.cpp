#include <bits/stdc++.h>

// same as the nlog2 solution, except that we do binary search on the segment tree
using namespace std;

const int n_bits=19;
const long long inf = 1e18;
long long minseg[1<<(n_bits+1)];
long long maxseg[1<<(n_bits+1)];
long long lazyadd[1<<(n_bits+1)];

// a standard lazy propagation segment tree
// here we need to support both min and max
// so it is essentially 2 segtrees combined together
// but we only need 1 copy of lazy add
struct segtree {
    long long last_value = 0;
    long long small = inf;
    long long big = -inf;

    segtree() {}

    void update(int node, int change) { // treated as a suffix update
        last_value += change;
        node += (1<<n_bits);
        lazyadd[node] += change;
        while(node>1) {
            if(node%2==0) {
                lazyadd[node+1] += change;
            }
            minseg[node/2] = min(minseg[node]+lazyadd[node], minseg[node^1]+lazyadd[node^1]);
            maxseg[node/2] = max(maxseg[node]+lazyadd[node], maxseg[node^1]+lazyadd[node^1]);
            node = node/2;
        }
    }

    int solve(int capacity) { // returns the largest index i, such that the range >= c
        int node = 1;
        small = inf;
        big = -inf;
        long long lz = 0;
        while(node < (1<<n_bits)) {
            lz += lazyadd[node];
            node *= 2;
            if(max(big, maxseg[node+1]+lazyadd[node+1]+lz) - min(small, minseg[node+1]+lazyadd[node+1]+lz) > capacity) {
                node++;
            } else {
                big = max(big, maxseg[node+1]+lazyadd[node+1]+lz);
                small = min(small, minseg[node+1]+lazyadd[node+1]+lz);
            }
        }
        if(minseg[node] + lazyadd[node] + lz < last_value) {
            return capacity - (big - last_value);
        } else {
            return last_value - small;
        }
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
            s.update(p.first+2, p.second); // store values as if the boxes have infinite capacity
        }

        if(maxseg[1] - minseg[1] < C[i]) { // easy case: range is small
            ans[i] = s.last_value - (minseg[1] + lazyadd[1]);
        } else { // we binary search on the segtree
            ans[i] = s.solve(C[i]);
        }
    }
    return ans;
}
