#include "testlib.h"
#include <cassert>
#include <queue>
#include <bitset>
#define MAXN 1000
using namespace std;

int parent[1000];
int find_set(int pos) {
    return parent[pos] == pos ? pos : parent[pos] = find_set(parent[pos]);
}
bool onion(int x, int y) {
    x = find_set(x);
    y = find_set(y);
    if(x==y) return false;
    parent[x] = y;
    return true;
}

int main(int argc, char **argv) {
  // read in test cases
	registerGen(argc, argv, 1);
	int tc = 10;
	printf("%d\n", tc);
	for(int a=0; a<tc; a++) {
        int n = rnd.next(950, 1000);
        printf("%d %d\n", n, 1000000000);
        int m = 10; // size of the small tree

        vector<vector<int>> edgelist;
        for(int i=0; i<m; i++) parent[i] = i;

        for(int i=0; i<m-1; i++) {
            int x = rnd.next(m);
            int y = rnd.next(m);

            while(!onion(x,y)) {
                x = rnd.next(m);
                y = rnd.next(m);
            }
            edgelist.push_back({x,y});
        }
        for(int i=m; i<n; i++) {
            int idx = rnd.next(m-1);
            int tmp = edgelist[idx].back();
            edgelist[idx].pop_back();
            edgelist[idx].push_back(i);
            edgelist[idx].push_back(tmp);
        }
        vector<int> adj[n];
        int perm[n];
        for(int i=0; i<n; i++) perm[i] = i;
        shuffle(perm, perm+n);
        int ctr = 0;
        for(int i=0; i<m-1; i++) {
            for(int j=1; j<edgelist[i].size(); j++) {
                ctr++;
                int u = edgelist[i][j-1];
                int v = edgelist[i][j];
                printf("%d %d\n", perm[u], perm[v]);
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
        assert(ctr==n-1);
        int q = rnd.next(3000, 4000);
        printf("%d\n", q);
        while(q--) {
            int u = rnd.next(n);
            int v = rnd.next(n);
            while(u==v) {
                u = rnd.next(n);
                v = rnd.next(n);
            }
            queue<int> bfs;
            bfs.push(v);
            bitset<1000> visited;
            visited[v] = true;
            printf("%d %d ", perm[u], perm[v]);
            bool done = false;
            while(true) {
                if(done) break;
                int next = bfs.front();
                bfs.pop();
                for(int w: adj[next]) {
                    if(w==u) {
                        printf("%d\n", perm[next]);
                        done = true;
                        break;
                    }
                    if(!visited[w]) {
                        visited[w] = true;
                        bfs.push(w);
                    }
                }
            }
        }
	}
	printf("1 233357\n");
	return 0;
}
