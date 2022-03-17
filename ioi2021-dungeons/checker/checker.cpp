#include "testlib.h"
#include <vector>

const std::string output_secret = "f39eb8f7-7d10-4b4a-af02-d7aef3d4dd0a";

int main(int argc, char * argv[]) {
    registerChecker("dungeons", argc, argv);
    
    readBothSecrets(output_secret);
    readBothGraderResults();

    compareRemainingLines();
}