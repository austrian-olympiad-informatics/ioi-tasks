#!/usr/bin/env python3

"""
Example usage (with H=4, W=5, and the hidden box at position r=2, c=3):

For python programs, say "solution.py" (normally run as "pypy3 solution.py"):

    python3 testing_tool.py python3 solution.py <<<"4 5 2 3"

For C++ programs, first compile it
(e.g. with "g++ -std=gnu++17 solution.cpp -o solution.out")
and then run

    python3 testing_tool.py ./solution.out <<<"4 5 2 3"

"""

import sys
import os
import signal
from typing import List, Tuple

F_SETPIPE_SZ = 1031


def error(msg: str) -> None:
    print("ERROR:", msg, file=sys.stderr)
    sys.exit(1)

def wait_for_child(pid: int) -> None:
    pid, status = os.waitpid(pid, 0)
    if os.WIFSIGNALED(status):
        sig = os.WTERMSIG(status)
        error(f"Program terminated with signal {sig} ({signal.Signals(sig).name})")
    ex = os.WEXITSTATUS(status)
    if ex != 0:
        error(f"Program terminated with exit code {ex}")

def read_line(pid: int, file, what: str) -> str:
    line = file.readline()
    if not line:
        wait_for_child(pid)
        error(f"Failed to read {what}: no more output")
    return line.rstrip("\r\n")

def write_line(file, line: str) -> None:
    try:
        file.write(line + "\n")
        file.flush()
    except BrokenPipeError:
        pass

def run_solution(submission: List[str], H: int, W: int, r: int, c: int, silent: bool) -> Tuple[int, int, int]:

    c2p_read, c2p_write = os.pipe()
    p2c_read, p2c_write = os.pipe()
    pid = os.fork()

    if pid == 0:
        os.close(p2c_write)
        os.close(c2p_read)

        os.dup2(p2c_read, 0)
        os.dup2(c2p_write, 1)

        try:
            os.execvp(submission[0], submission)
        except Exception as e:
            error(f"Failed to execute program: {e}")
        assert False, "unreachable"
    else:
        os.close(c2p_write)
        os.close(p2c_read)

        with os.fdopen(p2c_write, "w") as fout:
            with os.fdopen(c2p_read, "r") as fin:

                queries = 0
                write_line(fout, f"{H} {W}")
                while True:
                    line = read_line(pid, fin, f"query {queries+1}")
                    tokens = line.split();
                    if tokens[0] == '!':
                        if len(tokens) != 3:
                            error(f"Expected answer in format \"! r c\"; got \"{line}\"")
                        if not silent:
                            print(f"[*] Guess: \"{line}\"")

                        try:
                            guess_r = int(tokens[1])
                            guess_c = int(tokens[2])
                        except Exception as e:
                            error(f"Failed to parse answer line \"{line}\": {e}")

                        # Wait for program to terminate, and read all its output
                        remainder = fin.read()
                        if remainder.strip():
                            error(f"Unexpected trailing output: {remainder}")
                        wait_for_child(pid)

                        return guess_r, guess_c, queries


                    elif tokens[0] == '?':
                        queries = queries + 1
                        if len(tokens) != 2:
                            error(f"Expected query in format \"? s\"; got \"{line}\"")

                        if not silent:
                            print(f"[*] Query #{queries}: \"{line}\"")
                        if queries > 1000: 
                            error(f"Too many queries")

                        instructions = tokens[1]
                        if len(instructions) > 20000: 
                            error(f"Too long string of instructions")

                        x, y = 0, 0
                        for s in instructions:
                            if s == 'v':
                                if x+1 != H and (x+1,y) != (r,c):
                                    x = x+1
                            elif s == '^':
                                if x and (x-1,y) != (r,c):
                                    x = x-1
                            elif s == '>':
                                if y+1 != W and (x,y+1) != (r,c):
                                    y = y+1
                            elif s == '<':
                                if y and (x,y-1) != (r,c):
                                    y = y-1
                            else:
                                error(f"Expected direction (<^v>), got \"{s}\"")

                        if not silent:
                            print(f"[*] -> ({x}, {y})")

                        write_line(fout, f"{x} {y}")

                    else:
                        error(f"Expected \"!\" or \"?\", got \"{line}\"")


def main() -> None:
    silent = False
    args = sys.argv[1:]
    if args and args[0] == "--silent":
        args = args[1:]
        silent = True
    if not args:
        print("Usage:", sys.argv[0], '[--silent] program... <<<"H W r c"')
        sys.exit(0)

    toks = []
    for line in sys.stdin:
        for tok in line.split():
            toks.append(int(tok))
        if len(toks) >= 4:
            break

    if len(toks) != 4:
        error("Need exactly 4 input parameters: H W r c")
    H, W, r, c = toks
    if r >= H or r < 0:
        error(f"r must be between 0 and H-1; r={r}, H={H}")
    if c >= W or c < 0:
        error(f"c must be between 0 and W-1; c={c}, W={W}")
    if r == 0 and c == 0:
        error(f"Box cannot be in top left corner (0,0)")

    if not silent:
        print(f"[*] Running solution (H = {H}, W = {W}, r = {r}, c = {c})")

    guess_r, guess_c, queries = run_solution(args, H, W, r, c, silent)

    query_text = "queries" if queries != 1 else "query"
    if guess_r == r and guess_c == c:
        print(f"[*] OK: found box at position ({r}, {c}) using {queries} {query_text}.")
    else:
        print(f"[*] Wrong Answer: guessed ({guess_r}, {guess_c}), but answer was ({r}, {c}). Used {queries} {query_text}.")

if __name__ == "__main__":
    main()

