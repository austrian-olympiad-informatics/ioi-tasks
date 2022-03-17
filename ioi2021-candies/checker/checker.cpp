#include "testlib.h"
using namespace std;

static string output_secret = "4lpv73iykswp9e3nppi3jw2qggm5dfz29yy9nzpq";

int main(int argc, char * argv[])
{
	registerChecker("candies", argc, argv);

	readBothSecrets(output_secret);
	readBothGraderResults();

	compareRemainingLines();
}
