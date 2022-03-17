/**
 * Task: keys
 * Author: Kian Mirjalali
 * Solution: m*log(n)
 */

#include "keys.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <map>
#include <set>
using namespace std;

#define tpc(C) template<class C>
#define allOf(c) ((c).begin()), ((c).end())
#define fori(i, a, b) for (int i = (a); i < int(b); i++)
#define forn(i, n) fori(i, 0, (n))
#define dbg(x) #x << "=" << x
#define show(x) cerr << dbg(x) << endl

typedef vector<int> VI;
typedef vector<bool> VB;
typedef pair<int, int> PII;

tpc(C) inline int sz(const C& c) { return c.size(); }


/////////////////////////

struct DisjointSet {
    VI par;

    inline DisjointSet(int n): par(n) {
        forn(i, n)
            par[i] = i;
    }

    inline int findRoot(int x) {
        int i;
        for (i = x; par[i] != i; )
            i = par[i];
        int root = i;
        for (i = x; par[i] != i; ) {
            int j = par[i];
            par[i] = root;
            i = j;
        }
        return root;
    }

    /// returns true if the merge happened now or false if they are already merged
    inline bool merge(int x, int y) {
        int x_root = findRoot(x);
        int y_root = findRoot(y);
        if (x_root == y_root)
            return false;
        par[x_root] = y_root;
        return true;
    }

};


/////////////////////////

class IndexDSBase {
protected:
    int index_limit_;
    VI index_list;
    VI index_position;
    int list_head;

    inline void check_index_validity(int index) const {
        if (index < 0 || index_limit_ <= index) {
            cerr << "invalid index " << index << endl;
            exit(10);
        }
    }

    /// returns true if the index is added now or false if the index already existed
    inline bool add(int index) {
        //validity of index is checked in contains()
        if (contains(index))
            return false;
        index_position[index] = list_head;
        index_list[list_head++] = index;
        return true;
    }

public:
    inline IndexDSBase(int index_limit): index_limit_(index_limit), index_list(index_limit), index_position(index_limit), list_head(0) {
    }

    inline void clear() {
        list_head = 0;
    }

    inline bool contains(int index) const {
        check_index_validity(index);
        int pos = index_position[index];
        return 0 <= pos && pos < list_head && index_list[pos] == index;
    }
};

/// Performs operations in O(1) time
struct IndexSet: public IndexDSBase {
    using IndexDSBase::IndexDSBase;
    using IndexDSBase::add;
};


/// Performs operations in O(1) time
tpc(T) class IndexMap: public IndexDSBase {
    vector<T> index_value;
public:

    inline IndexMap(int index_limit): IndexDSBase(index_limit), index_value(index_limit) {
    }

    inline T& operator[](int index) {
        //validity of index is checked in add()
        if (add(index))
            index_value[index] = T();
        return index_value[index];
    }
};

/////////////////////////


int n, m, k;
VI vertex_key;
vector<vector<PII>> adj;
unique_ptr<DisjointSet> djs;

/////////////////////////

int span_head;
int valid_root;
VI span;
unique_ptr<IndexSet> mark;
unique_ptr<IndexSet> key_available;
unique_ptr<IndexMap<VI>> key_locked_adj;

inline void add2span(int x) {
    if (djs->findRoot(x) != valid_root)
        throw x;
    mark->add(x);
    span[span_head++] = x;
}

inline int compute_span(int root) {
    key_available->clear();
    span_head = 0;
    key_locked_adj->clear();
    mark->clear();
    valid_root = djs->findRoot(root);
    add2span(root);
    for (int tail = 0; tail<span_head; tail++) {
        //show(tail);
        int x = span[tail];
        int x_key = vertex_key[x];
        if (key_available->add(x_key)) {
            for (int y : (*key_locked_adj)[x_key])
                if (!mark->contains(y))
                    add2span(y);
            (*key_locked_adj)[x_key].clear();
        }
        for (auto p: adj[x])
            if (!mark->contains(p.first)) {
                if (key_available->contains(p.second))
                    add2span(p.first);
                else
                    (*key_locked_adj)[p.second].push_back(p.first);
            }
    }
    return span_head;
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
    djs = make_unique<DisjointSet>(n);
    mark = make_unique<IndexSet>(n);
    key_available = make_unique<IndexSet>(k);
    key_locked_adj = make_unique<IndexMap<VI>>(k);
    span.resize(n);

    map<int, VI> finalized_span;
    VI active_roots(n);
    forn(i, n)
        active_roots[i] = i;

    int min_span = n+1;
    while (!active_roots.empty()) {
        //show(active_roots.size());
        vector<PII> to_merge;
        VB is_active(n, false);
        for (int a : active_roots) {
            //show(a);
            try {
                int ss = compute_span(a);
                //show(ss);
                min_span = min(min_span, ss);
                finalized_span[a] = VI(span.begin(), span.begin()+span_head);
            } catch (int b) {
                //show(b);
                to_merge.emplace_back(a, b);
                is_active[a] = true;
            }
        }
        for (auto p : to_merge)
            if (djs->merge(p.first, p.second))
                is_active[p.first] = false;
        active_roots.clear();
        forn(i, n)
            if (is_active[i])
                active_roots.push_back(i);
    }

    key_locked_adj.reset(nullptr);
    key_available.reset(nullptr);
    mark.reset(nullptr);
    djs.reset(nullptr);

    VI ans(n, 0);
    for (auto p : finalized_span)
        if (sz(p.second) == min_span)
            for (int x : p.second)
                ans[x] = 1;
    return ans;
}
