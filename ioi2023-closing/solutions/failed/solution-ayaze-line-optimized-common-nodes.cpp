// O(N^2 log N), line graph
#include "closing.h"
#include <bits/stdc++.h>
using namespace std;

struct Fenwick
{
    vector<long long> tree;
    int sz;

    Fenwick(int _sz = 0)
    {
        sz = _sz;
        tree.clear();
        tree.resize(sz + 5, 0);
    }

    void add(int idx, long long val)
    {
        for (int i = idx; i <= sz; i += i & -i)
        {
            tree[i] += val;
        }
    }

    long long query(int idx)
    {
        long long ret = 0;
        for (int i = idx; i > 0; i -= i & -i)
        {
            ret += tree[i];
        }
        return ret;
    }

    int getHighestWithSumAtMost(long long k)
    {
        int idx = 0;
        long long total = 0;

        int lg = 0;
        while ((1 << lg) <= sz)
            lg++;

        for (int i = lg - 1; i >= 0; i--)
        {
            if (idx + (1 << i) <= sz && total + tree[idx + (1 << i)] <= k)
            {
                idx += (1 << i);
                total += tree[idx];
            }
        }

        return idx;
    }
};

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

int CalculateWithCommon(int N, int X, int Y, long long K)
{
    vector<pair<long long, int>> sorted_dist;
    for (int i = 0; i < N; i++)
    {
        long long p = min(dist_from_x[i], dist_from_y[i]);
        sorted_dist.push_back({p, i});
    }
    sorted_dist.push_back({-1, -1});
    sort(sorted_dist.begin(), sorted_dist.end());

    auto GetIdx = [&](long long v, int id) -> int
    {
        pair<long long, int> temp = {v, id};
        auto lb = lower_bound(sorted_dist.begin(), sorted_dist.end(), temp);
        return lb - sorted_dist.begin();
    };

    int ret = 0;
    for (int common_left = 0; common_left <= Y; common_left++)
    {
        long long total = 0;
        int score = 0;
        int leftmost_right = max(common_left, X);

        Fenwick p_counter(N), dist_tree(N);

        for (int i = 0; i < N; i++)
        {
            if (common_left <= i && i <= leftmost_right)
            {
                score += 2;
                total += max(dist_from_x[i], dist_from_y[i]);
            }
            else if (X <= i && i <= Y)
            {
                score += 1;
                total += min(dist_from_x[i], dist_from_y[i]);
            }
            else
            {
                long long p = min(dist_from_x[i], dist_from_y[i]);
                int idx = GetIdx(p, i);
                p_counter.add(idx, 1);
                dist_tree.add(idx, p);
            }
        }

        if (total <= K)
        {
            int highest_idx = dist_tree.getHighestWithSumAtMost(K - total);
            int current_score = score + static_cast<int>(p_counter.query(highest_idx));
            ret = max(ret, current_score);
        }

        for (int common_right = leftmost_right + 1; common_right < N; common_right++)
        {
            long long p = min(dist_from_x[common_right], dist_from_y[common_right]);
            long long q = max(dist_from_x[common_right], dist_from_y[common_right]);

            if (X <= common_right && common_right <= Y)
            {
                score += 1;
                total += (q - p);
            }
            else
            {
                int idx = GetIdx(p, common_right);
                p_counter.add(idx, -1);
                dist_tree.add(idx, -p);
                score += 2;
                total += q;
            }

            if (total <= K)
            {
                int highest_idx = dist_tree.getHighestWithSumAtMost(K - total);
                int current_score = score + static_cast<int>(p_counter.query(highest_idx));
                ret = max(ret, current_score);
            }
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
