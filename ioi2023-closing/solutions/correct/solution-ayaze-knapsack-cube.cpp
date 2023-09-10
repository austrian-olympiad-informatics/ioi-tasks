// O(N^3) with high constant factor
// probably can AC N <= 500 if optimized e.g using plain array
#include "closing.h"
#include <bits/stdc++.h>
using namespace std;

const int kStateX = 1, kStateYActive = 2, kStateYInActive = 4;
const long long kInf = 4e18;

vector<vector<pair<int, int>>> adj_list;
vector<long long> dist_from_x, dist_from_y;
vector<bool> in_path;
vector<vector<vector<long long>>> dp_mat;
int N, Y;

void DfsDistance(int now, int prv, long long current_dist, vector<long long> &dist_vec, vector<int> &parent)
{
    dist_vec[now] = current_dist;
    parent[now] = prv;

    for (pair<int, int> edge : adj_list[now])
    {
        int nxt = edge.first;
        if (nxt == prv)
            continue;
        long long new_dist = edge.second + current_dist;
        DfsDistance(nxt, now, new_dist, dist_vec, parent);
    }
}

void DfsDP(int now, int prv)
{
    for (int score = 0; score <= 2 * N; score++)
    {
        for (int mask = 0; mask < (1 << 3); mask++)
        {
            dp_mat[now][score][mask] = kInf;
        }
    }

    for (int mask = 0; mask < (1 << 3); mask++)
    {
        if ((mask & kStateYActive) > 0 && (mask & kStateYInActive) > 0)
            continue;

        int score = 0;
        long long val = 0;

        if (mask & kStateX)
            score++, val = max(val, dist_from_x[now]);
        if (mask & kStateYActive)
            score++, val = max(val, dist_from_y[now]);
        dp_mat[now][score][mask] = val;
    }

    for (pair<int, int> edge : adj_list[now])
    {
        int nxt = edge.first;
        if (nxt == prv)
            continue;
        DfsDP(nxt, now);

        vector<vector<long long>> copy_current_dp = dp_mat[now];
        for (int score = 0; score <= 2 * N; score++)
        {
            for (int mask = 0; mask < (1 << 3); mask++)
            {
                dp_mat[now][score][mask] = kInf;
            }
        }

        for (int score = 0; score <= 2 * N; score++)
        {
            for (int mask = 0; mask < (1 << 3); mask++)
            {
                if ((mask & kStateYActive) > 0 && (mask & kStateYInActive) > 0)
                    continue;

                int active_x = mask & kStateX;
                int upper_mask = mask - active_x;
                for (int child_score = 0; child_score + score <= 2 * N; child_score++)
                {
                    for (int state_x = 0; state_x <= active_x; state_x++)
                    { // we can keep X, or turn it off
                        // Y state do not change
                        dp_mat[now][score + child_score][mask] = min(
                            copy_current_dp[score][mask] + dp_mat[nxt][child_score][state_x | upper_mask],
                            dp_mat[now][score + child_score][mask]);
                        // currently reachable by Y, make child unreachable by Y
                        if (mask & kStateYActive)
                        {
                            dp_mat[now][score + child_score][mask] = min(
                                copy_current_dp[score][mask] + dp_mat[nxt][child_score][state_x],
                                dp_mat[now][score + child_score][mask]);
                        }
                        // this and above not reachable by Y,
                        // from here downward make it possible to be reachable by Y
                        if ((mask & kStateYInActive) > 0 && in_path[nxt])
                        {
                            dp_mat[now][score + child_score][mask] = min(
                                copy_current_dp[score][mask] + dp_mat[nxt][child_score][state_x | kStateYActive],
                                dp_mat[now][score + child_score][mask]);
                        }
                    }
                }
            }
        }
    }

    // if it is Y, then it must be reachable by Y
    if (now == Y)
    {
        for (int score = 0; score <= 2 * N; score++)
        {
            for (int mask = 0; mask < (1 << 3); mask++)
            {
                if ((mask & kStateYActive) == 0)
                {
                    dp_mat[now][score][mask] = kInf;
                }
            }
        }
    }
}

int max_score(int _N, int X, int _Y, long long K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    N = _N;
    Y = _Y;

    adj_list.assign(N, vector<pair<int, int>>());
    dist_from_x.resize(N);
    dist_from_y.resize(N);
    in_path.assign(N, false);
    dp_mat.assign(N, vector<vector<long long>>(2 * N + 1, vector<long long>(1 << 3, 0)));

    for (int i = 0; i < static_cast<int>(U.size()); i++)
    {
        adj_list[U[i]].push_back({V[i], W[i]});
        adj_list[V[i]].push_back({U[i], W[i]});
    }

    vector<int> parent_rooted_x(N);
    DfsDistance(Y, -1, 0, dist_from_y, parent_rooted_x);
    DfsDistance(X, -1, 0, dist_from_x, parent_rooted_x);
    for (int i = Y; i != X; i = parent_rooted_x[i])
    {
        in_path[i] = true;
    }

    DfsDP(X, -1);
    int ret = 0;
    for (int score = 0; score <= 2 * N; score++)
    {
        for (int mask = 0; mask < (1 << 3); mask++)
        {
            if (dp_mat[X][score][mask] <= K)
            {
                ret = max(ret, score);
            }
        }
    }

    return ret;
}
