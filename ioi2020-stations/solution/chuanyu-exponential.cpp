#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>

const int UNVISITED = -1;
const int VISITING = -2;

void recur(
	const std::vector<std::vector<int>>& adj_list, const int cur_node, 
	int *cur_label, std::vector<int> *labels) {
	
	if (labels->at(cur_node) != UNVISITED)
		return;
	
	labels->at(cur_node) = VISITING;
	
	const int in = (*cur_label)++;
	for (const int child_node: adj_list[cur_node]) 
		recur(adj_list, child_node, cur_label, labels);
	const int out = (*cur_label)++;
	
	labels->at(cur_node) = in * 1000 + out;	
}

std::vector<int> label(
	int num_stations, int max_label, 
	std::vector<int> u, std::vector<int> v) {	
    std::vector<int> labels;
	labels.resize(num_stations);
	for (int i = 0; i < num_stations; i++) 
		labels[i] = UNVISITED;
	
	std::vector<std::vector<int>> adj_list;
	for (int i = 0; i < num_stations; i++) {
		adj_list.push_back(std::vector<int>());
	}
	
	for (int i = 0; i <= num_stations - 2; i++) {
		adj_list[u[i]].push_back(v[i]);
		adj_list[v[i]].push_back(u[i]);
	}		
	
	int cur_label = 0;
	recur(adj_list, 0, &cur_label, &labels);
	
	return labels;
}

int find_next_station(
	int source, int dest, std::vector<int> neighbours) {
					
	const int in_dest = dest / 1000;
	for (unsigned int i = 1; i < neighbours.size(); i++) {
		const int neighbour = neighbours[i];
		if (neighbour == dest) {
			return dest;
		}
		const int in = neighbour / 1000;
		const int out = neighbour % 1000;
		if ((in < in_dest) && (in_dest < out))
			return neighbour;
	}
	return neighbours[0];
}

/*
int main() {
	const std::vector<int> u = {0, 1, 1, 2};
	const std::vector<int> v = {1, 2, 3, 4};
	const std::vector<int> result = label(5, 10, u, v);
	for (const int label : result) {
		printf("%d\n", label);
	}
	printf("\n");
	printf("%d\n", find_next_station(1008, 6007, {9, 2005, 6007}));
	return 0;
}

*/