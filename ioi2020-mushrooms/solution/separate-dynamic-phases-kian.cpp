/*
 * Solution with two separate phases of dynamic size
 * By "separate" we mean that species types are only detected in the first phase.
 * By "dynamic" we mean that #queries in the first phase is computed dynamically based on the current results.
 *
 * Author: Kian Mirjalali
 */
#include "mushrooms.h"
#include <cmath>
#include <algorithm>
#include <functional>
using namespace std;

#define allOf(c) ((c).begin()), ((c).end())

template<class C>
inline int largest_element_index(const vector<C>& v) {
	return max_element(allOf(v), [](const C& c1, const C& c2) {return c1.size()<c2.size();}) - v.begin();
}

/**
 * @returns ceil(a/b) for integers a, b
 */
template<class T>
inline T ceildiv(T a, T b) {
	return (a+b-1)/b;
}

/**
 * Finds minimum input of a function in range [begin, end).
 * @param f a function : Integer -> Real
 * @param begin
 * @param end
 * @returns Integer input which minimizes f
 */
inline int functionMinInput(function<double(int)> f, int begin, int end) {
	int best = begin;
	double fbest = f(best);
	for (int i = begin+1; i < end; i++) {
		const double fi = f(i);
		if (fbest > fi) {
			best = i;
			fbest = fi;
		}
	}
	return best;
}

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
	inline SpeciesType getLargestType() const {
		return largest_element_index(typesIndices);
	}
};

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

	const int maxk = int(2*sqrt(n));
	while (head+2 <= n) {
		const SpeciesType majType = knownSpeciesTypes.getLargestType();
		const int majSize = knownSpeciesTypes.getIndices(majType).size();
		const auto rem_queries_func = [n, head, majSize] (int k) -> double {
			return k + ceildiv(n-head-2*k, max(majSize, k+ceildiv(head, 2)));
		};
		const int k = functionMinInput(rem_queries_func, 0, maxk);
		if (k == 0)
			break;
		const auto p = getTypes(head, head+1);
		knownSpeciesTypes.add(p[0], head);
		knownSpeciesTypes.add(p[1], head+1);
		head += 2;
	}

	SpeciesCounts speciesCounts;
	const SpeciesType majType = knownSpeciesTypes.getLargestType();
	const Indices& majIndices = knownSpeciesTypes.getIndices(majType);
	const int majSize = majIndices.size();
	while (head < n) {
		const int blockSize = min(majSize, n-head);
		Indices v(2*blockSize);
		for (int i = 0, j = 0; i < blockSize;) {
			v[j++] = head++;
			v[j++] = majIndices[i++];
		}
		const int differences = use_machine(v);
		const int differents = (differences&1)+(differences>>1);
		const int same = blockSize-differents;
		speciesCounts.add(majType, same, differents);
	}
	return knownSpeciesTypes.getIndices(TYPE_0).size()+speciesCounts.get(TYPE_0);
}
