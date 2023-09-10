// O(N^3 log N)
#include "closing.h"
#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int>>> adj_list;
vector<long long> p_val, q_val;
vector<bool> in_path_xy;

int N;
long long K;

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

int CalculateWithCommon()
{
    vector<tuple<long long, long long, int>> sorted_diffs;
    for (int i = 0; i < N; i++)
    {
        sorted_diffs.push_back(make_tuple(q_val[i] - p_val[i], p_val[i], i));
    }
    sort(sorted_diffs.begin(), sorted_diffs.end());

    auto GetOptimumLeftover = [&](vector<long long> p_left, vector<long long> q_left, long long left) -> int
    {
        p_left.insert(p_left.begin(), 0);
        q_left.insert(q_left.begin(), 0);
        sort(p_left.begin(), p_left.end());
        sort(q_left.begin(), q_left.end());

        for (int i = 1; i < static_cast<int>(p_left.size()); i++)
            p_left[i] += p_left[i - 1];
        for (int i = 1; i < static_cast<int>(q_left.size()); i++)
            q_left[i] += q_left[i - 1];

        int ret = 0;
        for (int i = 0; i < static_cast<int>(p_left.size()); i++)
        {
            for (int j = 0; j < static_cast<int>(q_left.size()); j++)
            {
                if (p_left[i] + q_left[j] <= left)
                {
                    ret = max(ret, i + 2 * j);
                }
            }
        }

        return ret;
    };

    int ret = 0;

    for (int border = 0; border < N; border++)
    {
        long long total_in_path = 0;
        int score_in_path = 0;
        vector<long long> p_left, q_left;

        for (int i = 0; i < N; i++)
        {
            int node = get<2>(sorted_diffs[i]);
            if (in_path_xy[node])
            {
                if (i <= border)
                {
                    total_in_path += q_val[node];
                    score_in_path += 2;
                }
                else
                {
                    total_in_path += p_val[node];
                    score_in_path += 1;
                }
            }
            else
            {
                if (i <= border)
                {
                    q_left.push_back(q_val[node]);
                }
                else
                {
                    p_left.push_back(p_val[node]);
                }
            }
        }

        if (total_in_path <= K)
        {
            int opt_leftover = GetOptimumLeftover(p_left, q_left, K - total_in_path);
            ret = max(ret, score_in_path + opt_leftover);
        }
    }

    return ret;
}

int CalculateWithoutCommon()
{
    vector<long long> sorted_dist;
    for (int i = 0; i < N; i++)
    {
        sorted_dist.push_back(p_val[i]);
        sorted_dist.push_back(q_val[i]);
    }
    sort(sorted_dist.begin(), sorted_dist.end());

    int score = 0;
    long long total = 0;
    for (long long d : sorted_dist)
    {
        if (total + d <= K)
        {
            score++;
            total += d;
        }
    }

    return score;
}

int max_score(int _N, int X, int Y, long long _K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    N = _N;
    K = _K;

    adj_list.assign(N, vector<pair<int, int>>());
    q_val.resize(N);
    p_val.resize(N);
    in_path_xy.assign(N, false);

    for (int i = 0; i < static_cast<int>(U.size()); i++)
    {
        adj_list[U[i]].push_back({V[i], W[i]});
        adj_list[V[i]].push_back({U[i], W[i]});
    }

    vector<int> parent_rooted_x(N);
    vector<long long> dist_from_y(N);
    vector<long long> dist_from_x(N);
    DfsDistance(Y, -1, 0, dist_from_y, parent_rooted_x);
    DfsDistance(X, -1, 0, dist_from_x, parent_rooted_x);

    vector<int> path_xy;
    for (int i = Y; i != -1; i = parent_rooted_x[i])
    {
        in_path_xy[i] = true;
    }
    for (int i = 0; i < N; i++)
    {
        p_val[i] = min(dist_from_x[i], dist_from_y[i]);
        q_val[i] = max(dist_from_x[i], dist_from_y[i]);
    }

    int ret = CalculateWithCommon();
    ret = max(ret, CalculateWithoutCommon());

    return ret;
}
