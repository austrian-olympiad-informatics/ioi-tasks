#include "testlib.h"
using namespace std;

static string output_secret = "Ia5BbhHWUaqX58JkCdhOSt";

int main(int argc, char * argv[])
{
	registerChecker("keys", argc, argv);

	readBothSecrets(output_secret);
	readBothGraderResults();

	compareRemainingLines();
}
