/*
 * Two-by-one solution, computing the types for all indices
 *
 * Author: Kian Mirjalali
 */
#include "mushrooms.h"
using namespace std;

typedef int Index;
typedef int SpeciesType;
const SpeciesType TYPE_0 = 0;
const SpeciesType TYPE_1 = 1;

typedef vector<Index> Indices;
class SpeciesTypes {
	vector<Indices> typesIndices;
public:
	inline SpeciesTypes(): typesIndices(2) {
	}
	inline void add(SpeciesType type, Index index) {
		typesIndices[type].push_back(index);
	}
	inline const Indices& getIndices(SpeciesType type) const {
		return typesIndices[type];
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
	SpeciesTypes knownSpeciesTypes;
	Index head = 0;
	if (n >= 1) {
		knownSpeciesTypes.add(TYPE_0, head);
		head++;
		if (n >= 2) {
			const SpeciesType t1 = getType(head);
			knownSpeciesTypes.add(t1, head);
			head++;
			if (n == 2 || t1 == TYPE_0) {
				a = 0;
				b = 1;
				ta = TYPE_0;
			} else {//n >= 3 && t1 == TYPE_1
				const SpeciesType t2 = getType(head);
				knownSpeciesTypes.add(t2, head);
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
		knownSpeciesTypes.add(p[0], head);
		knownSpeciesTypes.add(p[1], head+1);
		head += 2;
	}
	// n-2 < head <= n
	if (head < n) { // head == n-1
		const SpeciesType t = getType(head);
		knownSpeciesTypes.add(t, head);
		head++;
	}
	// head == n
	return knownSpeciesTypes.getIndices(TYPE_0).size();
}
