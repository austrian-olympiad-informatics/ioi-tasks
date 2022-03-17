#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1000000 + 10;
const int MAXQ = 1000000 + 10;
const long long INF = (long long)(1e18);

struct SegmentTree {
    int n;
    long long vmin[(MAXN + MAXQ) * 4], vmax[(MAXN + MAXQ) * 4], lazy[(MAXN + MAXQ) * 4];

    void init(int _n) {
        n = _n;
        for(int i = 0; i <= 4 * n; ++i) vmin[i] = vmax[i] = lazy[i] = 0;
    }

    void lazy_update(int node, int from, int to) {
        vmin[node] += lazy[node]; vmax[node] += lazy[node];
        if (from < to) {
            lazy[node * 2] += lazy[node];
            lazy[node * 2 + 1] += lazy[node];
        }
        lazy[node] = 0;
    }

    void update(int node, int from, int to, int L, int R, int add) {
        lazy_update(node, from, to);
        if (from > R || to < L) return;
        if (L <= from && to <= R) {
            lazy[node] += add;
            lazy_update(node, from, to);
            return;
        }
        int mid = (from + to) / 2;
        update(node * 2, from, mid, L, R, add);
        update(node * 2 + 1, mid + 1, to, L, R, add);
        vmin[node] = min(vmin[node * 2], vmin[node * 2 + 1]);
        vmax[node] = max(vmax[node * 2], vmax[node * 2 + 1]);
    }

    long long get(int node, int from, int to, int lowerbound) {
        lazy_update(node, from, to);
        if (from == to) return vmin[node];
        int mid = (from + to) / 2;
        if (vmax[node * 2 + 1] + lazy[node * 2 + 1] >= lowerbound)
            return get(node * 2 + 1, mid + 1, to, lowerbound);
        return min( vmin[node * 2 + 1] + lazy[node * 2 + 1], get(node * 2, from, mid, lowerbound) );
    }

    void add_range(int L, int R, int add) {
        update(1, 0, n - 1, L, R, add);
    }

    long long min_suffix(int lowerbound) {
        if (vmax[1] < lowerbound) return -INF;
        return min(0LL, get(1, 0, n - 1, lowerbound));
    }
} T;

vector<int> distribute_candies(vector<int> C, vector<int> L, vector<int> R, vector<int> V) {
    int n = C.size();
    int q = L.size();
    vector< vector<int> > begin_at(n, vector<int>()), end_at(n, vector<int>());
    for(int i = 0; i < q; ++i) {
        begin_at[L[i]].push_back(i);
        end_at[R[i]].push_back(i);
    }

    T.init(n + q);
    vector<int> final_A(n);
    for(int i = 0; i < n; ++i) {
        if (i > 0) {
            for(int j : end_at[i - 1]) T.add_range(0, n + j, -V[j]);
        }
        for(int j : begin_at[i]) T.add_range(0, n + j, V[j]);

        int low = 1, high = C[i] + 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            long long smin = T.min_suffix(mid);
            if (-smin + mid > C[i]) high = mid - 1;
            else low = mid + 1;
        }
        final_A[i] = high;
    }

    return final_A;
}
