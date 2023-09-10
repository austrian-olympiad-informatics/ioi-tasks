// O(N log N)
// WA: always take q when we extend the border
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

    vector<pair<long long, int>> sorted_vals;
    for (int i = 0; i < static_cast<int>(sorted_diffs.size()); i++)
    {
        int node = get<2>(sorted_diffs[i]);

        sorted_vals.push_back({p_val[node], node});
    }
    sorted_vals.push_back({-1, -1});
    sort(sorted_vals.begin(), sorted_vals.end());

    Fenwick dist_tree(N), p_counter(N);

    auto GetIdx = [&](long long v, int id) -> int
    {
        pair<long long, int> temp = {v, id};
        auto lb = lower_bound(sorted_vals.begin(), sorted_vals.end(), temp);
        return lb - sorted_vals.begin();
    };

    long long total = 0;
    int score = 0;
    int ret = 0;

    for (int i = 0; i < N; i++)
    {
        if (in_path_xy[i])
        {
            total += p_val[i];
            score++;
            continue;
        }

        // initially, everything is in p
        int idx = GetIdx(p_val[i], i);
        dist_tree.add(idx, p_val[i]);
        p_counter.add(idx, 1);
    }

    for (int border = 0; border < N; border++)
    {
        int node = get<2>(sorted_diffs[border]);

        // convert to q
        if (in_path_xy[node])
        {
            total += (q_val[node] - p_val[node]);
            score++;
        }
        else
        {
            int idx_p = GetIdx(p_val[node], node);
            dist_tree.add(idx_p, -p_val[node]);
            p_counter.add(idx_p, -1);

            total += q_val[node];
            score += 2;
        }

        if (total <= K)
        {
            int highest_idx = dist_tree.getHighestWithSumAtMost(K - total);
            int added = p_counter.query(highest_idx);
            ret = max(ret, score + added);
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
