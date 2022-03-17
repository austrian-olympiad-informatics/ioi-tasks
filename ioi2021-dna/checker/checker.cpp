#include "testlib.h"
using namespace std;

static string output_secret = "7abbcd01962faf1b2655df14ab1e12";

int main(int argc, char * argv[])
{
	registerChecker("dna", argc, argv);

	readBothSecrets(output_secret);
	readBothGraderResults();

	compareRemainingLines();
}
