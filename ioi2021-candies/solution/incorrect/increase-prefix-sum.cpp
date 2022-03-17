#include <bits/stdc++.h>
using namespace std;

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    vector<long long> prefix_sum(n+1);
    int q = L.size();
    for(int i=0; i<q; i++) {
        prefix_sum[L[i]] += V[i];
        prefix_sum[R[i]+1] -= V[i];
    }
    vector<int> ans(n);
    long long cumulative = 0;
    for(int i=0; i<n; i++) {
        cumulative += prefix_sum[i];
        ans[i] = min((long long)C[i], cumulative);
    }
    return ans;
}
