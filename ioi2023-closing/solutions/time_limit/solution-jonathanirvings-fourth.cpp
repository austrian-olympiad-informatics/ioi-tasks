#include "closing.h"

#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int>>> adj;

void fill_distance(int u, int from, vector<long long> &distance)
{
    for (auto [v, w] : adj[u])
    {
        if (v != from)
        {
            distance[v] = distance[u] + w;
            fill_distance(v, u, distance);
        }
    }
}

void add_to_list(int u, int parent, vector<int> &list)
{
    list.push_back(u);
    for (auto [v, _] : adj[u])
    {
        if (v != parent)
        {
            add_to_list(v, u, list);
        }
    }
}

int solve_knapsack(long long rem, const vector<vector<long long>> &weights)
{
    int tot = 0;
    for (int i = 0; i < static_cast<int>(weights.size()); ++i)
    {
        tot += weights[i].size();
    }
    vector<long long> dp(tot + 1, LLONG_MAX >> 1);
    dp[0] = 0;
    for (const vector<long long> &w : weights)
    {
        for (int i = tot; i >= 0; --i)
        {
            for (int j = 0; j < static_cast<int>(w.size()); ++j)
            {
                if (i >= j + 1)
                {
                    dp[i] = min(dp[i], dp[i - j - 1] + w[j]);
                }
            }
        }
    }
    for (int i = tot; i >= 1; --i)
    {
        if (dp[i] <= rem)
        {
            return i;
        }
    }
    return 0;
}

int max_score(int N, int X, int Y, long long K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    adj.assign(N, vector<pair<int, int>>());
    for (int i = 0; i < N - 1; ++i)
    {
        adj[U[i]].emplace_back(V[i], W[i]);
        adj[V[i]].emplace_back(U[i], W[i]);
    }
    vector<long long> fromX(N), fromY(N);
    fill_distance(X, -1, fromX);
    fill_distance(Y, -1, fromY);
    vector<bool> is_in_path(N);
    vector<int> in_path;
    for (int i = 0; i < N; ++i)
    {
        is_in_path[i] = fromX[i] + fromY[i] == fromX[Y];
        if (is_in_path[i])
        {
            in_path.push_back(i);
        }
    }
    sort(in_path.begin(), in_path.end(), [&](int a, int b)
         { return fromX[a] < fromX[b]; });
    vector<vector<int>> subtree(N);
    for (int u : in_path)
    {
        for (auto [v, _] : adj[u])
        {
            if (!is_in_path[v])
            {
                add_to_list(v, u, subtree[u]);
            }
        }
    }

    int answer = 0;
    {
        vector<vector<long long>> weights;
        for (int i = 0; i < N; ++i)
        {
            weights.push_back({min(fromX[i], fromY[i])});
        }
        answer = solve_knapsack(K, weights);
    }
    for (int i = 0; i < static_cast<int>(in_path.size()); ++i)
    {
        for (int j = i; j < static_cast<int>(in_path.size()); ++j)
        {
            int cur = 0;
            long long rem = K;
            vector<vector<long long>> weights;
            for (int k = 0; k < static_cast<int>(in_path.size()); ++k)
            {
                if (k < i)
                {
                    rem -= fromX[in_path[k]];
                    cur += 1;
                    for (int u : subtree[in_path[k]])
                    {
                        weights.push_back({fromX[u]});
                    }
                }
                else if (k > j)
                {
                    rem -= fromY[in_path[k]];
                    cur += 1;
                    for (int u : subtree[in_path[k]])
                    {
                        weights.push_back({fromY[u]});
                    }
                }
                else
                {
                    rem -= max(fromX[in_path[k]], fromY[in_path[k]]);
                    cur += 2;
                    for (int u : subtree[in_path[k]])
                    {
                        weights.push_back({min(fromX[u], fromY[u]),
                                           max(fromX[u], fromY[u])});
                    }
                }
            }
            if (rem >= 0)
            {
                cur += solve_knapsack(rem, weights);
                answer = max(answer, cur);
            }
        }
    }
    return answer;
}
