#include <bits/stdc++.h>

using namespace std;

const int ts = 1 << 19, to = 1 << 18;
int n, m, d, a[200100], ind[200100], p[200100], t[ts][4];

void update(int i, int x) {
  i += to;
  t[i][0] = t[i][1] = t[i][2] = x;
  t[i][3] = min(0, x);
  while (i >>= 1) {
    t[i][0] = t[i << 1][0] + t[i << 1 | 1][0];
    t[i][1] = min(t[i << 1][1], t[i << 1][0] + t[i << 1 | 1][1]);
    t[i][2] = min(t[i << 1 | 1][2], t[i << 1 | 1][0] + t[i << 1][2]);
    t[i][3] = min(t[i << 1][2] + t[i << 1 | 1][1], min(t[i << 1][3], t[i << 1 | 1][3]));
  }
}

int main() {
  cin >> n >> m >> d;
  for (int i = 0; i < n; i++)
    cin >> a[i], ind[i] = i;
  for (int i = 0; i < m; i++)
    cin >> a[n + i], ind[n + i] = n + i;
  sort(ind, ind + n + m, [&](int i, int j) { return a[i] < a[j]; });
  for (int i = 0; i < n + m; i++)
    p[ind[i]] = i;
  set<pair<int, int>> c;
  for (int i = 0; i < n + m; i++) {
    auto it = c.lower_bound({p[i], i});
    if (it != c.end())
      update(p[i], a[it->second] - a[i] - d);
    if (it != c.begin())
      it--, update(p[it->second], a[i] - a[it->second] - d);
    c.emplace(p[i], i);
    if (i >= n)
      cout << -t[1][3] / 2.0 << (i < n + m - 1 ? " " : "\n");
  }
}