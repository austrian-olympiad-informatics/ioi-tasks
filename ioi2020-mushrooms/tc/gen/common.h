#include <iostream>
#include <vector>
#include "testlib.h"

using namespace std;

#define fori(i, n) for (int i=0; i<n; i++)
#define forv(i, v) fori(i, sz(v))
template<class T>
inline int sz(const T& c) {
	return c.size();
}

typedef vector<int> VI;

void write_plain(const VI& v) {
	cout << "plain" << endl
		 << sz(v) << endl;
	forv(i, v)
		cout << (i?" ":"") << v[i];
	cout << endl;
}

