#include "mushrooms.h"

int count_mushrooms(int n) {
	std::vector<int> m;
	for (int i = 0; i < n; i++)
		m.push_back(i);
	int c1 = use_machine(m);
	m = {0, 1};
	int c2 = use_machine(m);
	return c1+c2;
}
