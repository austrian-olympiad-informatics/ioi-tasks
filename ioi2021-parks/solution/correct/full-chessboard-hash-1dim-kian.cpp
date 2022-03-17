/**
 * Task: parks
 * Author: Kian Mirjalali
 * Solution: Correct solution based on chessboard coloring of cells (trees)
 *  Time: O(n) on average
 *  It uses a hash map for each x to find the index of a vertex by its coordinate.
 */
#include "parks.h"
#include <iostream>
#include <algorithm>
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

class Point2Index {
	int _min_x, _max_x;
	typedef unordered_map<int, int> Y2i;
	vector<Y2i> p2i;

	inline void check_x_validity(int x) const {
		if (x < _min_x || _max_x < x) {
			cerr << "invalid x " << x << endl;
			exit(10);
		}
	}

	inline Y2i& get_y2i(int x) {
		check_x_validity(x);
		return p2i[x-_min_x];
	}

	inline const Y2i& get_y2i(int x) const {
		check_x_validity(x);
		return p2i[x-_min_x];
	}

public:
	inline Point2Index(int min_x, int max_x): _min_x(min_x), _max_x(max_x), p2i(max_x-min_x+1) {
	}

	inline void put(const Point& p, int i) {
		get_y2i(p.X)[p.Y] = i;
	}

	inline int get(const Point& p) const {
		auto& y2i = get_y2i(p.X);
		auto it = y2i.find(p.Y);
		return (it == y2i.end()) ? -1 : it->second;
	}

	inline bool contains(const Point& p) const {
		auto& y2i = get_y2i(p.X);
		return y2i.find(p.Y) != y2i.end();
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
	Point2Index v2i(*min_element(allOf(x))-2, *max_element(allOf(x))+2);
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
