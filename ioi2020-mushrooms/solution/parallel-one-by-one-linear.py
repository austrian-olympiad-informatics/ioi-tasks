#
# A parallel implementation of the one-by-one solution (creating threads linearly).
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
    threads: List[Thread] = []
    def action(begin: int, end: int):
        for i in range(begin, end):
            species[i] = use_machine([0, i])
    for begin in range(1, n, block_size):
        end: int = min(n, begin+block_size)
        threads.append(Thread(target=action, args=(begin, end)))
    for th in threads:
        th.start()
    for th in threads:
        th.join()
    return species.count(0)
