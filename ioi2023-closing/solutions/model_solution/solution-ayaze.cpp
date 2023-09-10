// O(N log N)
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

        sorted_vals.push_back({2 * p_val[node], node});
        sorted_vals.push_back({q_val[node], -2 * node - 2});
        sorted_vals.push_back({q_val[node], -2 * node - 1});
    }
    sorted_vals.push_back({-1, -1});
    sort(sorted_vals.begin(), sorted_vals.end());

    Fenwick dist_tree(3 * N), q_counter(3 * N), p_counter(3 * N);

    auto GetIdx = [&](long long v, int id) -> int
    {
        pair<long long, int> temp = {v, id};
        auto lb = lower_bound(sorted_vals.begin(), sorted_vals.end(), temp);
        return lb - sorted_vals.begin();
    };

    auto GetOptimumLeftover = [&](long long left) -> int
    {
        int highest_idx = dist_tree.getHighestWithSumAtMost(left);
        long long current_total = dist_tree.query(highest_idx);
        int current_score = q_counter.query(highest_idx) + p_counter.query(highest_idx);

        if (q_counter.query(highest_idx) % 2 == 0)
        { // even q, everything is ok
            return current_score;
        }

        // odd q counter
        int highest_q_idx = 1 + q_counter.getHighestWithSumAtMost(q_counter.query(highest_idx) - 1);
        long long highest_q_val = sorted_vals[highest_q_idx].first;

        // case 1: replace highest q with next lowest p
        if (p_counter.query(highest_idx) != p_counter.query(3 * N))
        {
            int next_p_idx = 1 + p_counter.getHighestWithSumAtMost(p_counter.query(highest_idx));
            long long next_p_val = sorted_vals[next_p_idx].first;

            if (current_total - highest_q_val + next_p_val <= left)
            {
                return current_score;
            }
        }

        // case 2: replace highest p with highest q
        if (p_counter.query(highest_idx) > 0)
        {
            int highest_p_idx = 1 + p_counter.getHighestWithSumAtMost(p_counter.query(highest_idx) - 1);
            long long highest_p_val = sorted_vals[highest_p_idx].first;

            if (current_total - highest_p_val + highest_q_val <= left)
            {
                return current_score;
            }
        }

        // case 3: throw away one of the q
        return current_score - 1;
    };

    long long total_in_path = 0;
    int score_in_path = 0;
    int ret = 0;

    for (int i = 0; i < N; i++)
    {
        if (in_path_xy[i])
        {
            total_in_path += p_val[i];
            score_in_path++;
            continue;
        }

        // initially, everything is in p
        int idx = GetIdx(2 * p_val[i], i);
        dist_tree.add(idx, 2 * p_val[i]);
        p_counter.add(idx, 1);
    }

    for (int border = 0; border < N; border++)
    {
        int node = get<2>(sorted_diffs[border]);

        // convert to q
        if (in_path_xy[node])
        {
            total_in_path += (q_val[node] - p_val[node]);
            score_in_path++;
        }
        else
        {
            int idx_p = GetIdx(2 * p_val[node], node);
            dist_tree.add(idx_p, -2 * p_val[node]);
            p_counter.add(idx_p, -1);

            int idx_q = GetIdx(q_val[node], -2 * node - 2);
            dist_tree.add(idx_q, q_val[node]);
            q_counter.add(idx_q, 1);
            dist_tree.add(idx_q + 1, q_val[node]);
            q_counter.add(idx_q + 1, 1);
        }

        if (total_in_path <= K)
        {
            int opt_leftover = GetOptimumLeftover(2 * (K - total_in_path));
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
