/**
 * Task: parks
 * Author: Kian Mirjalali
 * Solution: Create a road-to-tree bipartite graph and find a road-perfect matching.
 *  Works correctly if there is no square.
 *  It also works correctly if the points are in at most two columns (x values).
 *  It uses a tree map to find the index of a vertex by its coordinate.
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

tpc(T) inline void removeValue(vector<T>& vec, const T& val) {
	vec.erase(remove(allOf(vec), val), vec.end());
}

vector<PII> roads;
PointIndex treeIndex;
vector<VI> road_adj, tree_adj;
vector<bool> road_removed, tree_mark;
VI leaf_trees;
inline void add_road_tree_edge(int road_id, int tree_id) {
	road_adj[road_id].push_back(tree_id);
	tree_adj[tree_id].push_back(road_id);
};
inline void check_leaf_tree(int tree_id) {
	if (tree_mark[tree_id] || sz(tree_adj[tree_id]) > 1)
		return;
	tree_mark[tree_id] = true;
	leaf_trees.push_back(tree_id);
}
inline void remove_road(int road_id) {
	road_removed[road_id] = true;
	for (int tree_id : road_adj[road_id]) {
		removeValue(tree_adj[tree_id], road_id);
		check_leaf_tree(tree_id);
	}
	road_adj[road_id].clear();
}
inline void remove_tree(int tree_id) {
	for (int road_id : tree_adj[tree_id])
		removeValue(road_adj[road_id], tree_id);
	tree_adj[tree_id].clear();
}

vector<PII> road_tree_matching;
inline void match(int road_id, int tree_id) {
	road_tree_matching.emplace_back(road_id, tree_id);
	remove_road(road_id);
	remove_tree(tree_id);
}


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

	//Building the bipartite graph
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

	//Build matching
	road_removed.resize(n_roads, false);
	tree_mark.resize(n_trees, false);
	forn(tree_id, n_trees)
		check_leaf_tree(tree_id);

	//* Removing a road from squares, in order to increase the chance of finding a valid solution
	forn(tree_id, n_trees)
		while (sz(tree_adj[tree_id]) > 3)
			remove_road(tree_adj[tree_id][3]);
	//*/

	for (int road_head = 0; road_head < n_roads; ) {
		while (!leaf_trees.empty()) {
			int leaf_tree = leaf_trees.back();
			leaf_trees.pop_back();
			if (tree_adj[leaf_tree].empty()) {
				remove_tree(leaf_tree);
				continue;
			}
			int road_id = tree_adj[leaf_tree][0];
			match(road_id, leaf_tree);
		}
		while (road_head < n_roads && road_removed[road_head])
			road_head++;
		if (road_head >= n_roads)
			continue;
		if (road_adj[road_head].empty()) {
			remove_road(road_head);
			continue;
		}
		match(road_head, road_adj[road_head][0]);
	}

	VI u, v, a, b;
	for (auto mp: road_tree_matching) {
		PII road = roads[mp.first];
		Point tree_pt = treeIndex.getPoint(mp.second);
		u.push_back(road.first);
		v.push_back(road.second);
		a.push_back(tree_pt.X);
		b.push_back(tree_pt.Y);
	}
	build(u, v, a, b);
	return 1;
}
