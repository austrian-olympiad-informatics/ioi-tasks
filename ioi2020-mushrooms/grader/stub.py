#
# Judge grader for Python
# Task: mushrooms
# Author: Kian Mirjalali
#
from typing import List, Callable


def _main():
    import sys
    import os
    from typing import Optional, Final, NoReturn
    from threading import Lock


    this_module_name = os.path.splitext(os.path.basename(__file__))[0]
    # Preventing this file to be loaded again.
    this_module = sys.modules[this_module_name] = sys.modules[__name__]


    def synchronized(func):
        func.__lock__ = Lock()
        def synced_func(*args, **kwargs):
            with func.__lock__:
                return func(*args, **kwargs)
        return synced_func


    class InputReader:

        def __init__(self, input_stream):
            self._stream = input_stream
            self._eof: bool = False

        def close(self) -> None:
            try:
                self._stream.close()
            except IOError:
                pass

        def __enter__(self):
            return self

        def __exit__(self, exc_type, exc_value, exc_traceback):
            self.close()

        def eof(self) -> bool:
            return self._eof

        def read(self) -> Optional[str]:
            if self._eof:
                return None
            c = self._stream.read(1)
            if c:
                return c
            self._eof = True
            return None

        def _eat_white(self) -> Optional[str]:
            c: Optional[str] = self.read()
            while c is not None and c.isspace():
                c = self.read()
            return c

        def read_token(self) -> Optional[str]:
            c: Optional[str] = self._eat_white()
            if c is None:
                return None
            tok: str = ""
            while c is not None and not c.isspace():
                tok += c
                c = self.read()
            return tok

        def read_int(self) -> Optional[int]:
            tok: Optional[str] = self.read_token()
            return int(tok) if tok is not None else None

        def read_line(self) -> str:
            line: str = ""
            while True:
                c: Optional[str] = self.read()
                if c is None or c in '\n\r':
                    break
                line += c
            if c == '\r':
                self.read() # Consume '\n'
            return line


    n: int
    fifo_in: InputReader = None
    fifo_out = None

    def die(exit_code: int = 0, message: str = None) -> NoReturn:
        if message is not None:
            sys.stderr.write("%s\n" % message)
        if fifo_in:
            fifo_in.close()
        if fifo_out:
            fifo_out.close()
        sys.exit(exit_code)

    def read_fifo_integer(name: str) -> int:
        try:
            value: Optional[int] = fifo_in.read_int()
            if value is None:
                raise ValueError
            return value
        except (ValueError, IOError):
            die(3, "Grader error: Could not read %s." % name)

    def wrong_if(cond: bool, message: str) -> None:
        if cond:
            fifo_out.write("W %s\n" % message)
            fifo_out.flush()
            die()

    @synchronized
    def local_use_machine(x: List[int]) -> int:
        k: Final[int] = len(x)
        wrong_if(k > n, "Too large array for query.")
        fifo_out.write("Q %d" % k)
        for i in range(k):
            fifo_out.write(" %d" % x[i])
        fifo_out.write("\n")
        fifo_out.flush()
        response: int = read_fifo_integer("query response")
        if response < 0: # 'Die' sent from manager
            die()
        return response

    this_module.use_machine = local_use_machine
    sys.stdin.close()
    sys.stdout.close()
    if len(sys.argv) < 3:
        die(1, "Grader error: Invalid number of arguments: %d" % (len(sys.argv)-1,))
    try:
        fifo_in = InputReader(open(sys.argv[1], 'r'))
    except OSError:
        die(2, "Grader error: Could not open file '%s' for reading." % (sys.argv[1],))
    try:
        fifo_out = open(sys.argv[2], 'a')
    except OSError:
        die(2, "Grader error: Could not open file '%s' for writing." % (sys.argv[2],))

    n = read_fifo_integer("'n'")
    from mushrooms import count_mushrooms
    answer: int = count_mushrooms(n)
    fifo_out.write("A %d\n" % answer)
    fifo_out.flush()
    die()


use_machine: Callable[[List[int]], int]

__all__ = ["use_machine"]


if __name__ == "__main__":
    _main() # All definitions kept private in the function
