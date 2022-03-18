// implementation of official soln

#include "stations.h"
#include <vector>

static int cnt = 0;

void dfs(int u, std::vector<std::vector<int>> &edge_list, std::vector<int> &labels, int pre, int depth) {
    if (depth % 2 == 0) {
        labels[u] = cnt;
    }
    cnt += 1;

    for (auto &t : edge_list[u]) {
        if (t == pre) continue;
        dfs(t, edge_list, labels, u, depth + 1);
    }
    if (depth % 2 == 1) {
        labels[u] = cnt;
    }
    cnt += 1;
//    printf("%d %d\n", u, labels[u]);
}

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
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
    dfs(0, edge_list, labels, -1, 0);

    return labels;
}

using Node = std::pair<int, int>;

int find_next_station(int s, int t, std::vector<int> c) {
    if (s == t) return s;
    if (s > c[0]) {
        for (int i = 1; i < c.size(); ++i) {
            int out;
            if (i + 1 < c.size()) {
                out = c[i + 1];
            } else {
                out = s;
            }

            if (c[i] <= t && t < out) {
                return c[i];
            }
        }
        return c[0];
    } else {
        int n = c.size() - 1;
//        printf("N: %d\n", n);
        for (int i = 0; i < n; ++i) {
            int in = 0;
            if (i == 0) {
                in = s;
            } else {
                in = c[i - 1];
            }
            if (in < t && t <= c[i]) {
                return c[i];
            }
        }
        return c[c.size() - 1];
    }
}
