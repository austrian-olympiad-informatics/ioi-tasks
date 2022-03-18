#include "stations.h"
#define MAXN 1000

std::vector<int> labels;

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
	labels.resize(n);
	for (int i = 0; i < n; i++) {
	  labels[i] = i;
	}
	return labels;
}

int find_next_station(int s, int t, std::vector<int> c) {
  ++s; ++t;
  while (s < t) {
    if (t / 2 == s) return t - 1;
    t /= 2;
  }
  return s / 2 - 1;
}
