#include "candies.h"

#include <cassert>
#include <cstdio>
#include <vector>
// BEGIN SECRET
#include <string>
// END SECRET

int main() {
    // BEGIN SECRET
    const std::string input_secret = "lrts0z0ktpqc670i0etyqgyb45dt1dysq5ap2mzg";
    const std::string output_secret = "4lpv73iykswp9e3nppi3jw2qggm5dfz29yy9nzpq";
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
    int n;
    assert(1 == scanf("%d", &n));
    std::vector<int> c(n);
    for(int i = 0; i < n; ++i) {
        assert(scanf("%d", &c[i]) == 1);
    }

    int q;
    assert(1 == scanf("%d", &q));
    std::vector<int> l(q), r(q), v(q);
    for(int i = 0; i < q; ++i) {
        assert(scanf("%d %d %d", &l[i], &r[i], &v[i]) == 3);
    }

    std::vector<int> ans = distribute_candies(c, l, r, v);
    // BEGIN SECRET
    printf("%s\n", output_secret.c_str());
    if (int(ans.size()) != n) {
        printf("WA\nReturned array is of the wrong size (len=%d).\n", (int)ans.size());
        return 0;
    }
    printf("OK\n");
    // END SECRET

    for(int i = 0; i < n; ++i) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", ans[i]);
    }
    printf("\n");
    fclose(stdout);
    return 0;
}
