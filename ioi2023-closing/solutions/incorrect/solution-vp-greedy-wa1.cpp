#include "closing.h"
#include <bits/stdc++.h>

using namespace std;

struct node
{
    long long t;
    int x, cnt;
    bool operator<(const node &n) const
    {
        return t * n.cnt < n.t * cnt;
    }
    bool operator>(const node &n) const
    {
        return n < (*this);
    }
};

vector<vector<pair<int, long long>>> g;
vector<long long> distX, distY;
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

int solve(int N, int max_cnt, long long K, long long min_sum)
{
    int res = 0;
    vector<int> cnt(N, 0);
    priority_queue<node, vector<node>, greater<node>> pq;

    for (int i = 0; i < N; i++)
    {
        if (max_cnt == 2 && distX[i] + distY[i] == min_sum)
        {
            res++;
            cnt[i]++;
            K -= min(distX[i], distY[i]);
            pq.push({abs(distX[i] - distY[i]), i, 1});
        }
        else
        {
            pq.push({min(distX[i], distY[i]), i, 1});
            if (max_cnt == 2)
                pq.push({max(distX[i], distY[i]), i, 2});
        }
    }
    while (!pq.empty())
    {
        while (!pq.empty() && (cnt[pq.top().x] + pq.top().cnt > max_cnt || K < pq.top().t))
        {
            pq.pop();
        }
        if (pq.empty())
            break;
        node cur = pq.top();
        pq.pop();
        K -= cur.t;
        res += cur.cnt;
        cnt[cur.x] += cur.cnt;
        if (max_cnt == 2 && cnt[cur.x] == 1)
        {
            pq.push({abs(distX[cur.x] - distY[cur.x]), cur.x, 1});
        }
    }
    return K < 0 ? 0 : res;
}

int max_score(int N, int X, int Y, long long K, std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    g.assign(N, std::vector<std::pair<int, long long>>());
    for (int i = 0; i < N - 1; i++)
    {
        g[U[i]].emplace_back(V[i], W[i]);
        g[V[i]].emplace_back(U[i], W[i]);
    }
    distX.resize(N);
    distY.resize(N);
    vis.assign(N, false);
    dfs(distX, X, 0);
    vis.assign(N, false);
    dfs(distY, Y, 0);

    // cout << "distX: ";
    // for(int i = 0; i < N; i++) cout << distX[i] << ' ';
    // cout << endl;
    // cout << "distY: ";
    // for(int i = 0; i < N; i++) cout << distY[i] << ' ';
    // cout << endl;

    // cout << solve(N, 2, K) << endl;
    // cout << solve(N, 1, K) << endl;

    return max(solve(N, 2, K, distX[Y]), solve(N, 1, K, distX[Y]));
}
