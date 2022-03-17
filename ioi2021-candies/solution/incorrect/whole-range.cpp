#include <bits/stdc++.h>

// this solves the subtask when L=0 and R=N-1 for all queries
// actually, we dont need a segment tree
// we only need to store static prefix sums
using namespace std;

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    int q = L.size();

    vector<long long> prefix(q+1);
    prefix[0] = 0;
    vector<long long> suffix_max(q+1);
    vector<long long> suffix_min(q+1);

    for(int i = 0; i < q; ++i) {
        prefix[i+1] = prefix[i] + V[i];
    }
    suffix_max[q] = prefix[q];
    suffix_min[q] = prefix[q];

    for(int i=q-1; i>=0; i--) {
        suffix_max[i] = max(prefix[i], suffix_max[i+1]);
        suffix_min[i] = min(prefix[i], suffix_min[i+1]);
    }

    vector<int> ans;
    ans.resize(n);
    for(int i=0; i<n; i++) {
        // calculate naively
        int tmp = 0;
        for(int j=0; j<q; j++) {
            if(i>=L[j] and i<R[j]) {
                tmp += V[j];
            }
            if(tmp > C[i]) tmp = C[i];
            if(tmp < 0) tmp = 0;
        }
        int lo = 0;
        int hi = q+1;
        // step 1: binary search for the point x in which the range is greater than c
        // at the end of this, lo would be the answer
        if(suffix_max[0] - suffix_min[0] < C[i]) { // easy case: range is small
            ans[i] = prefix[q] - suffix_min[0];
            assert(ans[i]<C[i]);
            continue;
        }
        while(hi-lo>1) {
            int mid = (lo+hi)/2;
            if(suffix_max[mid] - suffix_min[mid] > C[i]) {
                lo = mid;
            } else {
                hi = mid;
            }
        }

        long long tmp1 = prefix[lo];
        long long tmp2 = prefix[q];
        assert(tmp1 != tmp2);
        if(tmp1 < tmp2) {
            // box was empty at time lo
            // figure out when the box was full
            long long tmp3 = suffix_max[lo];
            assert(tmp3 - tmp1 >= C[i]);
            ans[i] = C[i] - (tmp3-tmp2);
            assert(ans[i]>=0);
        } else {
            // box was full at time lo
            // figure out when the box was empty
            long long tmp3 = suffix_min[lo];
            assert(tmp1 - tmp3 >= C[i]);
            ans[i] = tmp2 - tmp3;
            assert(ans[i]<=C[i]);
            assert(ans[i]>=0);
        }
    }
    return ans;
}
