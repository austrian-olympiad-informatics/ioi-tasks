#include "parks.h"
#include <cstdio>
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
using namespace std;


static void check(bool cond, string message) {
	if (!cond) {
		// BEGIN SECRET
		const string output_secret = "3kr2yac8xnf3ktgcoqviaw115df6rra7is6p5uix";
		printf("%s\n", output_secret.c_str());
		printf("WA\n");
		// END SECRET
		printf("%s\n", message.c_str());
		fclose(stdout);
		exit(0);
	}
}

static int n;
static bool build_called;
static int m;
static vector<int> _u, _v, _a, _b;

void build(vector<int> u, vector<int> v, vector<int> a, vector<int> b) {
	check(!build_called, "build is called more than once");
	build_called = true;
	m = u.size();
	check(int(v.size()) == m, "u.size() != v.size()");
	check(int(a.size()) == m, "u.size() != a.size()");
	check(int(b.size()) == m, "u.size() != b.size()");
	// BEGIN SECRET
	// Preventing excessive output:
	check(m <= 2*n, "Construction too large"); // Vertex degrees are at most 4
	// END SECRET
	_u = u;
	_v = v;
	_a = a;
	_b = b;
}

int main() {
	// BEGIN SECRET
	const string input_secret = "ba73dbf9c7d5e5202834d6a500541c";
	const string output_secret = "3kr2yac8xnf3ktgcoqviaw115df6rra7is6p5uix";
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("PV\n");
		printf("Possible tampering with the input\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	assert(scanf("%d", &n) == 1);
	vector<int> x(n), y(n);
	for (int i = 0; i < n; i++) {
		assert(scanf("%d%d", &x[i], &y[i]) == 2);
	}
	fclose(stdin);

	build_called = false;
	const int possible = construct_roads(x, y);

	check(possible == 0 || possible == 1, "Invalid return value of construct_roads()");
	if (possible == 1) {
		check(build_called, "construct_roads() returned 1 without calling build()");
	} else {
		check(!build_called, "construct_roads() called build() but returned 0");
	}

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	printf("%d\n", possible);
	if (possible == 1) {
		printf("%d\n", m);
		for (int j = 0; j < m; j++) {
			printf("%d %d %d %d\n", _u[j], _v[j], _a[j], _b[j]);
		}
	}
	fclose(stdout);
	return 0;
}
