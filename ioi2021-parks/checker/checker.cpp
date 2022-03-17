#include "testlib.h"
using namespace std;

const string output_secret = "3kr2yac8xnf3ktgcoqviaw115df6rra7is6p5uix"; //TODO: Change secret

typedef vector<int> VI;
typedef pair<int,int> PII;

typedef PII Point;
#define X first
#define Y second


struct DisjointSet {
	VI par;

	inline DisjointSet(int n): par(n) {
		for (int i = 0; i < n; i++)
			par[i] = i;
	}

	inline int findRoot(int x) {
		int i;
		for (i = x; par[i] != i; )
			i = par[i];
		int root = i;
		for (i = x; par[i] != i; ) {
			int j = par[i];
			par[i] = root;
			i = j;
		}
		return root;
	}

	/// returns true if the merge happened now or false if they are already merged
	inline bool merge(int x, int y) {
		int x_root = findRoot(x);
		int y_root = findRoot(y);
		if (x_root == y_root)
			return false;
		par[x_root] = y_root;
		return true;
	}
};


int main(int argc, char * argv[]) {
	registerChecker("parks", argc, argv);
	readBothSecrets(output_secret);
	readBothGraderResults();

	int possible = ouf.readInt(0, 1, "possible");
	int possible_ans = ans.readInt(0, 1, "possible_ans");

	if (possible == 0) {
		quitif(possible_ans > 0, _wa, "Solution announced impossible, but it is possible.");
		quit(_ok);
	}
	// possible > 0

	// Read from input file
	inf.readLine(); // Input secret
	int n = inf.readInt();
	vector<Point> fountain(n);
	for(int i=0; i<n; i++) {
		fountain[i].X = inf.readInt();
		fountain[i].Y = inf.readInt();
	}

	// Read from output file
	int m = ouf.readInt();
	VI u(m), v(m);
	vector<Point> tr(m);
	for (int j = 0; j < m; j++) {
		u[j] = ouf.readInt(0, n-1, "u[j]");
		v[j] = ouf.readInt(0, n-1, "v[j]");
		tr[j].X = ouf.readInt();
		tr[j].Y = ouf.readInt();
	}

	// Checking validity of edges and their adjacent trees
	for (int j = 0; j < m; j++) {
		int ux = fountain[u[j]].X;
		int uy = fountain[u[j]].Y;
		int vx = fountain[v[j]].X;
		int vy = fountain[v[j]].Y;
		long long dx = abs(ux-vx);
		long long dy = abs(uy-vy);
		quitif(dx*dx + dy*dy != 2*2,
				_wa, "Pair u[%d]=%d @(%d, %d) and v[%d]=%d @(%d, %d) does not form a valid edge (distance != 2)",
				j, u[j], ux, uy, j, v[j], vx, vy);
		quitif(tr[j].X % 2 == 0,
				_wa, "a[%d] = %d is not an odd integer", j, tr[j].X);
		quitif(tr[j].Y % 2 == 0,
				_wa, "b[%d] = %d is not an odd integer", j, tr[j].Y);
		quitif((dx == 0) ? (abs(tr[j].X-ux) != 1 || tr[j].Y != (uy+vy)/2) : (abs(tr[j].Y-uy) != 1 || tr[j].X != (ux+vx)/2),
				_wa, "Tree (a[%d], b[%d]) = (%d, %d) is not adjacent to edge between u[%d]=%d @(%d, %d) and v[%d]=%d @(%d, %d)",
				j, j, tr[j].X, tr[j].Y, j, u[j], ux, uy, j, v[j], vx, vy);
	}

	{// Checking uniqueness of edges
		map<PII, int> edge_index;
		for (int j = 0; j < m; j++) {
			PII e(u[j], v[j]);
			if (e.first > e.second)
				swap(e.first, e.second);
			auto it = edge_index.find(e);
			quitif(it != edge_index.end(),
					_wa, "Edge between %d and %d appears more than once: appeared on positions %d and %d",
					u[j], v[j], it->second, j);
			edge_index[e] = j;
		}
	}

	{// Checking uniqueness of trees
		map<Point, int> tree_index;
		for (int j = 0; j < m; j++) {
			auto it = tree_index.find(tr[j]);
			quitif(it != tree_index.end(),
					_wa, "Tree @(%d, %d) appears more than once: for edges on positions %d and %d",
					tr[j].X, tr[j].Y, it->second, j);
			tree_index[tr[j]] = j;
		}
	}

	{// Checking connectivity of the graph
		DisjointSet ds(n);
		for (int j = 0; j < m; j++)
			ds.merge(u[j], v[j]);
		int root = ds.findRoot(0);
		for(int i=0; i<n; i++)
			quitif(ds.findRoot(i) != root, _wa, "Given structure is not connected: There is no path between vertices %d and %d", 0, i);
	}

	quitif(possible_ans == 0, _fail, "Solution announced possible and built a valid structure, but we believed it is impossible.");

	quit(_ok);
}
