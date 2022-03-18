#
# Trivial one-by-one solution
#
# Author: Kian Mirjalali
#
from stub import use_machine

def count_mushrooms(n: int) -> int:
    s = 1
    for i in range(1, n):
        s += 1 - use_machine([0, i])
    return s
