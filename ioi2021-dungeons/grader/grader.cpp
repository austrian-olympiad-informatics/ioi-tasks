#include "dungeons.h"
#include <vector>
#include <cassert>
#include <cstdio>
// BEGIN SECRET
#include <string>
// END SECRET

static int n, q;
static std::vector<int> s, p, z;
static std::vector<int> w, l, x;
static std::vector<long long> answer;

int main() {
    // BEGIN SECRET
    const std::string input_secret = "b50747e9-747c-4fca-b3b0-62317b32d2f6";
    const std::string output_secret = "f39eb8f7-7d10-4b4a-af02-d7aef3d4dd0a";
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
	assert(scanf("%d %d", &n, &q) == 2);
	s.resize(n);
	p.resize(n);
	w.resize(n);
	l.resize(n);
    x.resize(q);
    z.resize(q);
    answer.resize(q);

	for (int i = 0; i < n; i++) {
		assert(scanf("%d", &s[i]) == 1);
	}
	for (int i = 0; i < n; i++) {
		assert(scanf("%d", &p[i]) == 1);
	}
	for (int i = 0; i < n; i++) {
		assert(scanf("%d", &w[i]) == 1);
	}
	for (int i = 0; i < n; i++) {
		assert(scanf("%d", &l[i]) == 1);
	}


    init(n, s, p, w, l);

    for (int i = 0; i < q; i++) {
		assert(scanf("%d %d", &x[i], &z[i]) == 2);
		answer[i] = simulate(x[i], z[i]);
    }
    fclose(stdin);

    // BEGIN SECRET
    printf("%s\n", output_secret.c_str());
    printf("OK\n");
    // END SECRET
    for (int i = 0; i < q; i++) {
        printf("%lld\n", answer[i]);
    }
    fclose(stdout);
    return 0;
}
