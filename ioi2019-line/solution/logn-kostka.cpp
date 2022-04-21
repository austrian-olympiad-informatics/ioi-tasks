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
    if(lisy.size() * lisy.size() >= Y.size()) {
      get_chain();
    } else {
      reverse(Y.begin(), Y.end());
      lisy = lis(Y);
      reverse(test.begin(), test.end());
      get_chain();
      reverse(chains.back().begin(), chains.back().end());
      reverse(test.begin(), test.end());
    }
  }
  return chains;
}

vector <pt> logarithmic(vector <pt> test) {
  enum dir {LEFT, RIGHT};
  enum pos {VERTICAL, HORIZONTAL};
  auto change_dir = [](dir x) -> dir {
    if(x == LEFT) return RIGHT;
    else return LEFT;
  };
  auto change_pos = [](pos x) -> pos {
    if(x == VERTICAL) return HORIZONTAL;
    else return VERTICAL;
  };
  vector <pt> ret, ordering;
  ret.emplace_back(0, 0);
  while(!test.empty()) {
    set <pt> left(test.begin(), test.end());
    set <pt> XY;
    set <pt, cmpyx> YX;
    vector <pt> F;
    map <pt, int> chain_no;
    auto chains = get_chains(test);
    for(int i=0; i<(int)chains.size(); i++) {
      for(auto c : chains[i]) chain_no[c] = i;
    }
    auto remove_outer_points = [&](pt w1 = pt(-1, -1), pt w2 = pt(-1, -1)) {
      while(!XY.empty()) {
        if(*XY.begin() != *YX.begin()) break;
        auto e = *XY.begin();
        auto f = e;
        f.y = abs(f.y);
        if(e == w1 or e == w2) break;
        chains[chain_no[f]].pop_front();
        F.push_back(e);
        XY.erase(e);
        YX.erase(e);
      }
      while(!XY.empty()) {
        if(*XY.rbegin() != *YX.rbegin()) break;
        auto e = *XY.rbegin();
        auto f = e;
        f.y = abs(f.y);
        if(e == w1 or e == w2) break;
        chains[chain_no[f]].pop_back();
        F.push_back(e);
        XY.erase(e);
        YX.erase(e);
      }
    };
    auto is_chain = [](deque <pt> chain) {
      if(chain.size() <= 1) return true;
      if(chain[0].y < chain[1].y) return true;
      return false;
    };
    dir curdir = RIGHT;
    pos curpos = VERTICAL, oldpos = VERTICAL;
    function<bool(pt, pt)> north = [](pt a, pt b) -> bool {
      return a.y < b.y;
    };
    function<bool(pt, pt)> east = [](pt a, pt b) -> bool {
      return a.x < b.x;
    };
    function<bool(pt, pt)> south = [](pt a, pt b) -> bool {
      return a.y > b.y;
    };
    function<bool(pt, pt)> west = [](pt a, pt b) -> bool {
      return a.x > b.x;
    };
    auto get_elem = [&](deque <pt>& Q, dir d) -> pt{
      if(d == RIGHT) return Q.back();
      else return Q.front();
    };
    auto pop_elem = [&](deque <pt>& Q, dir d) {
      if(d == RIGHT) Q.pop_back();
      else Q.pop_front();
    };
    auto push_elem = [&](deque <pt>& Q, pt elem, dir d) {
      if(d == RIGHT) Q.push_back(elem);
      else Q.push_front(elem);
    };
    vector <int> upp_chains, low_chains;
    for(int i=0; i<(int)chains.size(); i++) {
      if(is_chain(chains[i])) upp_chains.push_back(i);
      else low_chains.push_back(i);
    }
    vector <vector <int> > cur_chains = {upp_chains, low_chains};
    vector <int> cur_chain;
    auto do_step = [&](int i, function<bool(pt, pt)> dir1, function<bool(pt, pt)> dir2, bool inv) -> int {
      vector <int> N, E;
      int non_empty = 0;
      for(int j=0; j<(int)cur_chain.size(); j++) {
        if(i == j) continue;
        while(!chains[cur_chain[j]].empty()
            and dir1(get_elem(chains[cur_chain[i]], curdir), get_elem(chains[cur_chain[j]], curdir))
            and dir2(get_elem(chains[cur_chain[i]], curdir), get_elem(chains[cur_chain[j]], curdir))) {
          chain_no[get_elem(chains[cur_chain[j]], curdir)] = cur_chain[i];
          push_elem(chains[cur_chain[i]], get_elem(chains[cur_chain[j]], curdir), curdir);
          pop_elem(chains[cur_chain[j]], curdir);
        }
        if(!chains[cur_chain[j]].empty()) {
          non_empty++;
          if(!dir1(get_elem(chains[cur_chain[i]], curdir), get_elem(chains[cur_chain[j]], curdir))
              and dir2(get_elem(chains[cur_chain[i]], curdir), get_elem(chains[cur_chain[j]], curdir))) {
            E.push_back(j);
          }
          if(dir1(get_elem(chains[cur_chain[i]], curdir), get_elem(chains[cur_chain[j]], curdir))
              and !dir2(get_elem(chains[cur_chain[i]], curdir), get_elem(chains[cur_chain[j]], curdir))) {
            N.push_back(j);
          }
        }
      }
      assert(!non_empty or (!N.empty() or !E.empty()));
      pos nextpos = curpos;
      if(non_empty) {
        // cerr << "N" << non_empty << "\n";
        int no = (int)chains[cur_chain[i]].size();
        if(no % 2) nextpos = change_pos(nextpos);
        if((nextpos == VERTICAL and N.empty()) or (nextpos == HORIZONTAL and E.empty())) {
          nextpos = change_pos(nextpos);
          pt e = get_elem(chains[cur_chain[i]], curdir);
          // cerr << "REMOVE" << e.x << " " << e.y << "\n";
          pop_elem(chains[cur_chain[i]], curdir);
          XY.erase(e);
          YX.erase(e);
          if(inv) e.y *= -1;
          // test.push_back(e);
        }
      }
      vector <pt> O;
      // cerr << "P: ";
      for(auto& p : chains[cur_chain[i]]) {
        XY.erase(p);
        YX.erase(p);
        O.push_back(p);
        // cerr << p.x << " " << p.y << ", ";
      }
      // cerr << "\n";
      if(curdir == LEFT) reverse(O.begin(), O.end());
      int new_i = -1;
      if(non_empty) {
        if(nextpos == VERTICAL) new_i = N[0];
        else new_i = E[0];
      }
      curpos = nextpos;
      if(!chains[cur_chain[i]].empty()) {
        if(new_i != -1) {
          remove_outer_points(get_elem(chains[cur_chain[i]], curdir),
              get_elem(chains[cur_chain[new_i]], curdir));
        } else {
          remove_outer_points(get_elem(chains[cur_chain[i]], curdir));
        }
      } else {
        remove_outer_points();
      }
      if(!O.empty()) {
        if(inv) for(auto& o : O) o.y *= -1;
        ordering.insert(ordering.end(), O.begin(), O.end());
        chains[cur_chain[i]].clear();
      } else {
        curdir = change_dir(curdir);
      }
      return new_i;
    };
    for(int cc=0; cc<2; cc++) {
      curpos = oldpos = VERTICAL;
      cur_chain = cur_chains[cc];
      if(cc == 1) for(int i=0; i<(int)cur_chain.size(); i++) {
        for(auto& p : chains[cur_chain[i]]) p.y *= -1;
      }
      for(int i=0; i<(int)cur_chain.size(); i++) {
        for(auto p : chains[cur_chain[i]]) {
          XY.insert(p);
          YX.insert(p);
        }
      }
      int i = 0;
      remove_outer_points();
      ordering.clear();
      while(i < (int)cur_chain.size() and chains[cur_chain[i]].empty()) i++;
      if(i != (int)cur_chain.size()) {
        while(true) {
          if(chains[cur_chain[i]].empty()) break;
          int next_i;
          if(curdir == RIGHT) next_i = do_step(i, north, east, cc==1);
          else next_i = do_step(i, south, west, cc==1);
          curdir = change_dir(curdir);
          if(next_i == -1) break;
          i = next_i;
        }
        if(!ordering.empty()) {
          if(ret.back() != pt(0, 0)) ret.emplace_back(ret.back().x, 0);
          ret.emplace_back(0, 0);
          ret.emplace_back(ret.back().x, ordering[0].y);
          ret.emplace_back(ordering[0]);
          for(int i=0; i+1<(int)ordering.size(); i++) {
            if(oldpos == HORIZONTAL) ret.emplace_back(ordering[i].x, ordering[i+1].y);
            else ret.emplace_back(ordering[i+1].x, ordering[i].y);
            oldpos = change_pos(oldpos);
          }
          ret.push_back(ordering.back());
        }
      }
      if(!F.empty()) {
        sort(F.begin(), F.end());
        if(cc == 1) for(auto& f : F) f.y *= -1;
        if(ret.back() != pt(0, 0)) ret.emplace_back(ret.back().x, 0);
        ret.emplace_back(0, 0);
        ret.emplace_back(ret.back().x, F[0].y);
        ret.emplace_back(F[0]);
        for(int i=0; i+1<(int)F.size(); i++) {
          if(oldpos == HORIZONTAL) ret.emplace_back(F[i].x, F[i+1].y);
          else ret.emplace_back(F[i+1].x, F[i].y);
          oldpos = change_pos(oldpos);
        }
        ret.push_back(F.back());
        F.clear();
      }
      vector <pt> cXY(left.begin(), left.end()), cYX(left.begin(), left.end());
      sort(cXY.begin(), cXY.end());
      sort(cYX.begin(), cYX.end(), cmpyx());
      pt cur = {0, 0};
      for(int i=0; i<(int)ret.size(); i++) {
        if(ret[i].x == cur.x) {
          pt mini = min(cur, ret[i]), maxi = max(cur, ret[i]);
          for(auto it = lower_bound(cXY.begin(), cXY.end(), mini);
              it != upper_bound(cXY.begin(), cXY.end(), maxi); it++) {
            left.erase(*it);
          }
        } else {
          pt mini = min(cur, ret[i], cmpyx()), maxi = max(cur, ret[i], cmpyx());
          for(auto it = lower_bound(cYX.begin(), cYX.end(), mini, cmpyx());
              it != upper_bound(cYX.begin(), cYX.end(), maxi, cmpyx()); it++) {
            left.erase(*it);
          }
        }
        cur = ret[i];
      }
    }
    test = vector<pt>(left.begin(), left.end());
  }
  ret.erase(unique(ret.begin(), ret.end()), ret.end());
  ret.erase(ret.begin());
  return ret;
}

int main() {
  int n;
  cin >> n;
  vector <pt> test(n);
  for(int i=0; i<n; i++) cin >> test[i].x >> test[i].y;
  auto log_sol = logarithmic(test);
  cout << (int)log_sol.size() << "\n";
  for(auto s : log_sol) cout << s.x << " " << s.y << "\n";
}
