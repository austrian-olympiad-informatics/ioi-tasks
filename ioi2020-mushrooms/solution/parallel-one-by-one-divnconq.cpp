/*
 * A parallel implementation of the one-by-one solution (creating threads through divide and conquer).
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
	function<void (int, int)> handle_range;
	handle_range = [block_size, &species, &handle_range] (int begin, int end) {
		const int l = end - begin;
		if (l <= 0)
			return;
		if (l <= block_size) {
			for (int i = begin; i < end; i++)
				species[i] = use_machine({0, i});
			return;
		}
		// l >= 2
		const int mid = begin + l/2;
		thread th(handle_range, begin, mid);
		handle_range(mid, end);
		th.join();
	};
	handle_range(1, n);
	return count(species.begin(), species.end(), 0);
}
