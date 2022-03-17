/**
 * Task: parks
 * Author: Kian Mirjalali
 * Solution: Solution based on chessboard coloring of cells (trees)
 *  Time: O(n*log(n)+n) where n*log(n) is for just 2 sorts
 *  It computes and keeps the adjacent vertices for each vertex on each direction.
 */
#include "parks.h"
#include <complex>
#include <algorithm>
#include <iostream>
using namespace std;


#define tpc(C) template<class C>
#define allOf(c) ((c).begin()), ((c).end())
#define fori(i, a, b) for (int i = (a); i < int(b); i++)
#define forn(i, n) fori(i, 0, (n))
#define dbg(x) #x << "=" << (x)
#define show(x) cerr << dbg(x) << endl
#define cmpret(a, b) do { if ((a) != (b)) return ((a) < (b)); } while (false)

tpc(C) inline int sz(const C& c) { return c.size(); }

typedef vector<int> VI;

typedef complex<int> Point;
#define X real()
#define Y imag()

/////////////////////////

const vector<Point> MOVES = {
	{-2, 0},
	{+2, 0},
	{0, -2},
	{0, +2},
};
const int LEFT = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int UP = 3;

/////////////////////////

vector<Point> vertex;

inline bool cmp_x_then_y(int i, int j) {
	const Point& a = vertex[i];
	const Point& b = vertex[j];
	cmpret(a.X, b.X);
	return a.Y < b.Y;
}

inline bool cmp_y_then_x(int i, int j) {
	const Point& a = vertex[i];
	const Point& b = vertex[j];
	cmpret(a.Y, b.Y);
	return a.X < b.X;
}

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
	vertex.resize(n);
	forn(i, n)
		vertex[i] = Point(x[i], y[i]);

	vector<VI> adj(n, VI(sz(MOVES), -1));
	{// Fill adj
		VI index(n);
		forn(i, n)
			index[i] = i;
		sort(allOf(index), cmp_x_then_y);
		forn(t, n-1) {
			int i = index[t];
			int j = index[t+1];
			if (vertex[i]+MOVES[UP] == vertex[j]) {
				adj[i][UP] = j;
				adj[j][DOWN] = i;
			}
		}
		sort(allOf(index), cmp_y_then_x);
		forn(t, n-1) {
			int i = index[t];
			int j = index[t+1];
			if (vertex[i]+MOVES[RIGHT] == vertex[j]) {
				adj[i][RIGHT] = j;
				adj[j][LEFT] = i;
			}
		}
	}

	{// Check connectivity
		q.clear();
		q.reserve(n);
		mark.assign(n, false);
		enq(0);
		for (int tail = 0; tail < sz(q); tail++)
			for (int nei : adj[q[tail]])
				if (nei >= 0)
					enq(nei);
		if (sz(q) != n)
			return 0;
	}

	VI u, v, a, b;
	forn(i, n) {
		Point pi = vertex[i];
		bool is_white_cell = (((pi.X+pi.Y)&2) == 0);
		{
			int j = adj[i][RIGHT];
			if ((j >= 0) && !((adj[i][UP] >= 0) && (adj[j][UP] >= 0) && is_white_cell)) {
				u.push_back(i);
				v.push_back(j);
				a.push_back(pi.X+1);
				b.push_back(pi.Y+(is_white_cell?+1:-1));
			}
		}
		{
			int j = adj[i][UP];
			if ((j >= 0) && !((adj[i][LEFT] >= 0) && (adj[j][LEFT] >= 0) && is_white_cell)) {
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
