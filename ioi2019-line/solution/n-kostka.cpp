#include "bits/stdc++.h"

using namespace std;

typedef pair<int, int> pt;
#define x first
#define y second

vector <pt> trivial(vector <pt> test) {
  vector <pt> ret;
  sort(test.begin(), test.end());
  ret.emplace_back(test[0].x, 0);
  for(int i=0; i<(int)test.size(); i++) {
    ret.emplace_back(test[i].x, test[i].y);
    if(i+1 != (int)test.size()) ret.emplace_back(test[i].x, test[i+1].y);
  }
  ret.erase(unique(ret.begin(), ret.end()), ret.end());
  return ret;
}

int main() {
  int n;
  cin >> n;
  vector <pt> test(n);
  for(int i=0; i<n; i++) cin >> test[i].x >> test[i].y;
  auto trivial_sol = trivial(test);
  cout << (int)trivial_sol.size() << "\n";
  for(auto s : trivial_sol) cout << s.x << " " << s.y << "\n";
}
