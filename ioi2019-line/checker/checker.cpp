#include "testlib.h"
#include <algorithm>
#include <map>
using namespace std;

#define range(C) ((C).begin()), ((C).end())

template<class A, class B>
inline bool maxEq(A& a, const B& b) {
  return (a<b) && (a=b, true);
}


typedef pair<int,int> pii;

inline pii spii(int i, int j) {
  return (i<j) ? pii(i, j) : pii(j, i);
}

typedef pii Point;
#define X first
#define Y second

inline Point readPoint(InStream& is) {
  Point p;
  p.X = is.readInt(-2e9, 2e9);
  is.readSpace();
  p.Y = is.readInt(-2e9, 2e9);
  is.readEoln();
  return p;
}


inline bool isCovered(int x, const vector<pii>& intervals) {
  if (intervals.empty())
    return false;
  auto it = upper_bound(range(intervals), pii(x, intervals.back().second+1));
  if (it==intervals.begin())
    return false;
  auto& inter = *(--it);
  return inter.first <= x && x <= inter.second;
}

inline bool isCovered(int x, int y, const map<int, vector<pii> > &xinter) {
  auto it = xinter.find(x);
  if (it==xinter.end())
    return false;
  return isCovered(y, it->second);
}

int main(int argc, char * argv[]) {
  registerChecker("line", argc, argv);
  int n = inf.readInt();
  vector<Point> p;
  for (int i=0; i<n; i++)
    p.push_back(readPoint(inf));
  int k = ouf.readInt();
  vector<Point> q;
  q.emplace_back(0,0);
  for (int i=0; i<k; i++)
    q.push_back(readPoint(ouf));
  ouf.readEof();
  map<int, vector<pii> > xinter, yinter;
  for (int i=1; i<=k; i++)
    if (q[i].X==q[i-1].X)
      xinter[q[i].X].push_back(spii(q[i].Y, q[i-1].Y));
    else if (q[i].Y==q[i-1].Y)
      yinter[q[i].Y].push_back(spii(q[i].X, q[i-1].X));
    else
      quitf(_wa, "line segment %i is neither horizontal nor vertical", i);
  for (auto intervalsPtr : {&xinter, &yinter})
    for (auto& intervalsKV : *intervalsPtr) {
      vector<pii>& intervals = intervalsKV.second;
      sort(range(intervals));
      vector<pii> U;
      for (auto& it : intervals)
        if (U.empty() || U.back().second<it.first)
          U.push_back(it);
        else
          maxEq(U.back().second, it.second);
      intervals.swap(U);
    }
  for (int i=0; i<n; i++)
    if (!isCovered(p[i].X, p[i].Y, xinter) && !isCovered(p[i].Y, p[i].X, yinter))
      quitf(_wa, "point %i (%i, %i) is not covered by the line", i, p[i].X, p[i].Y);

  map<int, vector <int>> grading;
  grading[4] = {8};
  grading[20] = {50, 45, 40, 37, 35, 33, 28, 26, 25, 23};
  grading[600] = {1200, 937, 674, 651, 640, 628, 616, 610, 607, 603};
  grading[5000] = {10000, 7607, 5213, 5125, 5081, 5037, 5020, 5012, 5008, 5003};
  grading[50000] = {100000, 75336, 50671, 50359, 50203, 50047, 50025, 50014, 50009, 50003};
  grading[72018] = {144036, 108430, 72824, 72446, 72257, 72067, 72044, 72033, 72027, 72021};
  grading[91891] = {183782, 138292, 92801, 92371, 92156, 91941, 91918, 91906, 91900, 91894};
  grading[100000] = {200000, 150475, 100949, 100500, 100275, 100050, 100027, 100015, 100009, 100003};
  if(k <= grading[n].back()) quitf(_ok, "segments used: %d", k);
  if(k > grading[n][0]) quitp(0.0, "segments used: %d", k);
  for(int i=0; i<9; i++) {
    if(grading[n][i] >= k and k >= grading[n][i+1]) {
      quitp(0.1 * (i+1) + 0.1 * (grading[n][i] - k)/(grading[n][i] - grading[n][i+1]), "segments used: %d", k);
    }
  }
  return 0;
}
