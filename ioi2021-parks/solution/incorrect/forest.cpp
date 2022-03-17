#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "parks.h"


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

/*
void RunDFS(int s) {
    if(visited[s] == 1) return;
    visited[s]=1;
    if(north[s] != -1) RunDFS(north[s]);
    if(west[s] != -1) RunDFS(west[s]);
    if(south[s] != -1) RunDFS(south[s]);
    if(east[s] != -1) RunDFS(east[s]);
}
*/

// Edge set and tree set
std::vector<int> u, v, a, b;

void add_edge(int i, int j) {
	u.push_back(i);
	v.push_back(j);
//	cout << "merge " << i << "," << j << endl;
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

/*
void build(std::vector<int> u, std::vector<int> v, std::vector<int> a, std::vector<int> b) {
	return 0;
}
*/

int construct_roads(std::vector<int> _x, std::vector<int> _y) {
	x = _x;
	y = _y;
	std::vector<int> idx;

	int n = x.size();

	for(int i=0; i<n; i++) {
		idx.push_back( i );
	}

	/*
	cout << "The vector before applying sort is:\n" ;
	for (int i=0; i<n; i++) {
		cout << idx[i] << ": " << x[idx[i]] << " " << y[idx[i]] << endl;
	}
	*/


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
			add_edge(idx[i], idx[i+1]);
			north[idx[i]] = idx[i+1];
			south[idx[i+1]] = idx[i];
		}
	}

	// Assign east & west
	std::sort(idx.begin(), idx.end(), cmpy);
	for (int i=0; i<n-1; i++) {
		if (y[idx[i]]==y[idx[i+1]] && x[idx[i]]+2 == x[idx[i+1]]) {
			add_edge(idx[i], idx[i+1]);
			east[idx[i]] = idx[i+1];
			west[idx[i+1]] = idx[i];
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
	if (connected == false) {
//		cout << "It is not connected" << endl;
		return 0;
	}
//	cout << "It is connected" << endl;


	build(u, v, a, b);

	return 1;
}


