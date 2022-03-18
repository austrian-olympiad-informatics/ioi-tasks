/*
 * Two-by-one solution, just computing the counts
 *
 * Author: Kian Mirjalali
 */
#include "mushrooms.h"
using namespace std;

typedef int Index;
typedef int SpeciesType;
const SpeciesType TYPE_0 = 0;
const SpeciesType TYPE_1 = 1;

class SpeciesCounts {
	vector<int> counts;
public:
	inline SpeciesCounts(): counts({0, 0}) {
	}
	inline void add(SpeciesType type, int count=1, int otherCount=0) {
		counts[type] += count;
		counts[1-type] += otherCount;
	}
	inline int get(SpeciesType type) const {
		return counts[type];
	}
};

inline SpeciesType getType(Index i) {
	return use_machine({0, i});
}

Index a, b;
SpeciesType ta;
inline vector<SpeciesType> getTypes(Index i, Index j) {
	const int d = use_machine({a, i, b, j});
	return {(d>>1)^ta, (d&1)^ta};
}

int count_mushrooms(int n) {
	SpeciesCounts speciesCounts;
	Index head = 0;
	if (n >= 1) {
		speciesCounts.add(TYPE_0);
		head++;
		if (n >= 2) {
			const SpeciesType t1 = getType(head);
			speciesCounts.add(t1);
			head++;
			if (n == 2 || t1 == TYPE_0) {
				a = 0;
				b = 1;
				ta = TYPE_0;
			} else {//n >= 3 && t1 == TYPE_1
				const SpeciesType t2 = getType(head);
				speciesCounts.add(t2);
				head++;
				if (t2 == TYPE_0) {
					a = 0;
					b = 2;
					ta = TYPE_0;
				} else {//t2 == TYPE_1
					a = 1;
					b = 2;
					ta = TYPE_1;
				}
			}
		}
	}

	// head <= n
	while (head+2 <= n) {
		const auto p = getTypes(head, head+1);
		speciesCounts.add(p[0]);
		speciesCounts.add(p[1]);
		head += 2;
	}
	// n-2 < head <= n
	if (head < n) { // head == n-1
		const SpeciesType t = getType(head);
		speciesCounts.add(t);
		head++;
	}
	// head == n
	return speciesCounts.get(TYPE_0);
}
