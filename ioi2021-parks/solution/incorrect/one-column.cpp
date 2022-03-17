#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "parks.h"

using namespace std;

vector<int> x, y;
vector<int> north, south, east, west;
vector<int> visited;


bool cmpy(int i, int j) {
	if (y[i] == y[j])
		return (x[i] < x[j]);
	return (y[i] < y[j]);
}

// Edge set and tree set
std::vector<int> u, v, a, b;

void add_edge(int i, int j) {
	u.push_back(i);
	v.push_back(j);
	if (((x[i]+y[i])/2) % 2 == 0) {
		// insert tree on left
		if (x[i] == x[j]) {
			b.push_back( (y[i]+y[j])/2 );
			if (y[j] > y[i]) {
				a.push_back(x[i]-1);
			} else {
				a.push_back(x[i]+1);
			}
		} else {
			a.push_back( (x[i]+x[j])/2 );
			if (x[j] > x[i]) {
				b.push_back(y[i]+1);
			} else {
				b.push_back(y[i]-1);
			}
		}
	} else {
		// insert tree on right
		if (x[i] == x[j]) {
			b.push_back( (y[i]+y[j])/2 );
			if (y[j] > y[i]) {
				a.push_back(x[i]+1);
			} else {
				a.push_back(x[i]-1);
			}
		} else {
			a.push_back( (x[i]+x[j])/2 );
			if (x[j] > x[i]) {
				b.push_back(y[i]-1);
			} else {
				b.push_back(y[i]+1);
			}
		}
	}
}


int construct_roads(std::vector<int> _x, std::vector<int> _y) {
	x = _x;
	y = _y;
	std::vector<int> idx;

	int n = x.size();

	for(int i=0; i<n; i++) {
		idx.push_back( i );
	}

	// Assign east & west
	std::sort(idx.begin(), idx.end(), cmpy);

	/*
	for (int i=0; i<n; i++) {
		cout << i << ": " << x[idx[i]] << "," << y[idx[i]] << endl;
	}
	*/
	int success = 1;
	for (int i=0; i<n-1; i++) {
		if (y[idx[i+1]] - y[idx[i]] == 2)
			add_edge(idx[i], idx[i+1]);
		else {
			success = 0;
			break;
		}
	}
	if (success == 1) {
		build(u, v, a, b);
		return 1;
	} else
		return 0;
}


