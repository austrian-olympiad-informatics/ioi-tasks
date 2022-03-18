#include "testlib.h"
#include <cassert>
#define MAXN 1000
using namespace std;

struct test_case {
  string type;
  int n, k, c;
  int w, legs;
};
typedef struct test_case test_case;
vector<test_case> test_cases;

typedef pair<int,int> ii;
vector<int> perm, adjlist[MAXN];
vector<ii> edges, queries;

void gen_perm(int n) {
  perm.clear();
  for (int i = 0; i < n; i++) {
    perm.push_back(i);
  }
  shuffle(perm.begin(), perm.end());
}

void random_queries(int n, int c) {
  for (auto edge : edges) {
    adjlist[edge.first].push_back(edge.second);
    adjlist[edge.second].push_back(edge.first);
  }
  int totalc = 0, failcnt = 0;
  while (failcnt < 10) {
    int s = rnd.next(n);
    int t = rnd.next(n);
    if (s == t) continue;
    if (totalc + (int)adjlist[s].size() <= c) {
       queries.push_back(make_pair(s, t));
       totalc += (int)adjlist[s].size();
       failcnt = 0;
    } else {
      ++failcnt;
    }
  }
}

void inc_line(int n, int c) {
  for (int i = 0; i < n - 1; i++) {
    edges.push_back(ii(i, i + 1));
  }
	shuffle(edges.begin(), edges.end());
  random_queries(n, c);
}

void random_line(int n, int c) {
  gen_perm(n);
  for (int i = 0; i < n - 1; i++) {
    edges.push_back(ii(perm[i], perm[i + 1]));
  }
	shuffle(edges.begin(), edges.end());
  random_queries(n, c);
}

void binary_tree(int n, int c) {
  for (int i = 0; i < n - 1; i++) {
    edges.push_back(ii(i / 2,  i + 1));
  }
  random_queries(n, c);
}

// low w for shallower stars
// high w for deeper stars
void random_star(int n, int c, int w, int legs) {
  assert(legs <= n - 1);
  gen_perm(n);
  vector<int> leaves(legs);
  for (int i = 1; i <= legs; i++) {
    edges.push_back(ii(perm[0], perm[i]));
    leaves[i - 1] = i;
  }
  for (int i = legs + 1; i < n; i++) {
    int leg = rnd.wnext(legs, w);
    edges.push_back(ii(perm[leaves[leg]], perm[i]));
    leaves[leg] = i;
  }
	shuffle(edges.begin(), edges.end());
  random_queries(n, c);
}

// low w for shallower trees
// high w for deeper trees
void random_tree(int n, int c, int w) {
  gen_perm(n);
  for (int i = 1; i < n; i++) {
    edges.push_back(ii(perm[rnd.wnext(i, w)], perm[i]));
  }
	shuffle(edges.begin(), edges.end());
  random_queries(n, c);
}

// dfs to find the query answers
int pa[MAXN], tin[MAXN], toutt[MAXN], tcnt;
void dfs(int v, int p) {
  pa[v] = p;
  tin[v] = tcnt++;
  for (int i : adjlist[v]) if (i != p) dfs(i, v);
  toutt[v] = tcnt;
}

int main(int argc, char **argv) {
  // read in test cases
	registerGen(argc, argv, 1);
	int r = atoi(argv[1]);
	int index = 2;
	for (int i = 0; i < r; i++) {
	  test_case tc;
	  tc.type = string(argv[index++]);
	  tc.n = atoi(argv[index++]);
	  tc.k = atoi(argv[index++]);
	  tc.c = atoi(argv[index++]);
	  if (tc.type == "random_star" || tc.type == "random_tree") {
	    tc.w = atoi(argv[index++]);
	  }
	  if (tc.type == "random_star") {
	    tc.legs = atoi(argv[index++]);
	  }
	  test_cases.push_back(tc);
	}
	
	printf("%d\n", r);
	shuffle(test_cases.begin(), test_cases.end());
	for (int i = 0; i < r; i++) {
	  // generate edges and queries
	  auto& tc = test_cases[i];
	  for (int j = 0; j < tc.n; j++) {
	    adjlist[j].clear();
	  }
	  edges.clear();
	  queries.clear();
	  if (tc.type == "inc_line") inc_line(tc.n, tc.c);
	  if (tc.type == "random_line") random_line(tc.n, tc.c);
	  if (tc.type == "binary_tree") binary_tree(tc.n, tc.c);
	  if (tc.type == "random_star") random_star(tc.n, tc.c, tc.w, tc.legs);
	  if (tc.type == "random_tree") random_tree(tc.n, tc.c, tc.w);
	  
	  // print edges
	  printf("%d %d\n", tc.n, tc.k);
	  assert((int)edges.size() == tc.n - 1);
	  for (auto edge : edges) {
	    if (rnd.next(2)) {
	      printf("%d %d\n", edge.first, edge.second);
	    } else {
	      printf("%d %d\n", edge.second, edge.first);
	    }
	  }
	  
	  // call dfs to calculate query answers
	  tcnt = 0;
	  dfs(0, -1);
	  
	  // print queries with answers
	  printf("%d\n", (int)queries.size());
	  shuffle(queries.begin(), queries.end());
	  for (auto query : queries) {
	    int s = query.first, t = query.second;
	    if (tin[t] < tin[s] || tin[t] >= toutt[s]) {
	      printf("%d %d %d\n", s, t, pa[s]);
	    } else {
	      for (int i : adjlist[s]) {
	        if (i == pa[s]) continue;
	        if (tin[t] >= tin[i] && tin[t] < toutt[i]) {
	          printf("%d %d %d\n", s, t, i);
	          break;
	        }
	      }
	    }
	  }
	}
	printf("%d %d\n", atoi(argv[index]), rnd.next(INT_MAX));
	return 0;
}