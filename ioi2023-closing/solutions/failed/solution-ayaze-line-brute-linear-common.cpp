// O(N^2 log N), line graph
#include "closing.h"
#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int>>> adj_list;
vector<long long> dist_from_x, dist_from_y;

void DfsDistance(int now, int prv, long long current_dist, vector<long long> &dist_vec)
{
    dist_vec[now] = current_dist;

    for (pair<int, int> edge : adj_list[now])
    {
        int nxt = edge.first;
        long long new_dist = edge.second + current_dist;
        if (nxt == prv)
            continue;
        DfsDistance(nxt, now, new_dist, dist_vec);
    }
}

int CalculateWithCommon(int N, int X, int Y, long long K)
{
    int ret = 0;

    vector<tuple<long long, long long, int>> sorted_diff;
    for (int i = 0; i < N; i++)
    {
        long long p = min(dist_from_x[i], dist_from_y[i]);
        long long q = max(dist_from_x[i], dist_from_y[i]);
        sorted_diff.push_back(make_tuple(q - p, p, i));
    }
    sort(sorted_diff.begin(), sorted_diff.end());

    for (int common_prefix = 0; common_prefix < static_cast<int>(sorted_diff.size()); common_prefix++)
    {
        int common_left = N, common_right = -1;
        for (int i = 0; i <= common_prefix; i++)
        {
            common_left = min(common_left, get<2>(sorted_diff[i]));
            common_right = max(common_right, get<2>(sorted_diff[i]));
        }

        long long total = 0;
        int score = 0;

        vector<long long> free_nodes;
        for (int i = 0; i < N; i++)
        {
            long long p = min(dist_from_x[i], dist_from_y[i]);
            long long q = max(dist_from_x[i], dist_from_y[i]);

            if (common_left <= i && i <= common_right)
            {
                total += q;
                score += 2;
            }
            else if (i >= X && i <= Y)
            {
                total += p;
                score += 1;
            }
            else
            {
                free_nodes.push_back(p);
            }
        }

        sort(free_nodes.begin(), free_nodes.end());
        for (long long d : free_nodes)
        {
            if (total + d <= K)
            {
                total += d;
                score++;
            }
        }

        if (total <= K)
        {
            ret = max(ret, score);
        }
    }

    return ret;
}

int CalculateWithoutCommon(int N, long long K)
{
    vector<long long> sorted_dist;
    for (int i = 0; i < N; i++)
    {
        sorted_dist.push_back(dist_from_x[i]);
        sorted_dist.push_back(dist_from_y[i]);
    }
    sort(sorted_dist.begin(), sorted_dist.end());

    long long total = 0;
    int score = 0;
    for (long long d : sorted_dist)
    {
        if (total + d <= K)
        {
            total += d;
            score++;
        }
    }

    return score;
}

int max_score(int N, int X, int Y, long long K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    adj_list.assign(N, vector<pair<int, int>>());
    dist_from_x.resize(N);
    dist_from_y.resize(N);

    for (int i = 0; i < static_cast<int>(U.size()); i++)
    {
        adj_list[U[i]].push_back({V[i], W[i]});
        adj_list[V[i]].push_back({U[i], W[i]});
    }

    DfsDistance(X, -1, 0, dist_from_x);
    DfsDistance(Y, -1, 0, dist_from_y);

    int ret = CalculateWithCommon(N, X, Y, K);
    ret = max(ret, CalculateWithoutCommon(N, K));

    return ret;
}
