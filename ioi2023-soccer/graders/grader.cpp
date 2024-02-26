#include "soccer.h"
#include <cassert>
#include <cstdio>
#include <vector>
// BEGIN SECRET
#include <string>
// END SECRET

int main()
{
    // BEGIN SECRET
    const std::string input_secret = "R0R7sb2atQWJ6SAWOjw4ZG7Gwgo5zl9L";
    const std::string output_secret = "xlqtkQVzqzbOJxjzxlqsyVrlM2kqlbK0";
    char secret[1000];
    assert(1 == scanf("%999s", secret));
    if (std::string(secret) != input_secret)
    {
        printf("%s\n", output_secret.c_str());
        printf("PV\n");
        printf("Possible tampering with the input\n");
        fclose(stdout);
        return 0;
    }
    // END SECRET
    int N;
    assert(1 == scanf("%d", &N));
    std::vector<std::vector<int>> F(N, std::vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            assert(1 == scanf("%d", &F[i][j]));
        }
    }
    fclose(stdin);

    int res = biggest_stadium(N, F);

    // BEGIN SECRET
    printf("%s\n", output_secret.c_str());
    printf("OK\n");
    // END SECRET
    printf("%d\n", res);
    fclose(stdout);
    return 0;
}
