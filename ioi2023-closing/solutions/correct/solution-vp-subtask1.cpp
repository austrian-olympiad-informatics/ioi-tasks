#include "closing.h"
#include <bits/stdc++.h>

using namespace std;

vector<vector<pair<int, int>>> g;
vector<bool> vis;

void dfs(vector<long long> &dist, int x, long long d)
{
    vis[x] = true;
    dist[x] = d;
    for (auto [y, w] : g[x])
    {
        if (!vis[y])
        {
            dfs(dist, y, d + w);
        }
    }
}

int max_score(int N, int X, int Y, long long K, std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    g.assign(N, vector<pair<int, int>>());
    for (int i = 0; i < N - 1; i++)
    {
        g[U[i]].emplace_back(V[i], W[i]);
        g[V[i]].emplace_back(U[i], W[i]);
    }

    vector<long long> dist(N), dist2(N);
    vis.assign(N, false);
    dfs(dist, X, 0);
    vis.assign(N, false);
    dfs(dist2, Y, 0);

    dist.insert(dist.end(), dist2.begin(), dist2.end());
    sort(dist.begin(), dist.end());

    int j = 0, ans = 0;
    long long cost = 0;
    while (cost + dist[j] <= K && j < (int)dist.size())
    {
        cost += dist[j];
        ++ans;
        ++j;
    }

    return ans;
}
