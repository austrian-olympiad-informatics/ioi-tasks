// Generator for task mushrooms
// It takes the species directly from argv
// Author: Kian Mirjalali
#include <string>
#include "common.h"


int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);

	string p = argv[1];
	int n = sz(p);
	ensuref(n >= 2, "n must be at least 2.");
	fori(i, n)
		ensuref(p[i] == '0' || p[i] == '1', "Pattern '%s' has invalid character '%c' at position %d.", p.c_str(), p[i], i+1);
	ensuref(p[0] == '0', "Pattern must start with '0'.");

	VI v(n);
	fori(i, n)
		v[i] = p[i]-'0';

	write_plain(v);
}
