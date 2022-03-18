/*
 * Judge grader for C++
 * Task: mushrooms
 * Author: Kian Mirjalali
 */
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include <mutex>
#include "mushrooms.h"
using namespace std;

#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#elif defined __GNUC__
#define NORETURN __attribute__ ((noreturn))
#else
#define NORETURN
#endif

static char fmt_buffer[100000];
#define FMT_TO_STR(fmt, result) va_list vargs; va_start(vargs, fmt); \
	vsnprintf(fmt_buffer, sizeof(fmt_buffer), fmt, vargs); \
	va_end(vargs); fmt_buffer[sizeof(fmt_buffer)-1] = 0; \
	std::string result(fmt_buffer);

static int n;
static FILE* fifo_in = NULL;
static FILE* fifo_out = NULL;

#ifdef __GNUC__
__attribute__ ((format(printf, 2, 3)))
#endif
NORETURN static inline void die(int exit_code=0, const char* message_fmt=NULL, ...) {
	if (message_fmt) {
		FMT_TO_STR(message_fmt, message);
		fprintf(stderr, "%s\n", message.c_str());
	}
	if (fifo_in)
		fclose(fifo_in);
	if (fifo_out)
		fclose(fifo_out);
	exit(exit_code);
}

static inline int readFifoInteger(string name) {
	int x;
	if (1 != fscanf(fifo_in, "%d", &x))
		die(3, "Grader error: Could not read %s.", name.c_str());
	return x;
}

static inline void wrong_if(bool cond, string message) {
	if (cond) {
		fprintf(fifo_out, "W %s\n", message.c_str());
		fflush(fifo_out);
		die();
	}
}

int use_machine(vector<int> x) {
	#ifdef _GLIBCXX_HAS_GTHREADS
	static mutex _mutex;
	lock_guard<mutex> lock(_mutex);
	#endif
	const int k = x.size();
	wrong_if(k > n, "Too large array for query.");
	fprintf(fifo_out, "Q %d", k);
	for (int i = 0; i < k; i++)
		fprintf(fifo_out, " %d", x[i]);
	fprintf(fifo_out, "\n");
	fflush(fifo_out);
	int response = readFifoInteger("query response");
	if (response < 0) // 'Die' sent from manager
		die();
	return response;
}

int main(int argc, char **argv) {
	fclose(stdin);
	fclose(stdout);
	if (argc < 3)
		die(1, "Grader error: Invalid number of arguments: %d", argc-1);
	fifo_in = fopen(argv[1], "r");
	if (!fifo_in)
		die(2, "Grader error: Could not open file '%s' for reading.", argv[1]);
	fifo_out = fopen(argv[2], "a");
	if (!fifo_out)
		die(2, "Grader error: Could not open file '%s' for writing.", argv[2]);

	n = readFifoInteger("'n'");
	int answer = count_mushrooms(n);
	fprintf(fifo_out, "A %d\n", answer);
	fflush(fifo_out);
	die();
}
