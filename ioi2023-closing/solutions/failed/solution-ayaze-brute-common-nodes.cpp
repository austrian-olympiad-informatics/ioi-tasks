// O(2^N N log N)
#include "closing.h"
#include <bits/stdc++.h>
using namespace std;

const int kStateX = 1, kStateY = 2;

vector<vector<pair<int, int>>> adj_list;
vector<long long> dist_from_x, dist_from_y;

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

void DfsState(int now, int prv, int state_flag, vector<int> &state_vec)
{
    for (pair<int, int> edge : adj_list[now])
    {
        int nxt = edge.first;
        if (nxt == prv)
            continue;
        DfsState(nxt, now, state_flag, state_vec);
        if (state_vec[nxt] & state_flag)
        {
            state_vec[now] |= state_flag;
        }
    }
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

    vector<int> node_state(N);
    int ret = 0;
    for (int common_mask = 0; common_mask < (1 << N); common_mask++)
    {
        for (int i = 0; i < N; i++)
        {
            node_state[i] = 0;
            if (common_mask & (1 << i))
                node_state[i] = kStateX | kStateY;
        }

        DfsState(X, -1, kStateX, node_state);
        DfsState(Y, -1, kStateY, node_state);

        vector<int> free_node_val;
        int score = 0;
        long long total = 0;
        for (int i = 0; i < N; i++)
        {
            if (node_state[i] == 0)
            {
                free_node_val.push_back(min(dist_from_x[i], dist_from_y[i]));
                continue;
            }

            long long val = 0;
            if (node_state[i] & kStateX)
            {
                score++;
                val = max(val, dist_from_x[i]);
            }
            if (node_state[i] & kStateY)
            {
                score++;
                val = max(val, dist_from_y[i]);
            }
            total += val;
        }

        if (total > K)
            continue;

        sort(free_node_val.begin(), free_node_val.end());
        for (long long val : free_node_val)
        {
            if (total + val > K)
                continue;
            total += val;
            score++;
        }

        ret = max(ret, score);
    }

    return ret;
}
