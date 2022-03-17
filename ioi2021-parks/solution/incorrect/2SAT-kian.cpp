/**
 * Task: parks
 * Author: Kian Mirjalali
 * Solution: Create a 2SAT with variables for roads and clauses for conflicting roads (on a common tree)
 *  Works correctly if there is no solution, or a solution with all the roads.
 *  It uses a tree map to find the index of a vertex by its coordinate.
 *  This includes cases where this no square, or when the points are in at most two columns (x values).
 *  Time: O(n*log(n))
 */
#include "parks.h"
#include <complex>
#include <map>
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
typedef pair<int, int> PII;

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

struct PointCmp {
	inline bool operator()(const Point &a, const Point &b) const {
		cmpret(a.X, b.X);
		return a.Y < b.Y;
	}
};

class PointIndex {
	vector<Point> points;
	map<Point, int, PointCmp> p2i;
public:
	inline PointIndex() {
	}

	inline int add(const Point& p) {
		int i = getIndex(p);
		if (i < 0) {
			i = sz(points);
			points.push_back(p);
			p2i[p] = i;
		}
		return i;
	}

	inline int add(int x, int y) {
		return add(Point(x, y));
	}

	inline int size() const {
		return sz(points);
	}

	inline Point getPoint(int i) const {
		return points[i];
	}

	inline int getIndex(const Point& p) const {
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

class TwoSATSolver {
	int n_vars, n_vert;
	vector<VI> adj_f, adj_b;

	inline int dual(int x) const {
		return x^1;
	}

	inline int external_literal_to_internal(bool negation, int xi) const {
		return 2*xi + (negation?1:0);
	}

	inline void add_edge(int from, int to) {
		adj_f[from].push_back(to);
		adj_b[to].push_back(from);
	}

	vector<PII> dfs_stack;
	vector<bool> mark;
	VI order, comp;

	inline void dfs1_push(int x) {
		dfs_stack.emplace_back(x, 0);
		mark[x] = true;
	}
	inline void dfs1(int root) {
		dfs_stack.clear();
		dfs1_push(root);
		while (!dfs_stack.empty()) {
			int x = dfs_stack.back().first;
			int& i = dfs_stack.back().second;
			auto& x_adj = adj_f[x];
			while ((i < sz(x_adj)) && mark[x_adj[i]])
				i++;
			if (i < sz(x_adj))
				dfs1_push(x_adj[i]);
			else {
				order.push_back(x);
				dfs_stack.pop_back();
			}
		}
	}

	inline void dfs2_push(int x, int cl) {
		dfs_stack.emplace_back(x, 0);
		comp[x] = cl;
	}
	inline void dfs2(int root, int cl) {
		dfs_stack.clear();
		dfs2_push(root, cl);
		while (!dfs_stack.empty()) {
			int x = dfs_stack.back().first;
			int& i = dfs_stack.back().second;
			auto& x_adj = adj_b[x];
			while ((i < sz(x_adj)) && (comp[x_adj[i]] >= 0))
				i++;
			if (i < sz(x_adj))
				dfs2_push(x_adj[i], cl);
			else
				dfs_stack.pop_back();
		}
	}

public:
	inline TwoSATSolver(int n):
		n_vars(n), n_vert(2*n),
		adj_f(n_vert), adj_b(n_vert) {
	}

	inline void add(bool xi_negation, int xi, bool yi_negation, int yi) {
		int x = external_literal_to_internal(xi_negation, xi);
		int y = external_literal_to_internal(yi_negation, yi);
		add_edge(dual(x), y);
		add_edge(dual(y), x);
	}

	inline bool solve(vector<bool>& assignment) {
		order.clear();
		mark.assign(n_vert, false);
		forn(x, n_vert)
			if (!mark[x])
				dfs1(x);

		comp.assign(n_vert, -1);
		for (int i = n_vert-1, j = 0; i >= 0; i--) {
			int x = order[i];
			if (comp[x] == -1)
				dfs2(x, j++);
		}

		vector<bool> assignment0(n_vars);
		forn(i, n_vars) {
			int x = external_literal_to_internal(false, i);
			if (comp[x] == comp[dual(x)])
				return false;
			assignment0[i] = (comp[x] > comp[dual(x)]);
		}
		assignment = assignment0;
		return true;
	}
};

/////////////////////////

tpc(T) inline int find_index(const vector<T>& vec, const T& val) {
	return find(allOf(vec), val)-vec.begin();
}

vector<PII> roads;
PointIndex treeIndex;
vector<VI> road_adj, tree_adj;

inline void add_road_tree_edge(int road_id, int tree_id) {
	road_adj[road_id].push_back(tree_id);
	tree_adj[tree_id].push_back(road_id);
};

/////////////////////////

int construct_roads(VI x, VI y) {
	int n = x.size();
	PointIndex vertexIndex;
	forn(i, n)
		vertexIndex.add(x[i], y[i]);

	{// Check connectivity
		q.clear();
		q.reserve(n);
		mark.assign(n, false);
		enq(0);
		for (int tail = 0; tail < sz(q); tail++) {
			Point p = vertexIndex.getPoint(q[tail]);
			for (auto& mov : MOVES) {
				int nei = vertexIndex.getIndex(p+mov);
				if (nei >= 0)
					enq(nei);
			}
		}
		if (sz(q) != n)
			return 0;
	}

	// Building the road-tree structure
	road_adj.resize(2*n, VI());
	tree_adj.resize(4*n, VI());
	forn(i, n) {
		Point pi = vertexIndex.getPoint(i);
		{
			Point pj = pi + MOVES[RIGHT];
			int j = vertexIndex.getIndex(pj);
			if (j >= 0) {
				int road_id = sz(roads);
				roads.emplace_back(i, j);
				add_road_tree_edge(road_id, treeIndex.add(pi.X+1, pi.Y+1));
				add_road_tree_edge(road_id, treeIndex.add(pi.X+1, pi.Y-1));
			}
		}
		{
			Point pj = pi + MOVES[UP];
			int j = vertexIndex.getIndex(pj);
			if (j >= 0) {
				int road_id = sz(roads);
				roads.emplace_back(i, j);
				add_road_tree_edge(road_id, treeIndex.add(pi.X+1, pi.Y+1));
				add_road_tree_edge(road_id, treeIndex.add(pi.X-1, pi.Y+1));
			}
		}
	}
	int n_trees = treeIndex.size();
	int n_roads = roads.size();

	// Building the 2SAT structure
	TwoSATSolver two_sat(n_roads);
	forn(tree_id, n_trees) {
		auto& tr_adj = tree_adj[tree_id];
		forn(j, sz(tr_adj)) {
			int rj_id = tr_adj[j];
			bool rj_neg = (find_index(road_adj[rj_id], tree_id) == 1);
			forn(i, j) {
				int ri_id = tr_adj[i];
				bool ri_neg = (find_index(road_adj[ri_id], tree_id) == 1);
				two_sat.add(ri_neg, ri_id, rj_neg, rj_id);
			}
		}
	}

	// Solve and use the 2SAT
	vector<bool> assignment;
	if (!two_sat.solve(assignment))
		return 0;
	VI u, v, a, b;
	forn(road_id, n_roads) {
		PII road = roads[road_id];
		int tree_id = road_adj[road_id][assignment[road_id]?1:0];
		Point tree_pt = treeIndex.getPoint(tree_id);
		u.push_back(road.first);
		v.push_back(road.second);
		a.push_back(tree_pt.X);
		b.push_back(tree_pt.Y);
	}
	build(u, v, a, b);
	return 1;
}
