#include "closing.h"

#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int>>> adj;

void fill_distance(int u, int from, vector<long long> &distance)
{
    for (auto [v, w] : adj[u])
    {
        if (v != from)
        {
            distance[v] = distance[u] + w;
            fill_distance(v, u, distance);
        }
    }
}

int non_intersecting(
    int N, long long K,
    const vector<long long> &fromX, const vector<long long> &fromY)
{
    vector<long long> smaller(N);
    for (int i = 0; i < N; ++i)
    {
        smaller[i] = min(fromX[i], fromY[i]);
    }
    sort(smaller.begin(), smaller.end());
    for (int i = 0; i < N; ++i)
    {
        if (smaller[i] <= K)
        {
            K -= smaller[i];
        }
        else
        {
            return i;
        }
    }
    return N;
}

int max_score(int N, int X, int Y, long long K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    adj.assign(N, vector<pair<int, int>>());
    for (int i = 0; i < N - 1; ++i)
    {
        adj[U[i]].emplace_back(V[i], W[i]);
        adj[V[i]].emplace_back(U[i], W[i]);
    }
    vector<long long> fromX(N), fromY(N);
    fill_distance(X, -1, fromX);
    fill_distance(Y, -1, fromY);
    vector<bool> is_in_path(N);
    vector<int> in_path;
    for (int i = 0; i < N; ++i)
    {
        is_in_path[i] = fromX[i] + fromY[i] == fromX[Y];
        if (is_in_path[i])
        {
            in_path.push_back(i);
        }
    }

    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b)
         { return make_pair(max(fromX[a], fromY[a]) - min(fromX[a], fromY[a]), a) < make_pair(max(fromX[b], fromY[b]) - min(fromX[b], fromY[b]), b); });
    multiset<int> cost_one, cost_two;
    for (int i = 0; i < N; ++i)
    {
        cost_one.insert(min(fromX[i], fromY[i]));
    }

    int answer = non_intersecting(N, K, fromX, fromY);
    for (int u : in_path)
    {
        K -= min(fromX[u], fromY[u]);
        cost_one.erase(cost_one.find(min(fromX[u], fromY[u])));
    }
    if (K >= 0)
    {
        for (int i = 0; i < N; ++i)
        {
            int u = order[i];
            if (is_in_path[u])
            {
                cost_one.insert(max(fromX[u], fromY[u]) - min(fromX[u], fromY[u]));
            }
            else
            {
                cost_one.erase(cost_one.find(min(fromX[u], fromY[u])));
                cost_two.insert(max(fromX[u], fromY[u]));
            }
            multiset<int>::iterator pt_one = cost_one.begin();
            multiset<int>::iterator pt_two = cost_two.begin();
            long long rem = K;
            int cur = in_path.size();
            while (pt_one != cost_one.end() || pt_two != cost_two.end())
            {
                if (pt_two != cost_two.end() && (*pt_two) <= rem)
                {
                    answer = max(answer, cur + 2);
                }
                auto check_one = [&]()
                {
                    if ((*pt_one) <= rem)
                    {
                        rem -= (*pt_one);
                        ++cur;
                        ++pt_one;
                    }
                    else
                    {
                        pt_one = cost_one.end();
                    }
                };
                auto check_two = [&]()
                {
                    if ((*pt_two) <= rem)
                    {
                        rem -= (*pt_two);
                        cur += 2;
                        ++pt_two;
                    }
                    else
                    {
                        pt_two = cost_two.end();
                    }
                };
                if (pt_one != cost_one.end() && pt_two != cost_two.end())
                {
                    if ((*pt_two) <= 2 * (*pt_one))
                    {
                        check_two();
                    }
                    else
                    {
                        check_one();
                    }
                }
                else if (pt_one != cost_one.end())
                {
                    check_one();
                }
                else if (pt_two != cost_two.end())
                {
                    check_two();
                }
            }
            answer = max(answer, cur);
        }
    }
    return answer;
}
