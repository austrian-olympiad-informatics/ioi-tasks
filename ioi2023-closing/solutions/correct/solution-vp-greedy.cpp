#include "closing.h"
#include <bits/stdc++.h>

using namespace std;

struct node
{
    long long t;
    int x, cnt = 0;
    bool operator<(const node &n) const
    {
        return t < n.t;
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
    priority_queue<node, vector<node>, greater<node>> pq_one, pq_two;

    for (int i = 0; i < N; i++)
    {
        if (max_cnt == 2 && distX[i] + distY[i] == min_sum)
        {
            res++;
            cnt[i]++;
            K -= min(distX[i], distY[i]);
            pq_one.push({abs(distX[i] - distY[i]), i, 1});
        }
        else
        {
            pq_one.push({min(distX[i], distY[i]), i, 0});
            if (max_cnt == 2)
                pq_two.push({max(distX[i], distY[i]), i, 0});
        }
    }

    auto get_top = [](priority_queue<node, vector<node>, greater<node>> &pq) -> node
    {
        return pq.empty() ? node{(long long)1e18, 0} : pq.top();
    };

    auto reduce = [&cnt, &max_cnt, &K](priority_queue<node, vector<node>, greater<node>> &pq) -> void
    {
        while (!pq.empty() && (cnt[pq.top().x] != pq.top().cnt || K < pq.top().t))
        {
            pq.pop();
        }
    };

    while (!pq_one.empty() || !pq_two.empty())
    {
        reduce(pq_one);
        reduce(pq_two);
        if (pq_one.empty() && pq_two.empty())
            break;
        node one1 = get_top(pq_one), one2;
        if (!pq_one.empty())
        {
            pq_one.pop();
            cnt[one1.x]++;
            reduce(pq_one);
            cnt[one1.x]--;
            one2 = get_top(pq_one);
        }
        node two = get_top(pq_two);
        if (pq_two.empty() || one1.t + one2.t < two.t)
        {
            K -= one1.t;
            res++;
            cnt[one1.x]++;
            if (max_cnt == 2 && cnt[one1.x] == 1)
            {
                pq_one.push({abs(distX[one1.x] - distY[one1.x]), one1.x, 1});
            }
        }
        else
        {
            pq_one.push(one1);
            K -= min(distX[two.x], distY[two.x]);
            res++;
            cnt[two.x]++;
            pq_one.push({abs(distX[two.x] - distY[two.x]), two.x, 1});
            pq_two.pop();
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

    return max(solve(N, 2, K, distX[Y]), solve(N, 1, K, distX[Y]));
}
