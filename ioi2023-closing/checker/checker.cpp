#include "testlib.h"

#include <string>

int main(int argc, char *argv[])
{
    registerChecker("closing", argc, argv);

    std::string output_secret = "081ce3c351cbf526b37954b9ad30f2b531a7585c";

    readBothSecrets(output_secret);
    readBothGraderResults();

    compareRemainingLines();
}
