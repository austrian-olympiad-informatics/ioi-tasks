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

    void godown(int node, int from, int to, int C, long long &smin, long long &smax, int &pos) {
        lazy_update(node, from, to);
        if (from == to) {
            smin = min(smin, vmin[node]); smax = max(smax, vmax[node]); pos = from;
            return;
        }
        int mid = (from + to) / 2;
        if (max(smax, vmax[node * 2 + 1] + lazy[node * 2 + 1]) - min(smin, vmin[node * 2 + 1] + lazy[node * 2 + 1]) > C)
            godown(node * 2 + 1, mid + 1, to, C, smin, smax, pos);
        else {
            smin = min(smin, vmin[node * 2 + 1] + lazy[node * 2 + 1]);
            smax = max(smax, vmax[node * 2 + 1] + lazy[node * 2 + 1]);
            godown(node * 2, from, mid, C, smin, smax, pos);
        }
    }

    long long max_suffix(int node, int from, int to, int L) {
        lazy_update(node, from, to);
        if (from >= L) return vmax[node];
        if (to < L) return -INF;
        int mid = (from + to) / 2;
        return max( max_suffix(node * 2, from, mid, L), max_suffix(node * 2 + 1, mid + 1, to, L) );
    }

    long long max_upperbound(int node, int from, int to, long long ub) {
        lazy_update(node, from, to);
        if (from == to) return vmax[node];
        int mid = (from + to) / 2;
        if (vmax[node * 2 + 1] + lazy[node * 2 + 1] > ub) return max_upperbound(node * 2 + 1, mid + 1, to, ub);
        else return max_upperbound(node * 2, from, mid, ub);
    }

    void add_range(int L, int R, int add) {
        update(1, 0, n - 1, L, R, add);
    }

    int get(int C) {
        long long smin = 0, smax = 0; int pos = n;
        godown(1, 0, n - 1, C, smin, smax, pos);
        if (smax - smin <= C) return smax;
        long long x = max(0LL, max_suffix(1, 0, n - 1, pos + 1));
        if (x < smax) return max(x, C + smin);
        return smax;
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

    T.init(1 + q);
    vector<int> final_A(n);
    for(int i = 0; i < n; ++i) {
        if (i > 0) {
            for(int j : end_at[i - 1]) T.add_range(0, 1 + j, -V[j]);
        }
        for(int j : begin_at[i]) T.add_range(0, 1 + j, V[j]);


        final_A[i] = T.get(C[i]);
    }

    return final_A;
}

