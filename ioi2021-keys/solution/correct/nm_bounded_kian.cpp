/**
 * Task: keys
 * Author: Kian Mirjalali
 * Solution: n*m with a bound/pruning
 */

#include "keys.h"
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;

#define tpc(C) template<class C>
#define allOf(c) ((c).begin()), ((c).end())
#define fori(i, a, b) for (int i = (a); i < int(b); i++)
#define forn(i, n) fori(i, 0, (n))

tpc(C) inline int sz(const C& c) { return c.size(); }

typedef vector<int> VI;
typedef vector<bool> VB;
typedef pair<int, int> PII;

/////////////////////////

int n, m, k;
VI vertex_key;
vector<vector<PII>> adj;

/////////////////////////

VI span;
VB mark;

inline void add2span(int x) {
    mark[x] = true;
    span.push_back(x);
}

inline int compute_span(int root, int bound) {
    VB key_available(k, false);
    span.clear();
    span.reserve(n);
    vector<VI> key_locked_adj(k, VI());
    mark.assign(n, false);
    add2span(root);
    for (int tail = 0; tail<sz(span) && sz(span)<=bound; tail++) {
        int x = span[tail];
        int x_key = vertex_key[x];
        if (!key_available[x_key]) {
            key_available[x_key] = true;
            for (int y : key_locked_adj[x_key])
                if (!mark[y])
                    add2span(y);
            key_locked_adj[x_key].clear();
        }
        for (auto p: adj[x])
            if (!mark[p.first]) {
                if (key_available[p.second])
                    add2span(p.first);
                else
                    key_locked_adj[p.second].push_back(p.first);
            }
    }
    return sz(span);
}

VI find_reachable(VI r, VI u, VI v, VI c) {
    n = sz(r);
    m = sz(c);
    k = max(*max_element(allOf(r)), *max_element(allOf(c)))+1;
    vertex_key = r;
    adj.assign(n, vector<PII>());
    forn(j, m) {
        adj[u[j]].emplace_back(v[j], c[j]);
        adj[v[j]].emplace_back(u[j], c[j]);
    }

    VI perm(n);
    forn(i, n)
        perm[i] = i;
    shuffle(allOf(perm), default_random_engine(10));

    VI span_size(n, 0);
    int min_span = n+1;
    forn(i, n) {
        int x = perm[i];
        //cerr << x << ": ";
        span_size[x] = compute_span(x, min_span);
        //cerr << span_size[x] << endl;
        min_span = min(min_span, span_size[x]);
    }

    VI ans(n);
    forn(x, n)
        ans[x] = (span_size[x] == min_span) ? 1 : 0;
    return ans;
}
