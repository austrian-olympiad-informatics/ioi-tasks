#include "vision.h"

void construct_network(int H, int W, int K) {
	std::vector<int> Ns;
	Ns = {0, 1};
	int a = add_and(Ns);
	Ns = {0, a};
	int b = add_or(Ns);
	Ns = {0, 1, b};
	int c = add_xor(Ns);
	add_not(c);
}
