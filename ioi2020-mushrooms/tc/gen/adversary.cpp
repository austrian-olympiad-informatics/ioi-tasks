// Generator for task mushrooms
// It takes an adversary algorithm name with its parameters
//   and writes it as an input with the appropriate format.
// Author: Kian Mirjalali
#include "common.h"

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);
	string adversary_type = argv[1];
	cout << "adversary_"+adversary_type << endl;
	for (int i=2; i < argc; i++)
		cout << ((i>2)?" ":"") << argv[i];
	cout << endl;
}
