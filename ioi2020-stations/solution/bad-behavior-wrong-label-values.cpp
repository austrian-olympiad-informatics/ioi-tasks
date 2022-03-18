#include "stations.h"
#include <vector>

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
	std::vector<int> labels(n);
	for (int i=0; i<n; i++)
		labels[i] = ((n%100 == 0) ? -1 : (k+1));
	return labels;
}

int find_next_station(int s, int t, std::vector<int> c) {
	return c[0];
}
