#include <bits/stdc++.h>

using namespace std;

int n;

pair<int, int> expr() {
  char c1, c2;
  cin >> c1;
  if (c1 == '?') {
    n++;
    return {0, 0};
  }
  cin >> c2 >> c1 >> c1;
  pair<int, int> i1 = expr();
  cin >> c1;
  pair<int, int> i2 = expr();
  cin >> c1;
  if (c2 == 'i')
    return {min(i1.first, i2.first), i1.second + i2.second + 1};
  else
    return {i1.first + i2.first + 1, min(i1.second, i2.second)};
}

int main() {
  pair<int, int> r = expr();
  cout << n - (r.first + r.second) << "\n";
}