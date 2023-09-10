// O(N^3 log N), line graph
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
    for (int common_left = 0; common_left < N; common_left++)
    {
        for (int common_right = common_left; common_right < N; common_right++)
        {
            long long total = 0;
            int score = 0;

            for (int i = 0; i < N; i++)
            {
                if (common_left <= i && i <= common_right)
                {
                    total += max(dist_from_x[i], dist_from_y[i]);
                    score += 2;
                }
                else if (i >= X && common_left > i)
                {
                    total += dist_from_x[i];
                    score += 1;
                }
                else if (i <= Y && common_right < i)
                {
                    total += dist_from_y[i];
                    score += 1;
                }
            }

            if (total > K)
                continue;
            int left = min(X - 1, common_left - 1);
            int right = max(Y + 1, common_right + 1);

            vector<int> free_nodes;
            for (int i = 0; i < N; i++)
            {
                if (i <= left || i >= right)
                {
                    free_nodes.push_back(min(dist_from_x[i], dist_from_y[i]));
                }
            }
            sort(free_nodes.begin(), free_nodes.end());

            for (int p : free_nodes)
            {
                if (total + p <= K)
                {
                    total += p;
                    score++;
                }
            }
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
