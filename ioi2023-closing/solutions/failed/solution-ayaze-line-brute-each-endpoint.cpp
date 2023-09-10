// O(N^5), line graph
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
        if (nxt == prv)
            continue;
        long long new_dist = edge.second + current_dist;
        DfsDistance(nxt, now, new_dist, dist_vec);
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

    int ret = 0;
    for (int x_left = 0; x_left <= X; x_left++)
    {
        for (int x_right = X; x_right < N; x_right++)
        {
            for (int y_left = 0; y_left <= Y; y_left++)
            {
                for (int y_right = Y; y_right < N; y_right++)
                {
                    long long total = 0;

                    for (int i = 0; i < N; i++)
                    {
                        long long cost = 0;
                        if (x_left <= i && i <= x_right)
                            cost = max(cost, dist_from_x[i]);
                        if (y_left <= i && i <= y_right)
                            cost = max(cost, dist_from_y[i]);
                        total += cost;
                    }

                    if (total <= K)
                    {
                        ret = max(ret, x_right - x_left + 1 + y_right - y_left + 1);
                    }
                }
            }
        }
    }

    return ret;
}
