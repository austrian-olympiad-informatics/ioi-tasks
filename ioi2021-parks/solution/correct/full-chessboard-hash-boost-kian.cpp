/**
 * Task: parks
 * Author: Kian Mirjalali
 * Solution: Correct solution based on chessboard coloring of cells (trees)
 *  Time: O(n) on average
 *  It uses a hash map to find the index of a vertex by its coordinate.
 *  The hash function is boost_hash_combine(hash(x), hash(y)) based on boost library.
 */
#include "parks.h"
#include <complex>
#include <unordered_map>
using namespace std;


#define tpc(C) template<class C>
#define allOf(c) ((c).begin()), ((c).end())
#define fori(i, a, b) for (int i = (a); i < int(b); i++)
#define forn(i, n) fori(i, 0, (n))
#define dbg(x) #x << "=" << (x)
#define show(x) cerr << dbg(x) << endl

tpc(C) inline int sz(const C& c) { return c.size(); }

typedef vector<int> VI;

typedef complex<int> Point;
#define X real()
#define Y imag()

/////////////////////////

const Point MOVES[] = {
	{-2, 0},
	{+2, 0},
	{0, -2},
	{0, +2},
};
const int LEFT = 0;
const int RIGHT = 1;
const int UP = 3;

/////////////////////////

inline size_t boost_hash_combine(size_t a, size_t b) {
	return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

struct PointHash {
	inline size_t operator()(const Point &a) const {
		auto h = hash<int>();
		return boost_hash_combine(h(a.X), h(a.Y));
	}
};

class Point2Index {
	unordered_map<Point, int, PointHash> p2i;
public:
	inline Point2Index() {
	}

	inline void put(const Point& p, int i) {
		p2i[p] = i;
	}

	inline int get(const Point& p) const {
		auto it = p2i.find(p);
		return (it == p2i.end()) ? -1 : it->second;
	}

	inline bool contains(const Point& p) const {
		return p2i.find(p) != p2i.end();
	}
};

/////////////////////////

VI q, mark;

inline void enq(int x) {
	if (mark[x])
		return;
	mark[x] = true;
	q.push_back(x);
}

/////////////////////////


int construct_roads(VI x, VI y) {
	int n = x.size();
	vector<Point> vertex(n);
	Point2Index v2i;
	forn(i, n) {
		vertex[i] = Point(x[i], y[i]);
		v2i.put(vertex[i], i);
	}

	{// Check connectivity
		q.clear();
		q.reserve(n);
		mark.assign(n, false);
		enq(0);
		for (int tail = 0; tail < sz(q); tail++) {
			Point p = vertex[q[tail]];
			for (auto& mov : MOVES) {
				int nei = v2i.get(p+mov);
				if (nei >= 0)
					enq(nei);
			}
		}
		if (sz(q) != n)
			return 0;
	}

	VI u, v, a, b;
	forn(i, n) {
		Point pi = vertex[i];
		bool is_white_cell = (((pi.X+pi.Y)&2) == 0);
		{
			Point pj = pi + MOVES[RIGHT];
			int j = v2i.get(pj);
			if ((j >= 0) && !(v2i.contains(pi+MOVES[UP]) && v2i.contains(pj+MOVES[UP]) && is_white_cell)) {
				u.push_back(i);
				v.push_back(j);
				a.push_back(pi.X+1);
				b.push_back(pi.Y+(is_white_cell?+1:-1));
			}
		}
		{
			Point pj = pi + MOVES[UP];
			int j = v2i.get(pj);
			if ((j >= 0) && !(v2i.contains(pi+MOVES[LEFT]) && v2i.contains(pj+MOVES[LEFT]) && is_white_cell)) {
				u.push_back(i);
				v.push_back(j);
				a.push_back(pi.X+(is_white_cell?-1:+1));
				b.push_back(pi.Y+1);
			}
		}
	}
	build(u, v, a, b);
	return 1;
}
