#
# Solution with two mixed phases of dynamic size
# By "mixed" we mean that new types are also detected in the second phase.
# By "dynamic" we mean that #queries in the first phase is computed dynamically based on the current results.
#
# Author: Kian Mirjalali
#
from typing import Final, NewType, List, Callable
import math
from stub import use_machine


def largest_element_index(v: List) -> int:
    return max(range(len(v)), key=lambda i : len(v[i]))


def ceildiv(a: int, b: int) -> int:
    """returns ceil(a/b) for integers a, b"""
    return (a+b-1)//b


def function_min_input(f: Callable[[int], float], begin: int, end: int) -> int:
    """Finds minimum input of a function in range [begin, end).
    @param f a function : Integer -> Real
    @param begin
    @param end
    @return Integer input which minimizes f
    """
    best: int = begin
    fbest: float = f(best)
    for i in range(begin+1, end):
        fi: float = f(i)
        if fbest > fi:
            best = i
            fbest = fi
    return best


SpeciesType = NewType('SpeciesType', int)

def to_species_type(i: int) -> SpeciesType:
    assert 0 <= i < 2
    return SpeciesType(i)

TYPE_0: Final[SpeciesType] = to_species_type(0)
TYPE_1: Final[SpeciesType] = to_species_type(1)


Index = NewType('Index', int)
Indices = List[Index]


class SpeciesTypes:
    def __init__(self):
        self._types_indices: List[Indices] = 2 * [None]
        for i in range(len(self._types_indices)):
            self._types_indices[i] = []

    def add(self, type_: SpeciesType, index: Index) -> None:
        self._types_indices[type_].append(index)

    def get_indices(self, type_: SpeciesType) -> Indices:
        return self._types_indices[type_]

    def get_largest_type(self) -> SpeciesType:
        return to_species_type(largest_element_index(self._types_indices))


class SpeciesCounts:
    def __init__(self):
        self._counts: List[int] = [0, 0]

    def add(self, type_: SpeciesType, count: int = 1, otherCount: int = 0) -> None:
        self._counts[type_] += count
        self._counts[1-type_] += otherCount

    def get(self, type_: SpeciesType) -> int:
        return self._counts[type_]


def getType(i: Index) -> SpeciesType:
    return to_species_type(use_machine([0, i]))


a: Index
b: Index
ta: SpeciesType

def getTypes(i: Index, j: Index) -> List[SpeciesType]:
    d: Final[int] = use_machine([a, i, b, j])
    return [to_species_type((d>>1)^ta), to_species_type((d&1)^ta)]


def count_mushrooms(n: int) -> int:
    known_species_types: Final[SpeciesTypes] = SpeciesTypes()
    head = Index(0)
    if n >= 1:
        known_species_types.add(TYPE_0, head)
        head = Index(head+1)
        if n >= 2:
            t1: Final[SpeciesType] = getType(head)
            known_species_types.add(t1, head)
            head = Index(head+1)
            global a, b, ta
            if n == 2 or t1 == TYPE_0:
                a = Index(0)
                b = Index(1)
                ta = TYPE_0
            else: # n >= 3 && t1 == TYPE_1
                t2: Final[SpeciesType] = getType(head)
                known_species_types.add(t2, head)
                head = Index(head+1)
                if t2 == TYPE_0:
                    a = Index(0)
                    b = Index(2)
                    ta = TYPE_0
                else: # t2 == TYPE_1
                    a = Index(1)
                    b = Index(2)
                    ta = TYPE_1

    maxk: Final[int] = int(2*math.sqrt(n))
    while head+2 <= n:
        def _phase1_step():
            nonlocal head
            maj_type: Final[SpeciesType] = known_species_types.get_largest_type()
            maj_size: Final[int] = len(known_species_types.get_indices(maj_type))
            def rem_queries_func(k: int) -> float:
                l: int = 0
                s: int = n-head-2*k
                while s > 0:
                    s -= max(maj_size, k+ceildiv(head+l, 2))
                    l += 1
                return k + l
            k: Final[int] = function_min_input(rem_queries_func, 0, maxk)
            if k == 0:
                return True
            p: Final[List[SpeciesType]] = getTypes(head, Index(head+1))
            known_species_types.add(p[0], head)
            known_species_types.add(p[1], Index(head+1))
            head = Index(head+2)
            return False
        if _phase1_step():
            break

    species_counts: Final[SpeciesCounts] = SpeciesCounts()
    while head < n:
        def _phase2_step():
            nonlocal head
            maj_type: Final[SpeciesType] = known_species_types.get_largest_type()
            maj_indices: Final[Indices] = known_species_types.get_indices(maj_type)
            maj_size: Final[int] = len(maj_indices)
            blockSize: Final[int] = min(maj_size, n-head)
            v: Final[List[int]] = (2*blockSize) * [0]
            i: int = 0
            j: int = 0
            while i < blockSize:
                v[j] = head
                j += 1
                head = Index(head+1)
                v[j] = maj_indices[i]
                j += 1
                i += 1
            differences: Final[int] = use_machine(v)
            known_species_types.add(to_species_type((differences&1)^maj_type), Index(v[0]))
            differents: Final[int] = (differences>>1)
            same: Final[int] = blockSize-1-differents
            species_counts.add(maj_type, same, differents)
        _phase2_step()

    return len(known_species_types.get_indices(TYPE_0))+species_counts.get(TYPE_0)
