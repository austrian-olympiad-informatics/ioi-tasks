#include <bits/stdc++.h>

using namespace std;

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    vector<int> A(n);
    int q = L.size();
    for(int i = 0; i < q; ++i) {
        for(int j = L[i]; j <= R[i]; ++j) {
            A[j] += V[i];
            A[j] = max(A[j], 0); A[j] = min(A[j], C[j]);
        }
    }
    return A;
}
