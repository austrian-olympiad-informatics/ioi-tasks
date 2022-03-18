#include <algorithm>
#include <cmath>
#include <cstdio>
#include <map>
#include <vector>
#include <set>
#include <random>
#include <csignal>
#include <cstdarg>

using namespace std;

/******************************** Begin testlib-similar material ********************************/

#ifdef _MSC_VER
#   define NORETURN __declspec(noreturn)
#elif defined __GNUC__
#   define NORETURN __attribute__ ((noreturn))
#else
#   define NORETURN
#endif


static char __testlib_format_buffer[16777216];

#define FMT_TO_RESULT(fmt, cstr, result)  std::string result;                              \
            va_list ap;                                                                    \
            va_start(ap, fmt);                                                             \
            vsnprintf(__testlib_format_buffer, sizeof(__testlib_format_buffer), cstr, ap); \
            va_end(ap);                                                                    \
            __testlib_format_buffer[sizeof(__testlib_format_buffer) - 1] = 0;              \
            result = std::string(__testlib_format_buffer);                                 \


typedef string TResult;
const TResult _ok = "Correct";
const TResult _wa = "Wrong Answer";
const TResult _fail = "Judge Failure; Contact staff!";
const TResult _points = "Partially Correct";

NORETURN void quit(TResult result, string message = "") {
	fprintf(stdout, "%d\n", ((result==_ok) ? 1 : 0));
	fprintf(stderr, "%s\n", result.c_str());
	if (!message.empty())
		fprintf(stderr, "%s\n", message.c_str());
	exit(0);
}

NORETURN void quitp(double points, string message = "") {
	fprintf(stdout, "%.12lf\n", points);
	fprintf(stderr, "%s\n", _points.c_str());
	if (!message.empty())
		fprintf(stderr, "%s\n", message.c_str());
	exit(0);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void quitf(TResult result, const char* fmt, ...) {
	FMT_TO_RESULT(fmt, fmt, message);
	quit(result, message);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void quitpf(double points, const char* fmt, ...) {
	FMT_TO_RESULT(fmt, fmt, message);
	quitp(points, message);
}

/********************************* End testlib-similar material *********************************/


const int MAXR = 10;
const int MAXN = 1000;
const int MAXQ = 100000;

FILE *grader1in,*grader1out, *grader2in, *grader2out;
int r, maxlabel = 0, cnt = 0, is_partial, seed;
int n[MAXR], k[MAXR], u[MAXR][MAXN], v[MAXR][MAXN];
vector<int> adj[MAXR][MAXN];
int q[MAXR], s[MAXR][MAXQ], t[MAXR][MAXQ], ans[MAXR][MAXQ];
int labels[MAXR][MAXN];
map<int, int> reverse_labels[MAXR];

struct query {
	int idx, r, s, t, ans;
	vector<int>* c;
	
	query(int _idx, int _r, int _s, int _t, int _ans, vector<int>* _c) 
	: idx(_idx), r(_r), s(_s), t(_t), ans(_ans), c(_c) {}
};
vector<query> queries;

static void shuffle(vector<query> &v){
	mt19937 rng(seed);
	shuffle(v.begin(), v.end(), rng);
}


#ifdef __GNUC__
__attribute__ ((format (printf, 3, 4)))
#endif
NORETURN inline void die(TResult result, bool sendDie, const char* fmt, ...) {
	if (sendDie) {
	  fprintf(grader2out, "-1\n");
	  fflush(grader2out);
	}
	FMT_TO_RESULT(fmt, fmt, message);
	quit(result, message);
}


inline FILE* openFile(const char* name, const char* mode) {
	FILE* file = fopen(name, mode);
	if (!file) {
		quitf(_fail, "Could not open file '%s' with mode '%s'.", name, mode);
	}
	return file;
}

inline void assertInput(bool cond) {
	if (!cond) {
		quit(_fail, "Could not read input");
	}
}

int main(int argc, char **argv) {
	if (argc < 5) {
		quit(_fail, "Insufficient #args for manager of 'stations'");
	}

	{//Keep alive on broken pipes
		//signal(SIGPIPE, SIG_IGN);
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigaction(SIGPIPE, &sa, NULL);
	}

	grader1out = openFile(argv[2], "a");
	grader1in = openFile(argv[1], "r");
	grader2out = openFile(argv[4], "a");
	grader2in = openFile(argv[3], "r");
	
	// read in test input
	assertInput(scanf("%d", &r) == 1);
	for (int i = 0; i < r; i++) {
		assertInput(scanf("%d%d", &n[i], &k[i]) == 2);
		for (int j = 0; j < n[i] - 1; j++) {
			assertInput(scanf("%d%d", &u[i][j], &v[i][j]) == 2);
			adj[i][u[i][j]].push_back(v[i][j]);
			adj[i][v[i][j]].push_back(u[i][j]);
		}
		assertInput(scanf("%d", &q[i]) == 1);
		for (int j = 0; j < q[i]; j++) {
			assertInput(scanf("%d%d%d", &s[i][j], &t[i][j], &ans[i][j]) == 3);
		}
	}
	assertInput(scanf("%d%d", &is_partial, &seed) == 2);
	fclose(stdin);
	
	// send trees to first grader for relabeling
	fprintf(grader1out, "%d\n", r);
	fflush(grader1out);
	for (int i = 0; i < r; i++) {
		fprintf(grader1out, "%d %d\n", n[i], k[i]);
		for (int j = 0; j < n[i] - 1; j++) {
			fprintf(grader1out, "%d %d\n", u[i][j], v[i][j]);
		}
		fflush(grader1out);
		int m;
		if (fscanf(grader1in, "%d", &m) != 1) {
			die(_wa, true, "Could not read m from first grader (solution possibly terminated). scenario=%d", i);
		}
		if (m != n[i]) {
			die(_wa, true, "Invalid length of array as the response of 'label'. scenario=%d, n=%d, len=%d", i, n[i], m);
		}
		for(int j = 0; j < m; j++) {
			if (fscanf(grader1in, "%d", &labels[i][j]) != 1) {
				die(_fail, true, "Could not read array elements of first grader's response. scenario=%d, element #%d", i, j);
			}
		}
	}
	fclose(grader1out);
	fclose(grader1in);
	
	// check whether labels are valid then generate and shuffle queries
	for (int i = 0; i < r; i++) {
		// check labels
		for (int j = 0; j < n[i]; j++) {
			if (labels[i][j] < 0 || labels[i][j] > k[i]) {
				die(_wa, true, "Invalid labels (values out of range). scenario=%d, k=%d, vertex=%d, label=%d", i, k[i], j, labels[i][j]);
			}
			if (reverse_labels[i].find(labels[i][j]) != reverse_labels[i].end()) {
				die(_wa, true, "Invalid labels (duplicates values). scenario=%d, label=%d", i, labels[i][j]);
			}
			reverse_labels[i][labels[i][j]] = j;
			maxlabel = max(maxlabel, labels[i][j]);
		}
		
		// relabel adjacency list and sort
		for (int j = 0; j < n[i]; j++) {
			for (int& p : adj[i][j]) {
				p = labels[i][p];
			}
			sort(adj[i][j].begin(), adj[i][j].end());
		}
		
		// add query
		for (int j = 0; j < q[i]; j++) {
			queries.emplace_back(cnt++, i, labels[i][s[i][j]], labels[i][t[i][j]], 
					labels[i][ans[i][j]], &adj[i][s[i][j]]);
		}
	}
	shuffle(queries);
	
	// send queries
	bool fail = 0;
	const query* fail_q;
	int fail_chosen;
	fprintf(grader2out, "%d\n", (int)queries.size());
	fflush(grader2out);
	int q_num = -1;
	for (const query& q : queries) {
		q_num++;
		fprintf(grader2out, "%d %d %d\n", q.s, q.t, (int)q.c->size());
		for (int i : *q.c) {
			fprintf(grader2out, "%d\n", i);
		}
		fflush(grader2out);
		int chosen;
		if (fscanf(grader2in, "%d", &chosen) != 1) {
			die(_wa, false, "Could not read query response from second grader (solution possibly terminated)."
				" query #%d, index=%d, scenario=%d", q_num, q.idx, q.r);
		}
		if (chosen != q.ans) {
			fail = 1;
			fail_q = &q;
			fail_chosen = chosen;
		}
	}
	fclose(grader2in);
	fclose(grader2out);
	
	// print score
	if (fail) {
		quitf(_wa, "Wrong query response. scenario=%d, s=%d, t=%d, correct=%d, response=%d",
			fail_q->r, fail_q->s, fail_q->t, fail_q->ans, fail_chosen);
	} else if (!is_partial) { // non-partial scoring
		quit(_ok);
	} else { // partial scoring
		long double points = 0;
		if (maxlabel <= 1000) {
			quit(_ok);
		} else if (maxlabel < 2000) {
			points = 50;
		} else if (maxlabel < 1e9) {
			points = (log2l(1e9) - log2l(maxlabel)) * 50 / log2l(500000);
		} else {
			quitf(_wa, "max label too large: %d", maxlabel);
		}
		points /= 61;
		quitpf(points, "max label = %d", maxlabel);
	}
	return 0;
}
