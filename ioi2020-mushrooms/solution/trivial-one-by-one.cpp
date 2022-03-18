/*
 * Trivial one-by-one solution
 *
 * Author: Kian Mirjalali
 */
#include "mushrooms.h"

int count_mushrooms(int n) {
	int s = 1;
	for (int i=1; i<n; i++)
		s += 1 - use_machine({0, i});
	return s;
}
