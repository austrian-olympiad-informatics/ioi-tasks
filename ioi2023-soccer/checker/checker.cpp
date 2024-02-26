#include "testlib.h"

#include <string>
const std::string output_secret = "xlqtkQVzqzbOJxjzxlqsyVrlM2kqlbK0";

int main(int argc, char *argv[])
{
    registerChecker("soccer", argc, argv);
    readBothSecrets(output_secret);
    readBothGraderResults();

    std::string secret = inf.readToken();
    int N = inf.readInt();
    int empty_cells = 0;
    std::vector<std::vector<int>> F(N, std::vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            F[i][j] = inf.readInt();
            empty_cells += F[i][j] == 0;
        }
    }

    int jans = ans.readInt(), pans = ouf.readInt();
    if (jans == empty_cells)
    {
        if (jans == pans)
            quit(_ok, "ok");
        else
            quit(_wa, "wrong");
    }
    else
    {
        if (jans == pans)
            quit(_ok, "ok");
        else if (pans != empty_cells)
            quitp(0.25, "partial");
        else
            quit(_wa, "wrong");
    }
}
