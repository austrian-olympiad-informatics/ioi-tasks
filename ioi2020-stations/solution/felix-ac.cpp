#include "stations.h"
#include <vector>

void rec(int index, int parent, int pre_order, int &number,
  std::vector<std::vector<int>>& con, std::vector<int>& labels) {
  if (pre_order) labels[index] = number++;
  for (int i : con[index]) {
    if (i == parent) continue;
    rec(i, index, !pre_order, number, con, labels);
  }
  if (!pre_order) labels[index] = number++;
}

std::vector<int> label(int n, int k, std::vector<int> u, std::vector<int> v) {
  std::vector<int> labels(n);
	std::vector<std::vector<int>> con(n, std::vector<int>());
  for (int i = 0; i < u.size(); i++) {
    con[u[i]].push_back(v[i]);
    con[v[i]].push_back(u[i]);
  }
  int number = 0;
  rec(0, -1, true, number, con, labels);
	return labels;
}

int find_next_station(int s, int t, std::vector<int> c) {
  if (c[0] > s) {
    // Case 1: node s is in pre_order level.
    // All the neighbors are higher than s.

    if (t < s) {
      // The target is smaller than the source.
      // The target is definitely not in this subtree, go to parent.
      return c.back();
    }

    if (t > c.back()) {
      // The target is higher than the largest neighbor.
      // The target cannot be in this subtree, go to parent.
      return c.back();
    }

    // The target is in this subtree.
    // Pick the smallest child that's at least the target.
    int next = 0;
    while (c[next] < t) next++;
    return c[next];
  }


  // Case 2: node s is in the post_order level.
  if (t < c[0]) {
    // The target is smaller than the pre_order root c[0],
    // thus not in this subtree, go to the root.
    return c[0];
  }

  if (t > s) {
    // The target is higher than this post_order value.
    // Thus it's not in this subtree, go to the root.
    return c[0];
  }

  int next = c.size() - 1;
  while (c[next] > t) next--;
	return c[next];
}
