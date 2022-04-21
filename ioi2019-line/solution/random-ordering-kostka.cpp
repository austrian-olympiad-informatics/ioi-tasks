#include "bits/stdc++.h"

using namespace std;

typedef pair<int, int> pt;
#define x first
#define y second

const int REPEATS = 1000;

vector<pt> get_some_random_solution(vector <pt> test) {
  bool dir = false;
  pt cur = {0, 0};
  vector <pt> ret;
  for(int i=0; i<2*(int)test.size(); i++) {
    if(dir) {
      cur.x = test[i/2].x;
      dir ^= 1;
    } else {
      cur.y = test[i/2].y;
      dir ^= 1;
    }
    if(i&1 and rand()%10 < 5) dir ^= 1;
    ret.push_back(cur);
  }
  cur = {0, 0};
  vector <pt> final_ret;
  for(int i=0; i<(int)ret.size(); i++) {
    if(i+1 < (int)ret.size() and ((cur.x < ret[i].x and ret[i].x < ret[i+1].x) or
       (cur.y < ret[i].y and ret[i].y < ret[i+1].y) or
       (cur.x > ret[i].x and ret[i].x > ret[i+1].x) or
       (cur.y > ret[i].y and ret[i].y > ret[i+1].y))) {
      i++;
    }
    final_ret.push_back(ret[i]);
    cur = final_ret.back();
  }
  return final_ret;
}

vector <pt> random_ordering_sol(vector <pt> test) {
  auto sol = get_some_random_solution(test);
  for(int i=1; i<REPEATS; i++) {
    random_shuffle(test.begin(), test.end());
    auto other_sol = get_some_random_solution(test);
    if(other_sol.size() < sol.size()) {
      sol = other_sol;
    }
  }
  return sol;
}

int main() {
  srand(15061994);
  int n;
  cin >> n;
  vector <pt> test(n);
  for(int i=0; i<n; i++) cin >> test[i].x >> test[i].y;
  auto sol = random_ordering_sol(test);
  cout << (int)sol.size() << "\n";
  for(auto s : sol) cout << s.x << " " << s.y << "\n";
}
