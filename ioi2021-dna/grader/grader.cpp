#include "dna.h"
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>

int main() {
	// BEGIN SECRET
	const std::string input_secret = "ba73dbf9c7d5e5202834d6a500541c";
	const std::string output_secret = "7abbcd01962faf1b2655df14ab1e12";
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (std::string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("PV\n");
		printf("Possible tampering with the input\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	int n, q;
	assert(scanf("%d %d", &n, &q) == 2);
	char A[n+1], B[n+1];
	assert(scanf("%s", A) == 1);
	assert(scanf("%s", B) == 1);
	std::string a = std::string(A);
	std::string b = std::string(B);
	std::vector<int> x(q), y(q);
	for (int i = 0; i < q; i++) {
		assert(scanf("%d %d", &x[i], &y[i]) == 2);
	}
	fclose(stdin);
	std::vector<int> results(q);
	init(a, b);
	for (int i = 0; i < q; i++) {
		results[i] = get_distance(x[i], y[i]);
	}
	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	for (int i = 0; i < q; i++) {
		printf("%d\n", results[i]);
	}
	fclose(stdout);
	return 0;
}
