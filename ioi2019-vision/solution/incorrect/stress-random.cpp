#include <cstdlib>
#include <vector>
#include "vision.h"
using namespace std;
#define fori(i, n) for (int i=0; i<(n); i++)

void construct_network(int n, int m, int) {
	srand(10);
	int g = 10000;
	vector<int> v(g, 1);
	fori(i, 1000000-g)
		v[rand()%g]++;
	fori(i, g) {
		int l = i+m*n;
		if (v[i] == 1 && rand()%2==1)
			add_not(rand()%l);
		else {
			vector<int> in;
			fori(j, v[i])
				in.push_back(rand()%l);
			int t = rand()%3;
			if (t==0)
				add_and(in);
			else if (t==1)
				add_or(in);
			else
				add_xor(in);
		}
	}
}
