#include "closing.h"

#include <bits/stdc++.h>

using namespace std;

vector<vector<pair<int, int>>> neigh;
vector<long long> dist1, dist2;
int n, x, y;
long long k;
vector<int> u, v, w;
vector<bool> volt, path;

bool dfs_impl(int p, vector<long long> &dist, long long d)
{
    if (volt[p])
        return false;
    volt[p] = true;
    bool found = (p == y);
    dist[p] = d;
    for (int i = 0; i < neigh[p].size(); ++i)
    {
        pair<int, int> n = neigh[p][i];
        bool now = dfs_impl(n.first, dist, d + n.second);
        if (now)
            found = true;
    }
    if (found)
        path[p] = true;
    return found;
}

void dfs(int p, vector<long long> &dist)
{
    vector<bool>(n, false).swap(volt);
    vector<long long>(n, 0).swap(dist);
    dfs_impl(p, dist, 0);
}

priority_queue<long long> q, q2;

int solve1()
{
    int count = 0;
    long long sum = 0;
    for (int i = 0; i < n; i++)
    {
        q.push(-dist1[i]);
    }
    while (!q.empty() && sum - q.top() <= k)
    {
        count++;
        sum -= q.top();
        q.pop();
    }
    return count;
}

priority_queue<pair<long long, int>> q3, q4;
vector<bool> used;

int solve2()
{
    vector<bool>(n, false).swap(used);
    int count = 0;
    long long sum = 0;
    for (int i = 0; i < n; i++)
    {
        if (path[i])
        {
            count++;
            sum += dist1[i];
            q2.push(-dist2[i]);
        }
        else
        {
            if (dist1[i] <= dist2[i])
            {
                q2.push(-dist1[i]);
                q2.push(-dist2[i]);
            }
            else
            {
                q3.push({-dist1[i] - dist2[i], i});
                q4.push({-dist1[i], i});
            }
        }
    }
    if (sum > k)
        return 0;
    while (!q3.empty() && sum - q3.top().first <= k)
    {
        if (q2.size() <= 1)
        {
            count += 2;
            sum -= q3.top().first;
            used[q3.top().second] = true;
            q3.pop();
            continue;
        }
        long long q2t = q2.top();
        q2.pop();
        if (-q2.top() - q2t < -q3.top().first)
        {
            count++;
            sum -= q2t;
        }
        else
        {
            q2.push(q2t);
            count += 2;
            sum -= q3.top().first;
            used[q3.top().second] = true;
            q3.pop();
            continue;
        }
    }
    while (!q2.empty() && sum - q2.top() <= k)
    {
        count++;
        sum -= q2.top();
        q2.pop();
    }
    while (!q4.empty() && used[q4.top().second])
    {
        q4.pop();
    }
    if (!q4.empty() && -q4.top().first + sum <= k)
    {
        count++;
        sum -= q4.top().first;
        q4.pop();
    }
    return count;
}

int max_score(int N, int X, int Y, long long K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    while (!q.empty())
        q.pop();
    while (!q2.empty())
        q2.pop();
    while (!q3.empty())
        q3.pop();
    while (!q4.empty())
        q4.pop();
    n = N;
    x = X;
    y = Y;
    k = K;
    u = U;
    v = V;
    w = W;
    vector<bool>(n, false).swap(path);
    vector<vector<pair<int, int>>>(n, vector<pair<int, int>>(0)).swap(neigh);
    for (int i = 0; i < u.size(); ++i)
    {
        neigh[u[i]].push_back({v[i], w[i]});
        neigh[v[i]].push_back({u[i], w[i]});
    }
    dfs(x, dist1);
    dfs(y, dist2);
    for (int i = 0; i < n; i++)
    {
        if (dist2[i] < dist1[i])
            swap(dist2[i], dist1[i]);
        dist2[i] -= dist1[i];
    }
    int a = solve1();
    int b = solve2();
    // cerr << a << " " << b << endl;
    return max(a, b);
}
