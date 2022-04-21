#include <cassert>
#include <cstdio>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

map<int, set<int>> X, Y;
int n, x, y;

void clear_x() {
if (!X.count(x)) return;

n -= X[x].size();
for (int yy : X[x]) {
auto sz = Y[yy].erase(x);
assert(sz == 1);
if (Y[yy].empty()) Y.erase(yy);
}
X.erase(x);
}

void clear_y() {
if (!Y.count(y)) return;

n -= Y[y].size();
for (int xx : Y[y]) {
auto sz = X[xx].erase(y);
assert(sz == 1);
if (X[xx].empty()) X.erase(xx);
}
Y.erase(y);
}

void state() {
for (auto it : X) for (int xx : it.second) fprintf(stderr, "X[%d] = %d\n", it.first, xx);
for (auto it : Y) for (int yy : it.second) fprintf(stderr, "Y[%d] = %d\n", it.first, yy);
fprintf(stderr, "X %d %d\n", X.begin()->first, X.rbegin()->first);
fprintf(stderr, "Y %d %d\n", Y.begin()->first, Y.rbegin()->first);
}

int main() {
scanf("%d", &n);
for (int i = 0; i < n; i++) {
scanf("%d %d", &x, &y);
auto ix = X[x].insert(y);
assert(ix.second);
auto iy = Y[y].insert(x);
assert(ix.second);
}

x = 0;
y = 0;
vector<pair<int, int>> res;
while (n) {
x = X.rbegin()->first;
clear_y();
res.push_back(make_pair(x, y));

if (!n) break;

y = Y.rbegin()->first;
clear_x();
res.push_back(make_pair(x, y));

if (!n) break;

x = X.begin()->first;
clear_y();
res.push_back(make_pair(x, y));

if (!n) break;

y = Y.begin()->first;
clear_x();
res.push_back(make_pair(x, y));
}

printf("%lu\n", res.size());
for (auto i : res) printf("%d %d\n", i.first, i.second);
}
