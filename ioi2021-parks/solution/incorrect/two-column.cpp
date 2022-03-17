#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "parks.h"

// All edges in column 2, face the tree on column 1
// All edges in column 4, face the tree on column 5
// All horizontal edges, face the tree just above it on column 3

using namespace std;

vector<int> x, y;
vector<int> north, south, east, west;
vector<int> visited;

bool cmpx(int i, int j) {
	if (x[i] == x[j])
		return (y[i] < y[j]);
	return (x[i] < x[j]);
}

bool cmpy(int i, int j) {
	if (y[i] == y[j])
		return (x[i] < x[j]);
	return (y[i] < y[j]);
}


void RunDFS(int s) {
    vector<int> stack;
//    cout << s << visited[s] << " north: " << north[s] << " east: " << east[s] << " south: " << south[s] << " west: " << west[s] << endl;
    stack.push_back(s);
    while (stack.size() > 0) {
	    int t = stack.back();
	    stack.pop_back();
	    if (visited[t] != 1) {
		    visited[t]=1;
		    if(north[t] != -1) stack.push_back(north[t]);
      	 	    if(west[t] != -1) stack.push_back(west[t]);
	            if(south[t] != -1) stack.push_back(south[t]);
       		    if(east[t] != -1) stack.push_back(east[t]);
	    }
    }
}

// Edge set and tree set
std::vector<int> u, v, a, b;

void add_edge(int i, int j) {
	u.push_back(i);
	v.push_back(j);
	if (x[i] == 2 && x[j] == 2) {
		a.push_back(1);
		b.push_back( (y[i]+y[j])/2 );
	} else if (x[i] == 4 && x[j] == 4) {
		a.push_back(5);
		b.push_back( (y[i]+y[j])/2 );
	} else {
		a.push_back(3);
		b.push_back( (y[i]+1) );
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



	for (int i=0; i<n; i++) {
		north.push_back(-1);
		south.push_back(-1);
		east.push_back(-1);
		west.push_back(-1);
	}


	// Assign north & south
	std::sort(idx.begin(), idx.end(), cmpx);
	for (int i=0; i<n-1; i++) {
		if (x[idx[i]]==x[idx[i+1]] && y[idx[i]]+2 == y[idx[i+1]]) {
			north[idx[i]] = idx[i+1];
			south[idx[i+1]] = idx[i];
			add_edge(idx[i], idx[i+1]);
		}
	}

	// Assign east & west
	std::sort(idx.begin(), idx.end(), cmpy);
	for (int i=0; i<n-1; i++) {
		if (y[idx[i]]==y[idx[i+1]] && x[idx[i]]+2 == x[idx[i+1]]) {
			east[idx[i]] = idx[i+1];
			west[idx[i+1]] = idx[i];
			add_edge(idx[i], idx[i+1]);
		}
	}

//	cout << "Finish assigning directions\n";

	for(int i=0; i<n; i++) {
		visited.push_back(-1);
	}
	RunDFS(0);

	bool connected = true;
	for(int i=0; i<n; i++) {
		if (visited[i] == -1) {
			connected = false;
			break;
		}
	}

	if (connected == true) {
		build(u, v, a, b);
		return 1;
	} else {
		return 0;
	}
}


