/**
 * Task: parks
 * Author: Kian Mirjalali
 * Solution: Performing different bad behaviors based on input hash
 *  Based on solution: full-chessboard-adj-kian.cpp
 *  Used for testing the correct behavior of checker.
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

inline bool cmp_point_x_then_y(const Point& a, const Point& b) {
	cmpret(a.X, b.X);
	return a.Y < b.Y;
}

inline bool cmp_x_then_y(int i, int j) {
	return cmp_point_x_then_y(vertex[i], vertex[j]);
}

inline bool cmp_y_then_x(int i, int j) {
	const Point& a = vertex[i];
	const Point& b = vertex[j];
	cmpret(a.Y, b.Y);
	return a.X < b.X;
}

inline int id(int x, int y) {
	forn(i, sz(vertex))
		if (vertex[i].X == x && vertex[i].Y == y)
			return i;
	return -1;
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

vector<Point> sorted_vertices;
inline bool is_direct(vector<Point> p) {
	sort(allOf(p), cmp_point_x_then_y);
	return p == sorted_vertices;
}
/*
inline bool is_direct(vector<Point> p) {
	bool b = is_direct0(p);
	if (b) {
		cerr << "direct";
		for (auto a : p)
			cerr << "  " << a.X << " " << a.Y;
		cerr << endl;
	}
	return b;
}
//*/
inline bool is_squares(vector<Point> p) {
	for (int i = sz(p)-1; i>=0; i--)
		p.push_back(p[i]+MOVES[RIGHT]);
	for (int i = sz(p)-1; i>=0; i--)
		p.push_back(p[i]+MOVES[UP]);
	sort(allOf(p), cmp_point_x_then_y);
	p.resize(unique(allOf(p))-p.begin());
	return p == sorted_vertices;
}
/*
inline bool is_squares(vector<Point> p) {
	bool b = is_squares0(p);
	if (b) {
		cerr << "squares";
		for (auto a : p)
			cerr << "  " << a.X << " " << a.Y;
		cerr << endl;
	}
	return b;
}
//*/

/////////////////////////


int construct_roads(VI x, VI y) {
	int n = x.size();
	vertex.resize(n);
	forn(i, n)
		vertex[i] = Point(x[i], y[i]);

	VI u, v, a, b;
	auto addEdgeV = [&u, &v, &a, &b] (int _u, int _v, int _a, int _b) {
		u.push_back(_u);
		v.push_back(_v);
		a.push_back(_a);
		b.push_back(_b);
	};
	auto addEdge = [addEdgeV, &u, &v, &a, &b] (int ux, int uy, int vx, int vy, int _a, int _b) {
		addEdgeV(id(ux, uy), id(vx, vy), _a, _b);
	};
	#define bye1 do { build(u, v, a, b); return 1; } while (false)

	sorted_vertices = vertex;
	sort(allOf(sorted_vertices), cmp_point_x_then_y);

	/********************* Diverging behaviors *********************/
	#define setBehavior(b) cerr << "set behavior: " << (#b) << endl

	const bool CORRECT = false;

	/*
	unused:
	is_direct({{2, 0}, {4, 2}}); //0
	is_direct({{2, 2}, {4, 0}}); //0
	is_direct({{2, 2}, {4, 0}, {4, 4}}); //0
	is_direct({{2, 0}, {6, 0}}); //0
	is_direct({{2, 2}, {4, 0}, {4, 4}, {6, 2}}); //0
	is_squares({{2, 0}, {2, 6}}); //0
	is_squares({{0, 0}, {0, 6}}); //0
	is_squares({{0, 999998}});
	is_squares({{999998, 0}});
	is_squares({{999998, 999998}});
	//*/

	if (is_direct({{2, 0}, {2, 4}})) {
		setBehavior(PV_output);
		if (CORRECT) {
		} else {
			printf("something\n");
		}
		// Let the default flow of code handle it.
	}
	if (is_direct({{2, 0}, {2, 2}, {2, 6}})) {
		setBehavior(ret_invalid);
		if (CORRECT) {
			// Let the default flow of code handle it.
		} else {
			build(u, v, a, b);
			return 2;
		}
	}
	if (is_direct({{2, 0}, {2, 2}, {2, 6}, {2, 8}})) {
		setBehavior(ret_impossible_with_call);
		if (CORRECT) {
			// Let the default flow of code handle it.
		} else {
			build(u, v, a, b);
			return 0;
		}
	}
	if (is_direct({{2, 0}, {2, 2}, {2, 4}, {2, 8}})) {
		setBehavior(ret_possible_without_call);
		if (CORRECT) {
			// Let the default flow of code handle it.
		} else {
			return 1;
		}
	}
	if (is_direct({{2, 0}, {2, 2}})) {
		setBehavior(multiple_calls);
		addEdge(2,0, 2,2, 1,1);
		if (CORRECT) {
		} else {
			build(u, v, a, b);
		}
		bye1;
	}
	if (is_direct({{4, 0}, {4, 2}})) {
		setBehavior(invalid_len_v);
		if (CORRECT) {
			// Let the default flow of code handle it.
		} else {
			addEdge(4,0, 4,2, 3,1);
			v.push_back(0);
			bye1;
		}
	}
	if (is_direct({{2, 0}, {4, 0}})) {
		setBehavior(invalid_len_a);
		if (CORRECT) {
			// Let the default flow of code handle it.
		} else {
			addEdge(2,0, 4,0, 3,1);
			a.push_back(0);
			bye1;
		}
	}
	if (is_direct({{2, 2}, {4, 0}, {4, 2}})) {
		setBehavior(invalid_len_b);
		if (CORRECT) {
			// Let the default flow of code handle it.
		} else {
			addEdge(2,2, 4,2, 3,3);
			b.push_back(0);
			bye1;
		}
	}
	if (is_direct({{2, 0}, {4, 0}, {4, 2}})) {
		setBehavior(too_large_m);
		if (CORRECT) {
			// Let the default flow of code handle it.
		} else {
			int m = 2*n+1;
			u.assign(m, 0);
			v.assign(m, 0);
			a.assign(m, 0);
			b.assign(m, 0);
			bye1;
		}
	}
	if (is_direct({{2, 2}, {4, 2}})) {
		setBehavior(wrong_impossible);
		if (CORRECT) {
			addEdge(2,2, 4,2, 3,1);
			bye1;
		} else {
			return 0;
		}
	}
	if (is_direct({{2, 0}})) {
		setBehavior(loop_edge);
		if (CORRECT) {
		} else {
			addEdge(2,0, 2,0, 0,0);
		}
		bye1;
	}
	if (is_direct({{2, 0}, {2, 2}, {2, 4}})) {
		setBehavior(long_vertic_edge);
		addEdge(2,0, 2,2, 1,1);
		if (CORRECT) {
			addEdge(2,2, 2,4, 1,3);
		} else {
			addEdge(2,0, 2,4, 3,2);
		}
		bye1;
	}
	if (is_direct({{2, 0}, {4, 0}, {6, 0}})) {
		setBehavior(long_horiz_edge);
		addEdge(2,0, 4,0, 3,1);
		if (CORRECT) {
			addEdge(4,0, 6,0, 5,1);
		} else {
			addEdge(2,0, 6,0, 4,1);
		}
		bye1;
	}
	if (is_direct({{2, 0}, {2, 2}, {4, 2}})) {
		setBehavior(diag_edge);
		addEdge(2,0, 2,2, 1,1);
		if (CORRECT) {
			addEdge(2,2, 4,2, 3,3);
		} else {
			addEdge(2,0, 4,2, 3,1);
		}
		bye1;
	}
	if (is_direct({{2, 0}, {2, 2}, {4, 0}})) {
		setBehavior(diag2_edge);
		addEdge(2,0, 2,2, 1,1);
		if (CORRECT) {
			addEdge(2,0, 4,0, 3,1);
		} else {
			addEdge(2,2, 4,0, 3,1);
		}
		bye1;
	}
	if (is_squares({{2, 0}})) {
		setBehavior(vertic_edge_a_even);
		addEdge(2,0, 2,2, 1,1);
		addEdge(2,0, 4,0, 3,1);
		if (CORRECT) {
			addEdge(4,0, 4,2, 5,1);
		} else {
			addEdge(4,0, 4,2, 4,1);
		}
		bye1;
	}
	if (is_squares({{4, 0}})) {
		setBehavior(vertic_edge_a_pos2);
		addEdge(4,0, 4,2, 3,1);
		addEdge(4,0, 6,0, 5,1);
		if (CORRECT) {
			addEdge(6,0, 6,2, 7,1);
		} else {
			addEdge(6,0, 6,2, 9,1);
		}
		bye1;
	}
	if (is_squares({{4, 2}})) {
		setBehavior(vertic_edge_a_neg2);
		addEdge(4,2, 6,2, 5,1);
		addEdge(6,2, 6,4, 7,3);
		if (CORRECT) {
			addEdge(4,2, 4,4, 3,3);
		} else {
			addEdge(4,2, 4,4, 1,3);
		}
		bye1;
	}
	if (is_squares({{2, 2}})) {
		setBehavior(vertic_edge_b_even);
		addEdge(2,2, 2,4, 1,3);
		addEdge(2,2, 4,2, 3,1);
		if (CORRECT) {
			addEdge(4,2, 4,4, 5,3);
		} else {
			addEdge(4,2, 4,4, 5,2);
		}
		bye1;
	}
	if (is_squares({{2, 0}, {4, 4}})) {
		setBehavior(vertic_edge_b_pos);
		addEdge(2,0, 2,2, 1,1);
		addEdge(2,2, 4,2, 3,1);
		addEdge(4,2, 4,0, 5,1);
		addEdge(4,4, 6,4, 5,3);
		addEdge(6,4, 6,6, 7,5);
		addEdge(6,6, 4,6, 5,7);
		if (CORRECT) {
			addEdge(4,2, 4,4, 3,3);
		} else {
			addEdge(4,2, 4,4, 3,5);
		}
		bye1;
	}
	if (is_squares({{2, 4}, {4, 0}})) {
		setBehavior(vertic_edge_b_neg);
		addEdge(2,4, 2,6, 1,5);
		addEdge(2,4, 4,4, 3,5);
		addEdge(4,4, 4,6, 5,5);
		addEdge(4,2, 4,0, 5,1);
		addEdge(4,2, 6,2, 5,3);
		addEdge(6,2, 6,0, 7,1);
		if (CORRECT) {
			addEdge(4,4, 4,2, 3,3);
		} else {
			addEdge(4,4, 4,2, 3,1);
		}
		bye1;
	}
	if (is_squares({{2, 0}, {2, 2}})) {
		setBehavior(horiz_edge_b_even);
		addEdge(2,0, 2,2, 1,1);
		addEdge(2,2, 2,4, 1,3);
		addEdge(4,0, 4,2, 5,1);
		addEdge(4,2, 4,4, 5,3);
		if (CORRECT) {
			addEdge(2,4, 4,4, 3,5);
		} else {
			addEdge(2,4, 4,4, 3,4);
		}
		bye1;
	}
	if (is_squares({{2, 0}, {4, 0}})) {
		setBehavior(horiz_edge_b_pos2);
		addEdge(2,0, 2,2, 1,1);
		addEdge(2,0, 4,0, 3,1);
		addEdge(4,0, 4,2, 5,1);
		addEdge(6,2, 6,0, 7,1);
		if (CORRECT) {
			addEdge(4,2, 6,2, 5,3);
		} else {
			addEdge(4,2, 6,2, 5,5);
		}
		bye1;
	}
	if (is_squares({{2, 0}, {4, 2}})) {
		setBehavior(horiz_edge_b_neg2);
		addEdge(2,0, 2,2, 1,1);
		addEdge(2,0, 4,0, 3,-1);
		addEdge(2,2, 4,2, 3,1);
		addEdge(4,2, 4,4, 3,3);
		addEdge(6,2, 6,4, 7,3);
		if (CORRECT) {
			addEdge(4,4, 6,4, 5,3);
		} else {
			addEdge(4,4, 6,4, 5,1);
		}
		bye1;
	}
	if (is_squares({{2, 0}, {2, 4}})) {
		setBehavior(horiz_edge_a_even);
		addEdge(2,0, 2,2, 1,1);
		addEdge(2,2, 2,4, 1,3);
		addEdge(2,4, 2,6, 1,5);
		addEdge(4,0, 4,2, 5,1);
		addEdge(4,2, 4,4, 5,3);
		addEdge(4,4, 4,6, 5,5);
		if (CORRECT) {
			addEdge(2,6, 4,6, 3,7);
		} else {
			addEdge(2,6, 4,6, 4,7);
		}
		bye1;
	}
	if (is_squares({{2, 2}, {4, 0}})) {
		setBehavior(horiz_edge_a_pos);
		addEdge(2,2, 4,2, 3,1);
		addEdge(2,2, 2,4, 1,3);
		addEdge(2,4, 4,4, 3,5);
		addEdge(4,0, 4,2, 5,1);
		addEdge(6,2, 6,0, 7,1);
		if (CORRECT) {
			addEdge(4,2, 6,2, 5,3);
		} else {
			addEdge(4,2, 6,2, 7,3);
		}
		bye1;
	}
	if (is_squares({{0, 0}, {0, 2}})) {
		setBehavior(horiz_edge_a_neg);
		addEdge(0,0, 0,2, 1,1);
		addEdge(0,2, 0,4, 1,3);
		addEdge(2,0, 2,2, 3,1);
		addEdge(2,2, 2,4, 3,3);
		if (CORRECT) {
			addEdge(0,4, 2,4, 1,5);
		} else {
			addEdge(0,4, 2,4, -1,5);
		}
		bye1;
	}
	if (is_squares({{0, 2}})) {
		setBehavior(duplicate_edge);
		addEdge(0,2, 2,2, 1,1);
		addEdge(0,2, 0,4, -1,3);
		addEdge(2,2, 2,4, 3,3);
		if (CORRECT) {
		} else {
			addEdge(0,2, 2,2, 1,3);
		}
		bye1;
	}
	if (is_squares({{0, 0}})) {
		setBehavior(duplicate_tree);
		addEdge(0,0, 0,2, -1,1);
		addEdge(0,0, 2,0, 1,-1);
		addEdge(0,2, 2,2, 1,1);
		if (CORRECT) {
			addEdge(2,0, 2,2, 3,1);
		} else {
			addEdge(2,0, 2,2, 1,1);
		}
		bye1;
	}
	if (is_direct({{2, 0}, {2, 2}, {2, 4}, {2, 6}})) {
		setBehavior(disconnected);
		addEdge(2,0, 2,2, 3,1);
		addEdge(2,4, 2,6, 3,5);
		if (CORRECT) {
			addEdge(2,2, 2,4, 3,3);
		} else {
		}
		bye1;
	}
	if (is_squares({{0, 0}, {0, 4}})) {
		setBehavior(disconnected);
		addEdge(0,0, 0,2, -1,1);
		addEdge(0,0, 2,0, 1,-1);
		addEdge(0,2, 2,2, 1,1);
		addEdge(2,0, 2,2, 3,1);
		addEdge(0,4, 0,6, -1,5);
		addEdge(0,4, 2,4, 1,5);
		addEdge(0,6, 2,6, 1,7);
		addEdge(2,4, 2,6, 3,5);
		if (CORRECT) {
			addEdge(0,2, 0,4, 1,3);
		} else {
		}
		bye1;
	}

	/***************************************************************/

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
