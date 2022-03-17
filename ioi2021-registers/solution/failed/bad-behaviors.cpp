#include "registers.h"
#include <iostream>
using namespace std;

void construct_instructions(int s, int n, int k, int q) {
	const int m = 100;
	const int b = 2000;

	if (s==0 && n==2 && k==1 && q==1000) {
		cerr << "empty" << endl;
		return;
	}
	if (s==1 && n==2 && k==1 && q==1000) {
		cerr << "tamper output" << endl;
		cout << "hi" << endl;
		append_move(1, 0);
		return;
	}
	if (s==0 && n==2 && k==2 && q==1000) {
		cerr << "short v" << endl;
		vector<bool> v(b-1, false);
		append_store(1, v);
		return;
	}
	if (s==0 && n==2 && k==2 && q==20) {
		cerr << "long v" << endl;
		vector<bool> v(b+1, false);
		append_store(1, v);
		return;
	}
	if (s==0 && n==100 && k==10 && q==4000) {
		cerr << "shift-left low p" << endl;
		append_left(0, 1, -1);
		return;
	}
	if (s==0 && n==89 && k==7 && q==4000) {
		cerr << "shift-left high p" << endl;
		append_left(0, 1, b+1);
		return;
	}
	if (s==0 && n==94 && k==9 && q==4000) {
		cerr << "many instructions (4000)" << endl;
		for (int i=0; i<q+1; i++)
			append_move(1, 0);
		return;
	}
	if (s==0 && n==100 && k==10 && q==150) {
		cerr << "shift-right low p" << endl;
		append_right(0, 1, -1);
		return;
	}
	if (s==0 && n==80 && k==9 && q==150) {
		cerr << "shift-right high p" << endl;
		append_right(0, 1, b+1);
		return;
	}
	if (s==0 && n==96 && k==8 && q==150) {
		cerr << "many instructions (150)" << endl;
		vector<bool> v(b, false);
		for (int i=0; i<q+1; i++)
			append_store(1, v);
		return;
	}
	if (s==0 && n==65 && k==10 && q==150) {
		cerr << "param1 low" << endl;
		append_move(-1, 0);
		return;
	}
	if (s==0 && n==64 && k==10 && q==150) {
		cerr << "param1 high" << endl;
		append_move(m, 0);
		return;
	}
	if (s==0 && n==63 && k==10 && q==150) {
		cerr << "param2 low" << endl;
		append_move(0, -1);
		return;
	}
	if (s==1 && n==10 && k==10 && q==4000) {
		cerr << "param2 high" << endl;
		append_move(0, m);
		return;
	}
	if (s==1 && n==8 && k==7 && q==4000) {
		cerr << "param3 low" << endl;
		append_add(1, 0, -1);
		return;
	}
	if (s==1 && n==100 && k==10 && q==4000) {
		cerr << "param3 high" << endl;
		append_add(1, 0, m);
		return;
	}
// gen 1 91 6 4000
// gen 1 65 10 4000
// gen 1 64 10 4000
// gen 1 63 10 4000
	append_right(1, 1, 1);
	append_and(0, 0, 1);
}
