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

int main() {
  int n;
  cin >> n;
  vector <pt> P(n);
  for(int i=0; i<n; i++) cin >> P[i].x >> P[i].y;
  set <pt> Xs(P.begin(), P.end());
  set <pt, cmpyx> Ys(P.begin(), P.end());
  vector <vector <pt> > spiral;
  vector <pt> diagonals[2];
  vector <pt> helper;
  while(!Xs.empty()) {
    set <pt> considered;
    considered.insert(*(Xs.begin()));
    considered.insert(*(Xs.rbegin()));
    considered.insert(*(Ys.begin()));
    considered.insert(*(Ys.rbegin()));
    if(considered.size() == 4) {
      spiral.push_back(vector<pt>());
      int minx = (*Xs.begin()).x, maxx = (*Xs.rbegin()).x;
      int miny = (*Ys.begin()).y, maxy = (*Ys.rbegin()).y;
      spiral.back().emplace_back(minx, miny);
      spiral.back().emplace_back(minx, maxy);
      spiral.back().emplace_back(maxx, maxy);
      spiral.back().emplace_back(maxx, miny);
      helper.push_back(*Ys.begin());
      for(auto c : considered) {
        Xs.erase(c);
        Ys.erase(c);
      }
    } else {
      for(auto c : considered) {
        if((c == *Xs.begin() and c == *Ys.begin()) or (c == *Xs.rbegin() and c == *Ys.rbegin())) {
          diagonals[0].push_back(c);
          Xs.erase(c);
          Ys.erase(c);
          break;
        } else if((c == *Xs.begin() and c == *Ys.rbegin()) or (c == *Xs.rbegin() and c == *Ys.begin())) {
          diagonals[1].push_back(c);
          Xs.erase(c);
          Ys.erase(c);
          break;
        }
      }
    }
  }
  vector <pt> ret;
  for(int d=0; d<2; d++) sort(diagonals[d].begin(), diagonals[d].end());
  for(int d=0; d<2; d++) {
    if(!diagonals[d].empty()) {
      ret.emplace_back(ret.empty() ? 0 : ret.back().x, diagonals[d][0].y);
      ret.push_back(diagonals[d][0]);
      for(int i=0; i+1<(int)diagonals[d].size(); i++) {
        if(i&1) ret.emplace_back(diagonals[d][i].x, diagonals[d][i+1].y);
        else ret.emplace_back(diagonals[d][i+1].x, diagonals[d][i].y);
      }
      ret.push_back(diagonals[d].back());
    }
  }
  reverse(spiral.begin(), spiral.end());
  reverse(helper.begin(), helper.end());
  for(int i=0; i<(int)spiral.size(); i++) {
    if(i == 0){
      ret.emplace_back(ret.empty() ? 0 : ret.back().x, spiral[i][3].y);
      ret.push_back(spiral[i][3]);
    }
    else ret.back().y = spiral[i][0].y;
    if(i != 0 and ret.back().x <= helper[i].x) reverse(spiral[i].begin(), spiral[i].end());
    ret.insert(ret.end(), spiral[i].begin(), spiral[i].end());
  }
  ret.erase(unique(ret.begin(), ret.end()), ret.end());
  cout << (int)ret.size() << "\n";
  for(int i=0; i<(int)ret.size(); i++) {
    cout << ret[i].x << " " << ret[i].y << "\n";
  }
}
