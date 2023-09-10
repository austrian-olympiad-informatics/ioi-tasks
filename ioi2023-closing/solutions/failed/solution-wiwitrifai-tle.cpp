/*
TLE: This can be O(N^2 log N) because it tries all possible cases such that
picker_two.top()/2 < picker_one.top() and picker_two.top()/2 > picker_one.top()
are covered.

picker_one.top() is the highest picked cost in all nodes reachable from one source.
picker_two.top() is the highest picked cost in all nodes reachable from both sources.

Possible counter example:
200000 0 1 1200000
0 1 2
0 2 1000000
0 3 1
0 4 1
0 5 1
.
.
.
0 199998 1
0 199999 1
*/
#include "closing.h"

#include <bits/stdc++.h>

using namespace std;

constexpr long long kInf = 1e17;

template <typename T>
using min_priority_queue = priority_queue<T, vector<T>, greater<T>>;

struct GreedyPicker
{
    vector<long long> vals;
    priority_queue<pair<long long, int>> ins;
    min_priority_queue<pair<long long, int>> outs;
    vector<bool> is_ins, is_outs, active;
    long long total;
    int cnt;

    GreedyPicker(vector<long long> _vals, bool _active) : vals(_vals)
    {
        int n = vals.size();
        is_ins.assign(n, false);
        is_outs.assign(n, _active);
        active.assign(n, _active);
        cnt = 0;
        total = 0;
        if (_active)
        {
            for (int i = 0; i < n; ++i)
            {
                outs.emplace(vals[i], i);
            }
        }
    }

    bool inc()
    {
        while (!outs.empty())
        {
            auto [val, id] = outs.top();
            outs.pop();
            is_outs[id] = false;
            if (!active[id])
                continue;
            total += val;
            ++cnt;
            ins.emplace(val, id);
            is_ins[id] = true;
            return true;
        }
        return false;
    }

    bool dec()
    {
        while (!ins.empty())
        {
            auto [val, id] = ins.top();
            ins.pop();
            is_ins[id] = false;
            if (!active[id])
                continue;
            total -= val;
            --cnt;
            outs.emplace(val, id);
            is_outs[id] = true;
            return true;
        }
        return false;
    }

    long long top()
    {
        while (!ins.empty() && !active[ins.top().second])
            ins.pop();
        return ins.empty() ? -1 : ins.top().first;
    }

    long long check_inc()
    {
        while (!outs.empty() && !active[outs.top().second])
            outs.pop();
        return outs.empty() ? kInf : outs.top().first;
    }

    void deactivate(int v)
    {
        if (!active[v])
            return;
        if (is_ins[v])
        {
            --cnt;
            total -= vals[v];
        }
        active[v] = false;
    }

    void activate(int v)
    {
        if (active[v])
            return;
        active[v] = true;
        if (is_ins[v])
        {
            ++cnt;
            total += vals[v];
        }
        else if (!is_outs[v])
        {
            if (vals[v] <= top())
            {
                ins.emplace(vals[v], v);
                ++cnt;
                total += vals[v];
            }
            else
            {
                outs.emplace(vals[v], v);
            }
        }
    }
};

vector<vector<pair<int, int>>> tree;

void dfs(int v, int par, vector<long long> &dist)
{
    for (auto [u, w] : tree[v])
    {
        if (u == par)
            continue;
        dist[u] = dist[v] + w;
        dfs(u, v, dist);
    }
}

int max_score(int N, int X, int Y, long long K,
              vector<int> U, vector<int> V, vector<int> W)
{
    tree.assign(N, {});
    for (int i = 0; i < N - 1; ++i)
    {
        tree[U[i]].emplace_back(V[i], W[i]);
        tree[V[i]].emplace_back(U[i], W[i]);
    }
    vector<long long> from_x(N, 0), from_y(N, 0);
    dfs(X, X, from_x);
    dfs(Y, Y, from_y);
    vector<long long> val1(N), val2(N), val_diff(N);
    for (int i = 0; i < N; ++i)
    {
        val1[i] = min(from_x[i], from_y[i]);
        val2[i] = max(from_x[i], from_y[i]);
        val_diff[i] = val2[i] - val1[i];
    }
    vector<int> sorted(N);
    iota(sorted.begin(), sorted.end(), 0);
    int ans = 0;
    {
        sort(sorted.begin(), sorted.end(), [&](int l, int r)
             { return val1[l] < val1[r]; });
        long long total = 0;
        for (auto v : sorted)
        {
            if (total + val1[v] > K)
                break;
            ++ans;
            total += val1[v];
        }
    }

    GreedyPicker picker_one(val1, true), picker_two(val2, false);
    auto in_path = [&](int v)
    {
        return from_x[v] + from_y[v] == from_x[Y];
    };
    int cur = 0;
    for (int v = 0; v < N; ++v)
    {
        if (in_path(v))
        {
            K -= val1[v];
            ++cur;
            picker_one.deactivate(v);
        }
    }
    sort(sorted.begin(), sorted.end(), [&](int l, int r)
         { return make_pair(val_diff[l], val1[l]) < make_pair(val_diff[r], val1[r]); });
    for (auto v : sorted)
    {
        if (!in_path(v))
        {
            picker_two.activate(v);
            picker_one.deactivate(v);
        }
        else
        {
            K -= val_diff[v];
            ++cur;
        }
        if (K < 0)
            break;
        while (picker_one.total + picker_two.total > K)
        {
            if (picker_two.top() < 2LL * picker_one.top())
            {
                picker_one.dec();
            }
            else
            {
                picker_two.dec();
            }
        }
        ans = max(ans, cur + picker_two.cnt * 2 + picker_one.cnt);
        while (true)
        {
            auto add = picker_one.check_inc();
            if (add >= kInf)
                break;
            if (picker_one.total + picker_two.total + add <= K)
            {
                picker_one.inc();
                ans = max(ans, cur + picker_two.cnt * 2 + picker_one.cnt);
            }
            else
            {
                if (picker_one.top() * 2 > picker_two.top())
                    break;
                if (!picker_two.dec())
                    break;
            }
        }
        while (true)
        {
            auto add = picker_two.check_inc();
            if (add >= kInf)
                break;
            if (picker_one.total + picker_two.total + add <= K)
            {
                picker_two.inc();
                ans = max(ans, cur + picker_two.cnt * 2 + picker_one.cnt);
            }
            else
            {
                if (picker_one.top() * 2 < picker_two.top())
                    break;
                if (!picker_one.dec())
                    break;
            }
        }
    }
    return ans;
}
