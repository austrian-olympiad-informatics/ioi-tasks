#include <bits/stdc++.h>

using namespace std;

const int ts = 1 << 19, o = 1 << 18;
int n, q, p[200100], pr[200100], nxt[200100], r[1000100], c[1000100], t[ts];
vector<int> qu[200100];

void update(int i, int x) {
  i += o;
  t[i] = x;
  while (i >>= 1)
    t[i] = t[i << 1] + t[i << 1 | 1];
}

int find(int &x, int i = 1) {
  while (i < o)
    if (t[i << 1] < x)
      x -= t[i << 1], i = i << 1 | 1;
    else
      i = i << 1;
  return i - o;
}

int main() {
  cin >> n >> q;
  for (int i = 0; i < n; i++)
    cin >> p[i], pr[p[i]] = i, nxt[i] = n;
  for (int i = 0; i < q; i++) {
    int t;
    cin >> t >> c[i];
    t = min(t, n);
    qu[t].push_back(i);
  }
  stack<int> s;
  for (int i = 0; i < n; i++) {
    while (!s.empty() && p[i] > p[s.top()])
      nxt[s.top()] = i, s.pop();
    s.push(i);
  }
  set<int> ps;
  ps.insert(n);
  for (int i = 0; i < n; i = nxt[i])
    update(p[i], nxt[i] - i), ps.insert(i);
  for (int s = 0; s <= n; s++) {
    for (int i : qu[s]) {
      int j = find(c[i]);
      r[i] = p[pr[j] + c[i] - 1];
    }
    int cn = n / 2 + 1;
    int i = find(cn);
    if (cn == 1)
      continue;
    update(i, cn - 1);
    int np = *ps.upper_bound(pr[i]);
    for (i = pr[i] + cn - 1; i < np; i = nxt[i])
      update(p[i], min(np, nxt[i]) - i), ps.insert(i);
  }
  for (int i = 0; i < q; i++)
    cout << r[i] << "\n";
}