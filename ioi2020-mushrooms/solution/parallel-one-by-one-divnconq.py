#
# A parallel implementation of the one-by-one solution (creating threads through divide and conquer).
# The purpose of the parallel solutions is to verify if the graders have the correct behavior
#   when contestants use parallelism in their solutions.
#
# Author: Kian Mirjalali
#
from typing import Final, List
from threading import Thread
from stub import use_machine

def count_mushrooms(n: int) -> int:
    species: Final[List[int]] = n * [0]
    max_threads: Final[int] = 200
    block_size: Final[int] = max(1, n//max_threads)
    def handle_range(begin: int, end: int):
        l: Final[int] = end - begin
        if l <= 0:
            return
        if l <= block_size:
            for i in range(begin, end):
                species[i] = use_machine([0, i])
            return
        # l >= 2
        mid: Final[int] = begin + l//2
        th: Thread = Thread(target=handle_range, args=(begin, mid))
        th.start()
        handle_range(mid, end)
        th.join()
    handle_range(1, n)
    return species.count(0)
