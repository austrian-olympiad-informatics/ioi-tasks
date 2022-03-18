#include "stations.h"
#include <vector>

#define MAXN 2000

struct ISolution {
    virtual
    std::vector<int> label(int n, int k, std::vector<int> &u, std::vector<int> &v) = 0;

    virtual
    bool is_ancestor(int up, int down) = 0;

    int find_next_station(int s, int t, std::vector<int> &c) {
        if (s == t) return s;
        for (auto &node : c) {
            if (node == t) {
                return node;
            }
        }
        if (is_ancestor(s, t)) {
            // find c in between s-t
            for (auto &node : c) {
                if (is_ancestor(s, node) && is_ancestor(node, t)) {
                    return node;
                }
            }
            throw "Cannot find in-between node node";
        } else {
            // find ancestor of s
            for (auto &node : c) {
                if (is_ancestor(node, s)) {
                    return node;
                }
            }
            throw "Cannot find ancestor of s";
        }
    }
};

struct TwoNSoln : ISolution {
    virtual
    std::vector<int> label(int n, int k, std::vector<int> &u, std::vector<int> &v) {
        std::vector<int> labels(n);
        labels.resize(n);

        std::vector< std::vector< int > > edge_list;
        edge_list.resize(n);

        for (int i = 0; i < n - 1; ++i)
        {
            edge_list[u[i]].push_back(v[i]);
            edge_list[v[i]].push_back(u[i]);
        }
        cnt = 0;
        dfs(0, edge_list, labels, -1);

        return labels;
    }

    virtual
    bool is_ancestor(int up, int down) {
        if (up == 1000) return true;

        int up_small = up / MAXN;
        int up_big = up % MAXN;
        int down_small = down / MAXN;

        return (up_small < down_small) &&
            (down_small < up_big);
    }

private:
    void dfs(int u, std::vector<std::vector<int>> &edge_list, std::vector<int> &labels, int pre) {
//        printf("%d\n", u);
        labels[u] = MAXN * cnt;
        cnt += 1;

        for (auto &t : edge_list[u]) {
            if (t == pre) continue;
            dfs(t, edge_list, labels, u);
        }
        labels[u] += cnt;
    }

    static int cnt;
};

int TwoNSoln::cnt = 0;

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
    TwoNSoln soln;
    return soln.label(n, k, u, v);
}

int find_next_station(int s, int t, std::vector<int> c) {
    TwoNSoln soln;
    return soln.find_next_station(s, t, c);
}
