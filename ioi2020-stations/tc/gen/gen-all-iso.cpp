#include "testlib.h"
#include <cassert>
#include <queue>
#include <bitset>
#define MAXN 1000
using namespace std;

int main(int argc, char **argv) {
  // read in test cases
	registerGen(argc, argv, 1);
	int first = atoi(argv[1]);
	int second = atoi(argv[2]);
	int is_partial = atoi(argv[3]);
	int tc = second-first;
	printf("%d\n", tc);
	vector<string> iso = {"0000000","0000001","0000011","0000012","0000016","0000111","0000112","0000115","0000123","0000125","0000155","0000156","0001122","0001123","0001124","0001126","0001234","0001244","0001245","0001246","0001455","0001456","0012345","000000","000001","000011","000012","000015","000112","000123","000124","000144","000145","001234","00000","00001","00011","00012","00014","00123","0000","0001","0012","000","001","00"};
	for(tc=first; tc<second; tc++) {
        string tree_type = iso[tc];
        int n = tree_type.size();
        vector<int> adj[n];
        int parent[n];
        for(int i=0; i<n-1; i++) {
            parent[i+1] = tree_type[i]-'0';
        }
        for(int i=1; i<n; i++) {
            adj[i].push_back(parent[i]);
            adj[parent[i]].push_back(i);
        }
        int perm[n];
        for(int i=0; i<n; i++) perm[i] = i;
        shuffle(perm, perm+n);
        printf("%d %d\n", n, 1000000000);
        for(int i=1; i<n; i++) {
            printf("%d %d\n", perm[i], perm[parent[i]]);
        }

        printf("%d\n", n*(n-1));
        for(int u=0; u<n; u++) {
            for(int v=0; v<n; v++) {
                if(u==v) continue;
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
	}
	printf("%d 231397\n", is_partial);
	return 0;
}
