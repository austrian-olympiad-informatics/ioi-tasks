/*
 * A parallel implementation of the one-by-one solution (creating threads linearly).
 * The purpose of the parallel solutions is to verify if the graders have the correct behavior
 *   when contestants use parallelism in their solutions.
 * This solution needs "-pthread" as a link argument when compiling.
 *
 * Author: Kian Mirjalali
 */
#include "mushrooms.h"
#include <algorithm>
#include <thread>
#include <functional>
using namespace std;

int count_mushrooms(int n) {
	vector<int> species(n, 0);
	const int max_threads = 200;
	const int block_size = max(1, n/max_threads);
	vector<thread> threads;
	for (int begin = 1; begin < n; begin += block_size) {
		const int end = min(n, begin+block_size);
		threads.emplace_back([begin, end, &species]() {
			for (int i = begin; i < end; i++)
				species[i] = use_machine({0, i});
		});
	}
	for (auto& th : threads)
		th.join();
	return count(species.begin(), species.end(), 0);
}
