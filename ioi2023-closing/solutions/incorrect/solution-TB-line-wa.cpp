#include "closing.h"
#include <bits/stdc++.h>

#define ll long long int

using namespace std;

ll get_pref_sum(vector<ll> &v, int i, int j)
{
    return v[j] - (i == 0 ? 0 : v[i - 1]);
}

int max_score(int n, int X, int Y, ll K,
              vector<int> U, vector<int> V, vector<int> W)
{

    vector<vector<pair<int, int>>> graph(n);

    for (int i = 0; i < n - 1; i++)
    {
        graph[U[i]].push_back({V[i], W[i]});
        graph[V[i]].push_back({U[i], W[i]});
    }

    vector<ll> distX(n, -1);
    vector<ll> distY(n, -1);
    vector<ll> totalcostX(n, 0);
    vector<ll> totalcostY(n, 0);

    stack<int> st;
    st.push(X);
    distX[X] = 0;
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        for (auto pr : graph[v])
        {
            int u = pr.first;
            int w = pr.second;
            if (distX[u] == -1)
            {
                distX[u] = distX[v] + w;
                totalcostX[u] = totalcostX[v] + distX[u];
                st.push(u);
            }
        }
    }
    st.push(Y);
    distY[Y] = 0;
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        for (auto pr : graph[v])
        {
            int u = pr.first;
            int w = pr.second;
            if (distY[u] == -1)
            {
                distY[u] = distY[v] + w;
                totalcostY[u] = totalcostY[v] + distY[u];
                st.push(u);
            }
        }
    }

    vector<ll> distXY(n);
    vector<ll> distXY_pref(n, 0);
    for (int i = 0; i < n; i++)
    {
        distXY[i] = max(distX[i], distY[i]);
        distXY_pref[i] = distXY[i] + (i == 0 ? 0 : distXY_pref[i - 1]);
    }

    set<pair<ll, int>> costs;
    vector<int> reached_by(n, 0);
    for (int i = 0; i < n; i++)
    {
        costs.insert({min(distX[i], distY[i]), i});
    }

    int ans = 0;
    ll cost = 0;
    while (!costs.empty())
    {
        auto it = costs.begin();
        int v = it->second;
        int c = it->first;
        // cout << "rem: " << v << ' ' << c << ' ' << cost << endl;
        costs.erase(it);
        if (cost + c <= K)
        {
            cost += c;
            ans++;
            reached_by[v]++;
            if (reached_by[v] == 1)
            {
                // cout << "insert: " << distXY[v] - c << endl;
                costs.insert({distXY[v] - c, v});
            }
        }
        else
        {
            break;
        }
    }

    return ans;
}
