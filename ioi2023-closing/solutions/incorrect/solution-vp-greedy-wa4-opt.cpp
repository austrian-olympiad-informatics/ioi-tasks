#include "closing.h"
#include <bits/stdc++.h>

using namespace std;

struct node
{
    long long t;
    int x;
    bool extra = false;
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
            pq_one.push({abs(distX[i] - distY[i]), i});
        }
        else
        {
            pq_one.push({min(distX[i], distY[i]), i});
            if (max_cnt == 2)
                pq_two.push({max(distX[i], distY[i]), i});
        }
    }

    auto get_top = [](priority_queue<node, vector<node>, greater<node>> &pq) -> node
    {
        return pq.empty() ? node{(long long)1e18, 0} : pq.top();
    };

    while (!pq_one.empty() || !pq_two.empty())
    {
        while (!pq_one.empty() && (cnt[pq_one.top().x] + 1 > max_cnt || K < pq_one.top().t))
        {
            pq_one.pop();
        }
        while (!pq_two.empty() && (cnt[pq_two.top().x] + 2 > max_cnt || K < pq_two.top().t))
        {
            pq_two.pop();
        }
        if (pq_one.empty() && pq_two.empty())
            break;
        node one1 = get_top(pq_one);
        pq_one.pop();
        node one2 = get_top(pq_one);
        node two = get_top(pq_two);
        if (pq_two.empty() || one1.t + one2.t < two.t)
        {
            K -= one1.t;
            res++;
            cnt[one1.x]++;
            if (max_cnt == 2 && cnt[one1.x] == 1)
            {
                pq_one.push({abs(distX[one1.x] - distY[one1.x]), one1.x});
            }
        }
        else
        {
            pq_one.push(one1);
            K -= two.t;
            res += 2;
            cnt[two.x] += 2;
            pq_two.pop();
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

    return max(solve(N, 2, K, distX[Y]), solve(N, 1, K, distX[Y]));
}
