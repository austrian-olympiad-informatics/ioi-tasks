#include "testlib.h"
using namespace std;

int main(int argc, char * argv[])
{
	registerChecker("shoes", argc, argv);
	compareRemainingLines();
}
