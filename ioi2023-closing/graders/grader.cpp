#include "closing.h"

#include <cassert>
#include <cstdio>

// BEGIN SECRET
#include <string>
// END SECRET
#include <vector>

int main()
{
    // BEGIN SECRET
    {
        std::string in_secret = "cc61ad56a4797fb3f5c9529f73ce6fcedd85669b";
        std::string out_secret = "081ce3c351cbf526b37954b9ad30f2b531a7585c";
        char secret[1000];
        assert(1 == scanf("%s", secret));
        if (std::string(secret) != in_secret)
        {
            printf("%s\n", out_secret.c_str());
            printf("SV\n");
            fclose(stdout);
            return 0;
        }
    }
    // END SECRET

    int Q;
    assert(1 == scanf("%d", &Q));

    std::vector<int> N(Q), X(Q), Y(Q);
    std::vector<long long> K(Q);
    std::vector<std::vector<int>> U(Q), V(Q), W(Q);

    for (int q = 0; q < Q; q++)
    {
        assert(4 == scanf("%d %d %d %lld", &N[q], &X[q], &Y[q], &K[q]));

        U[q].resize(N[q] - 1);
        V[q].resize(N[q] - 1);
        W[q].resize(N[q] - 1);
        for (int i = 0; i < N[q] - 1; ++i)
        {
            assert(3 == scanf("%d %d %d", &U[q][i], &V[q][i], &W[q][i]));
        }
    }
    fclose(stdin);

    std::vector<int> result(Q);
    for (int q = 0; q < Q; q++)
    {
        result[q] = max_score(N[q], X[q], Y[q], K[q], U[q], V[q], W[q]);
    }

    // BEGIN SECRET
    {
        std::string out_secret = "081ce3c351cbf526b37954b9ad30f2b531a7585c";
        printf("%s\n", out_secret.c_str());
        printf("OK\n");
    }
    // END SECRET
    for (int q = 0; q < Q; q++)
    {
        printf("%d\n", result[q]);
    }
    fclose(stdout);

    return 0;
}
