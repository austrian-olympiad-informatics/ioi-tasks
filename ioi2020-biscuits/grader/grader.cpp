#include "biscuits.h"
#include <cassert>
#include <cstdio>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;

int main() {
	// BEGIN SECRET
	const string input_secret = "1b32a07d5f5fc55f21038b12a3655e";
	const string output_secret = "23b69acd873f5d7e892bae7de83615";
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	int q;
	assert(scanf("%d", &q) == 1);
	vector<int> k(q);
	vector<long long> x(q);
	vector<vector<long long>> a(q);
	vector<long long> results(q);
	for (int t = 0; t < q; t++) {
		assert(scanf("%d%lld", &k[t], &x[t]) == 2);
		a[t] = vector<long long>(k[t]);
		for (int i = 0; i < k[t]; i++) {
			assert(scanf("%lld", &a[t][i]) == 1);
		}
	}
	fclose(stdin);

	for (int t = 0; t < q; t++) {
		results[t] = count_tastiness(x[t], a[t]);
	}
	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	for (int t = 0; t < q; t++) {
		printf("%lld\n", results[t]);
	}
	fclose(stdout);
	return 0;
}

