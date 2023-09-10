#include "closing.h"
#include <bits/stdc++.h>

using namespace std;

struct node
{
    long long t;
    int x, cnt;
    long double w;
    bool operator<(const node &n) const
    {
        return t * n.w < n.t * w;
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

bool optimize1(vector<int> &cnt, int &ans, long long &K)
{
    int min_idx = -1, max_idx = -1;
    for (int i = 0; i < (int)cnt.size(); i++)
    {
        if (cnt[i] == 0)
        {
            if (min_idx == -1 || max(distX[min_idx], distY[min_idx]) > max(distX[i], distY[i]))
            {
                min_idx = i;
            }
        }
        else if (cnt[i] == 2)
        {
            if (max_idx == -1 || abs(distX[max_idx] - distY[max_idx]) < abs(distX[i] - distY[i]))
            {
                max_idx = i;
            }
        }
    }
    long long diff = max(distX[min_idx], distY[min_idx]) - abs(distX[max_idx] - distY[max_idx]);
    if (min_idx != -1 && max_idx != -1 && diff <= K)
    {
        ans++;
        cnt[min_idx] = 2;
        cnt[max_idx]--;
        K -= diff;
        return true;
    }
    return false;
}

int solve(int N, int max_cnt, long long K, long long min_sum, long double w2)
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
            pq.push({abs(distX[i] - distY[i]), i, 1, 1});
        }
        else
        {
            pq.push({min(distX[i], distY[i]), i, 1, 1});
            if (max_cnt == 2)
                pq.push({max(distX[i], distY[i]), i, 2, w2});
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
            pq.push({abs(distX[cur.x] - distY[cur.x]), cur.x, 1, 1});
        }
    }
    if (max_cnt == 2 && K >= 0)
    {
        while (optimize1(cnt, res, K))
            ;
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

    int res = solve(N, 1, K, distX[Y], 2);

    long double l = 0.2, r = 3;

    res = max({res, solve(N, 2, K, distX[Y], l), solve(N, 2, K, distX[Y], r)});
    for (int tt = 0; tt < 12; tt++)
    {
        long double m1 = (2 * l + r) / 3;
        long double m2 = (l + 2 * r) / 3;
        int M1 = solve(N, 2, K, distX[Y], m1);
        int M2 = solve(N, 2, K, distX[Y], m2);
        res = max({res, M1, M2});
        if (M1 < M2)
        {
            r = m2;
        }
        else
        {
            l = m1;
        }
    }
    // cout << "expected: " << (l + r) / 2 << endl;
    return res;
}
