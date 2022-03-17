#include "keys.h"
#include <cassert>
#include <cstdio>
// BEGIN SECRET
#include <string>
// END SECRET

int main() {
    // BEGIN SECRET
    const std::string input_secret = "xVPHhgQS9btZF57s4vEoQhd6CQPdpE8GWf";
    const std::string output_secret = "Ia5BbhHWUaqX58JkCdhOSt";
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
    int n, m;
    assert(2 == scanf("%d%d", &n, &m));
    std::vector<int> r(n), u(m), v(m), c(m);
    for(int i=0; i<n; i++) {
        assert(1 == scanf("%d", &r[i]));
    }
    for(int i=0; i<m; i++) {
        assert(3 == scanf("%d %d %d", &u[i], &v[i], &c[i]));
    }
    fclose(stdin);

    std::vector<int> ans = find_reachable(r, u, v, c);

    // BEGIN SECRET
    printf("%s\n", output_secret.c_str());
    if (int(ans.size()) != n) {
        printf("WA\nReturned array is of the wrong size (len=%d).\n", (int)ans.size());
        return 0;
    }
    for(int i=0; i< (int) ans.size(); i++) {
        if(ans[i]!=0 and ans[i]!=1) {
            printf("WA\nans[%d] should be 0 or 1.\n", i);
            return 0;
        }
    }
    printf("OK\n");
    // END SECRET
    for (int i = 0; i < (int) ans.size(); ++i) {
        if(i) putchar(' ');
        putchar(ans[i]+'0');
    }
    printf("\n");
    return 0;
}
