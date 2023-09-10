// O(N^4)
#include "closing.h"
#include <bits/stdc++.h>
using namespace std;

const int kVisited = 1;
const int kTypeSingle = 1, kTypeBoth = 2;
const long long kInf = 4e18;

vector<vector<pair<int, int>>> adj_list;
vector<long long> dist_from_x, dist_from_y;

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

void DfsComponent(int now, vector<int> &component, vector<int> &state)
{
    state[now] = kVisited;
    for (pair<int, int> edge : adj_list[now])
    {
        int nxt = edge.first;
        if (state[nxt] == kVisited)
            continue;
        DfsComponent(nxt, component, state);
    }
    component.push_back(now);
}

int Knapsack(vector<pair<int, int>> free_nodes, long long K)
{
    int sz = free_nodes.size();
    vector<vector<long long>> dp_mat(sz + 1, vector<long long>(2 * sz + 1, kInf));
    dp_mat[0][0] = 0;

    for (int i = 0; i < sz; i++)
    {
        int node = free_nodes[i].first;
        int typ = free_nodes[i].second;
        long long p = min(dist_from_x[node], dist_from_y[node]);
        long long q = max(dist_from_x[node], dist_from_y[node]);

        for (int j = 0; j <= 2 * i; j++)
        {
            dp_mat[i + 1][j] = min(dp_mat[i + 1][j], dp_mat[i][j]);
            dp_mat[i + 1][j + 1] = min(dp_mat[i + 1][j + 1], dp_mat[i][j] + p);
            if (typ == kTypeBoth)
            {
                dp_mat[i + 1][j + 2] = min(dp_mat[i + 1][j + 2], dp_mat[i][j] + q);
            }
        }
    }

    int ret = 0;
    for (int i = 0; i <= 2 * sz; i++)
    {
        if (dp_mat[sz][i] <= K)
        {
            ret = i;
        }
    }

    return ret;
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

    vector<int> parent_rooted_x(N);
    DfsDistance(Y, -1, 0, dist_from_y, parent_rooted_x);
    DfsDistance(X, -1, 0, dist_from_x, parent_rooted_x);

    vector<int> path_xy;
    for (int i = Y; i != -1; i = parent_rooted_x[i])
    {
        path_xy.push_back(i);
    }

    int ret = 0;
    int path_xy_size = static_cast<int>(path_xy.size());
    vector<int> node_state(N);
    for (int common_left = 0; common_left < path_xy_size; common_left++)
    {
        for (int common_right = common_left; common_right < path_xy_size; common_right++)
        {
            fill(node_state.begin(), node_state.end(), 0);
            for (int i = 0; i < path_xy_size; i++)
            {
                node_state[path_xy[i]] = kVisited;
            }

            long long total = 0;
            int score = 0;

            vector<pair<int, int>> free_nodes;
            for (int i = 0; i < path_xy_size; i++)
            {
                int node = path_xy[i];
                int typ = -1;

                if (common_left <= i && i <= common_right)
                {
                    total += max(dist_from_x[node], dist_from_y[node]);
                    score += 2;
                    typ = kTypeBoth;
                }
                else
                {
                    total += min(dist_from_x[node], dist_from_y[node]);
                    score += 1;
                    typ = kTypeSingle;
                }

                vector<int> component;
                DfsComponent(node, component, node_state);
                component.pop_back();

                for (int c : component)
                {
                    free_nodes.push_back({c, typ});
                }
            }

            if (total > K)
                continue;
            int opt = Knapsack(free_nodes, K - total);
            ret = max(ret, opt + score);
        }
    }

    // no common
    {
        vector<long long> sorted_dist;
        for (int i = 0; i < N; i++)
        {
            sorted_dist.push_back(dist_from_y[i]);
            sorted_dist.push_back(dist_from_x[i]);
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
        ret = max(ret, score);
    }

    return ret;
}
