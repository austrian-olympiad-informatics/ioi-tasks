// O(N^2)
// WA: knapsack, ignoring reachability of preceeding nodes
#include "closing.h"
#include <bits/stdc++.h>
using namespace std;

const long long kInf = 4e18;

vector<vector<pair<int, int>>> adj_list;
vector<long long> p_val, q_val;

void DfsDistance(int now, int prv, long long current_dist, vector<long long> &dist_vec)
{
    dist_vec[now] = current_dist;

    for (pair<int, int> edge : adj_list[now])
    {
        int nxt = edge.first;
        if (nxt == prv)
            continue;
        long long new_dist = edge.second + current_dist;
        DfsDistance(nxt, now, new_dist, dist_vec);
    }
}

int Knapsack(int N, long long K)
{
    vector<vector<long long>> dp(N + 1, vector<long long>(2 * N + 1, kInf));
    dp[0][0] = 0;

    for (int i = 0; i < N; i++)
    {
        int cap = 2 * i;
        for (int j = 0; j <= cap; j++)
        {
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j]);
            dp[i + 1][j + 1] = min(dp[i + 1][j + 1], dp[i][j] + p_val[i]);
            dp[i + 1][j + 2] = min(dp[i + 1][j + 2], dp[i][j] + q_val[i]);
        }
    }

    for (int j = 2 * N; j >= 0; j--)
    {
        if (dp[N][j] <= K)
        {
            return j;
        }
    }

    return 0;
}

int max_score(int N, int X, int Y, long long K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    adj_list.assign(N, vector<pair<int, int>>());
    q_val.resize(N);
    p_val.resize(N);

    for (int i = 0; i < static_cast<int>(U.size()); i++)
    {
        adj_list[U[i]].push_back({V[i], W[i]});
        adj_list[V[i]].push_back({U[i], W[i]});
    }

    vector<long long> dist_from_x(N);
    vector<long long> dist_from_y(N);
    DfsDistance(Y, -1, 0, dist_from_y);
    DfsDistance(X, -1, 0, dist_from_x);

    for (int i = 0; i < N; i++)
    {
        p_val[i] = min(dist_from_x[i], dist_from_y[i]);
        q_val[i] = max(dist_from_x[i], dist_from_y[i]);
    }

    int ret = Knapsack(N, K);
    return ret;
}
