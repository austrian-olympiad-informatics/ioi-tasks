#include "bits/stdc++.h"

using namespace std;

typedef pair<int, int> pt;
#define x first
#define y second

struct cmpyx {
  bool operator()(const pt& a, const pt& b) {
    return make_pair(a.y, a.x) < make_pair(b.y, b.x);
  }
};

vector<int> lis(vector<int> arr) {
  vector<int> seq, back(arr.size()), ans;
  for(int i=0; i<(int)arr.size(); i++) {
    int res = 0, lo = 1, hi = (int)seq.size();
    while (lo <= hi) {
      int mid = (lo+hi)/2;
      if (arr[seq[mid-1]] < arr[i]){
        res = mid;
        lo = mid + 1;
      }
      else hi = mid - 1;
    }
    if (res < (int)seq.size()) seq[res] = i;
    else seq.push_back(i);
    back[i] = res == 0 ? -1 : seq[res-1];
  }
  int at = seq.back();
  while (at != -1) ans.push_back(at), at = back[at];
  reverse(ans.begin(), ans.end());
  return ans;
}

vector <deque <pt>> get_chains(vector <pt>& test) {
  sort(test.begin(), test.end());
  vector <deque <pt> > chains;
  while(!test.empty()) {
    vector <int> Y;
    for(int i=0; i<(int)test.size(); i++) Y.push_back(test[i].y);
    auto lisy = lis(Y);
    reverse(Y.begin(), Y.end());
    auto rev_lisy = lis(Y);
    auto get_chain = [&]() {
      deque <pt> ret;
      int id = 0;
      vector <pt> new_test;
      for(int i=0; i<(int)test.size(); i++) {
        while(i < (int)test.size() and (id == (int)lisy.size() or lisy[id] != i)) {
          new_test.push_back(test[i]);
          i++;
        }
        if(id != (int)lisy.size()) ret.push_back(test[i]);
        id++;
      }
      chains.push_back(ret);
      test = new_test;
    };
    if(lisy.size() > rev_lisy.size()) {
      get_chain();
    } else {
      swap(lisy, rev_lisy);
      reverse(test.begin(), test.end());
      get_chain();
      reverse(chains.back().begin(), chains.back().end());
      reverse(test.begin(), test.end());
    }
  }
  return chains;
}

vector <pt> heuristic_get_longest(vector <pt> test) {
  auto chains = get_chains(test);
  vector <pt> ret;
  pt las = {0, 0};
  int dir = 0;
  for(auto chain : chains) {
    if(dir) reverse(chain.begin(), chain.end());
    dir ^= 1;
    ret.emplace_back(las.x, chain[0].y);
    if((int)chain.size() != 1) ret.push_back(chain[0]);
    for(int i=0; i+1<(int)chain.size(); i++) {
      if(i&1) ret.emplace_back(chain[i].x, chain[i+1].y);
      else ret.emplace_back(chain[i+1].x, chain[i].y);
    }
    ret.push_back(chain.back());
    las = chain.back();
  }
  return ret;
}

int main() {
  int n;
  cin >> n;
  vector <pt> test(n);
  for(int i=0; i<n; i++) cin >> test[i].x >> test[i].y;
  auto sol = heuristic_get_longest(test);
  cout << (int)sol.size() << "\n";
  for(auto s : sol) cout << s.x << " " << s.y << "\n";
}
