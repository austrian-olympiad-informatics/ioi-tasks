/*
 * Judge manager (interacts with graders)
 * Task: mushrooms
 * The manager itself produces the judgement results (including the scores).
 * Author: Kian Mirjalali
 */
#include "testlib.h"
#include <csignal>
#include <memory>
using namespace std;



/*********************** Begin helper definitions for compiling in Windows ***********************/
#ifdef ON_WINDOWS
typedef __p_sig_fn_t __sighandler_t;
#define SIGPIPE 13
struct sigaction {
	__sighandler_t sa_handler;
};
int sigaction(int, const struct sigaction *__restrict, struct sigaction *__restrict) {
	return 0;
}
#endif
/************************ End helper definitions for compiling in Windows ************************/


/****************************** Begin FactoryRegistry and tools for self-registering classes ******************************/
/**
 * A FactoryRegistry<T> can create instances of (sub)type T.
 * The exact type for instantiation is specified by a (string) key.
 */
template<class T>
struct FactoryRegistry {
	typedef T InstanceType;
	typedef unique_ptr<InstanceType> InstancePtr;
	typedef InstancePtr (*FactoryFunction)();
private:
	map<string, FactoryFunction> factories;

public:

	inline void registerFactory(string name, FactoryFunction factoryFunc) {
		factories[name] = factoryFunc;
	}

	template <class C, char const *registrationName, FactoryRegistry<InstanceType>& factoryRegistry>
	class SelfRegisteringInstanceMixin {
		struct _RegisteringObject {
			inline _RegisteringObject() {
				FactoryFunction factoryFunc = []() -> InstancePtr {
					return make_unique<C>();
				};
				factoryRegistry.registerFactory(registrationName, factoryFunc);
			}
		};
		static _RegisteringObject _registeringObject;
		// will force instantiation of definition of static member
		template<_RegisteringObject&> struct _RegisteringObjectReferrer {};
		static _RegisteringObjectReferrer<_registeringObject> _registeringObjectReferrer;
	};

	inline InstancePtr create(string name) {
		auto it = factories.find(name);
		if (it == factories.end())
			return nullptr;
		FactoryFunction factory = it->second;
		InstancePtr instance = factory();
		return instance;
	}
};

template<class T> template<class C, char const *registrationName, FactoryRegistry<T>& factoryRegistry>
	typename FactoryRegistry<T>::template SelfRegisteringInstanceMixin<C, registrationName, factoryRegistry>::_RegisteringObject
	FactoryRegistry<T>::SelfRegisteringInstanceMixin<C, registrationName, factoryRegistry>::_registeringObject;

#define __REGISTRATION_NAME_FOR__(className) __REGISTRATION_NAME_FOR__##className##__

#define SELF_REGISTERED(structOrClass, className, registrationName, selfRegisteringInstanceMixinClass) \
	char __REGISTRATION_NAME_FOR__(className)[] = (registrationName); \
	structOrClass className: selfRegisteringInstanceMixinClass<className, __REGISTRATION_NAME_FOR__(className)>

#define DEFINE_SELF_REGISTERING_INSTANCE_MIXIN(mixinName, factoryRegistry) \
template<class C, char const *registrationName> using mixinName = \
	FactoryRegistry<decltype(factoryRegistry)::InstanceType>::SelfRegisteringInstanceMixin<C, registrationName, factoryRegistry>

/******************************* End FactoryRegistry and tools for self-registering classes *******************************/


/****************************** Begin template/utility codes ******************************/
#define fori(i, n) for (int i=0; (i<(n)); i++)
#define forv(i, v) fori(i, sz(v))
#define forall(x, c) for (auto& x: c)
#define allOf(c) ((c).begin()), ((c).end())


typedef vector<int> VI;

template<class T>
inline T sqr(const T& x) {
	return x*x;
}

template<class T>
inline int sz(const T& c) {
	return c.size();
}

/**
 * Reads a single word from a FILE
 * @param s The variable to store the word on success
 * @return true on success
 */
inline bool readStr(FILE* f, string& s) {
	static char buff[1000000];
	if (1 != fscanf(f, "%999999s", buff))
		return false;
	s = buff;
	return true;
}

/**
 * Reads a single line from a FILE
 * New line characters in the end of the line (if any) are trimmed.
 * @param s The variable to store the line on success
 * @return true on success
 */
inline bool readLine(FILE* f, string& s) {
	const int max_len = 1000000;
	static char buff[max_len];
	if (!fgets(buff, max_len, f))
		return false;
	buff[strcspn(buff, "\r\n")] = 0; //Remove trailing newline characters
	s = buff;
	return true;
}

inline string vec2str(const VI& v) {
	string s;
	forv(i, v)
		s += format((i?" %d":"%d"), v[i]);
	return s;
}
#define vec2cstr(v) ((vec2str(v)).c_str())

template<class C, class T>
inline int count(const C& c, T value) {
	return count(allOf(c), value);
}

/******************************* End template/utility codes *******************************/


const int TYPE_0 = 0;
const int TYPE_1 = 1;
const int N_TYPES = 2;

int n;

inline int count_diffs(const VI& species, const VI& index) {
	int diffs = 0;
	for (int i = 1; i < sz(index); i++)
		diffs += int(species[index[i]] != species[index[i-1]]);
	return diffs;
}

class ManagerStrategy {
public:
	virtual void read_input(InStream& inStream) = 0;
	virtual void describe_in_one_line(FILE* file) const = 0;
	virtual int ask(const VI& x) = 0;
	virtual bool checkAnswer(int answer) = 0;
	virtual VI get_species() = 0;
	virtual ~ManagerStrategy() = default;
};


FactoryRegistry<ManagerStrategy> strategyFactoryRegistry;
DEFINE_SELF_REGISTERING_INSTANCE_MIXIN(SelfRegisteringStrategyMixin, strategyFactoryRegistry);
#define REGISTERED_STRATEGY(className, registrationName, parentClass) \
	SELF_REGISTERED(struct, className, registrationName, SelfRegisteringStrategyMixin), public parentClass


/******************************** Begin different implementations for manager strategies ********************************/

/// Reads the types thoroughly from the input
REGISTERED_STRATEGY(ManagerStrategy_Plain, "plain", ManagerStrategy) {
	VI species;
public:
	virtual void read_input(InStream& inStream) {
		n = inStream.readInt();
		species = inStream.readInts(n, TYPE_0, N_TYPES-1, "species[i]", 0);
	}
	virtual void describe_in_one_line(FILE* file) const {
		fprintf(file, "%d %s", n, vec2cstr(species));
	}
	virtual int ask(const VI& x) {
		return count_diffs(species, x);
	}
	virtual bool checkAnswer(int answer) {
		return answer == count(species, TYPE_0);
	}
	virtual VI get_species() {
		return species;
	}
};


/// Descendants of this strategy keep the types partially specified and lazily set their values (on demand).
class ManagerStrategyWithPartiallySpecifiedTypes: public ManagerStrategy {
protected:
	static const int TYPE_UNSPECIFIED = -1;
	inline bool is_unspecified(int index) {
		return species[index] == TYPE_UNSPECIFIED;
	}
	inline bool is_specified(int index) {
		return species[index] != TYPE_UNSPECIFIED;
	}

	VI species;

	inline void init_species() {
		species = VI(n, TYPE_UNSPECIFIED);
		species[0] = TYPE_0;
	}

public:
	virtual bool checkAnswer(int answer) {
		int scn = count(species, TYPE_UNSPECIFIED);
		int sc0 = count(species, TYPE_0);
		if (scn == 0)
			return answer == sc0;
		int t = (sc0 < answer) ? TYPE_1 : TYPE_0;
		replace(allOf(species), TYPE_UNSPECIFIED, t);
		return false;
	}
	virtual VI get_species() {
		replace(allOf(species), TYPE_UNSPECIFIED, TYPE_0);
		return species;
	}
};


/// Sets the types to random values, lazily (on demand).
REGISTERED_STRATEGY(ManagerStrategy_Adversary_SimpleRandom, "adversary_simple_random", ManagerStrategyWithPartiallySpecifiedTypes) {
	int seed;
public:
	virtual void read_input(InStream& inStream) {
		n = inStream.readInt();
		init_species();
		seed = inStream.readInt();
		rnd.setSeed(seed);
	}
	virtual void describe_in_one_line(FILE* file) const {
		fprintf(file, "%d %d", n, seed);
	}
	virtual int ask(const VI& x) {
		forall(xi, x)
			if (is_unspecified(xi))
				species[xi] = rnd.next(N_TYPES);
		return count_diffs(species, x);
	}
};


/// Balanced Random with p * n type A species
REGISTERED_STRATEGY(ManagerStrategy_Adversary_BalancedRandom, "adversary_balanced_random", ManagerStrategyWithPartiallySpecifiedTypes) {
	int seed;
	double p;
	VI species_stack;
public:
	virtual void read_input(InStream& inStream) {
		n = inStream.readInt();
		init_species();
		p = inStream.readDouble(0, 1, "p");
		seed = inStream.readInt();
		rnd.setSeed(seed);

		species_stack = VI(n-1, TYPE_1);
		int zeros = int(p * (n-1));
		fill_n(species_stack.begin(), zeros, TYPE_0);
		shuffle(allOf(species_stack));
	}
	virtual void describe_in_one_line(FILE* file) const {
		fprintf(file, "%d %f %d", n, p, seed);
	}
	virtual int ask(const VI& x) {
		forall(xi, x)
			if (is_unspecified(xi)) {
				species[xi] = species_stack.back();
				species_stack.pop_back();
			}
		return count_diffs(species, x);
	}
};


/**
 * Two types of queries are considered: small and large.
 * A query is considered large if number of its unspecified elements exceeds a threshold.
 * We assume that the solution can detect the element types in small queries.
 * The strategy tries to equalize the known types in small queries and act random in large queries.
 */
REGISTERED_STRATEGY(ManagerStrategy_Adversary_SmallEqual, "adversary_small_equal", ManagerStrategyWithPartiallySpecifiedTypes) {
	int seed;
	/// The threshold for small vs. large queries.
	int qthreshold;
	/**
	 * Specifies the species type equalizing strategy:
	 * true: The strategy tries to equalize the known types for all of the elements with specified species.
	 * false: The strategy tries to equalize the known types for only the elements that the solution has (possibly) detected.
	 */
	bool total_equalizing;
	int cnt[N_TYPES];
public:
	virtual void read_input(InStream& inStream) {
		n = inStream.readInt();
		init_species();
		qthreshold = inStream.readInt(0, n, "qthreshold");
		total_equalizing = inStream.readInt(0, 1, "total_equalizing") == 1;
		seed = inStream.readInt();
		rnd.setSeed(seed);
		cnt[TYPE_0] = 1;
		cnt[TYPE_1] = 0;
	}
	virtual void describe_in_one_line(FILE* file) const {
		fprintf(file, "%d %d %d %d", n, qthreshold, total_equalizing, seed);
	}
	virtual int ask(const VI& x) {
		vector<int> ultimate_unspecifieds;
		for (int i = 1; i < sz(x)-1; i++)
			if (is_unspecified(x[i])
					&& is_specified(x[i-1])
					&& is_specified(x[i+1])
					&& species[x[i-1]] != species[x[i+1]]) {
				ultimate_unspecifieds.push_back(x[i]);
				species[x[i]] = TYPE_0;
			}

		int unspecified = count_if(allOf(x), [&] (int xi) {return is_unspecified(xi);});
		bool small_query = (unspecified <= qthreshold);

		forall(xi, x)
			if (is_unspecified(xi)) {
				species[xi] = small_query ? (cnt[TYPE_0] < cnt[TYPE_1] ? TYPE_0 : TYPE_1) : rnd.next(N_TYPES);
				if (small_query || total_equalizing)
					cnt[species[xi]]++;
			}

		int ret = count_diffs(species, x);

		forall(xi, ultimate_unspecifieds)
			species[xi] = TYPE_UNSPECIFIED;

		return ret;
	}
};
/********************************* End different implementations for manager strategies *********************************/


/******************************** Begin testlib-related material ********************************/

inline FILE* openFile(const char* name, const char* mode) {
	FILE* file = fopen(name, mode);
	if (!file)
		quitf(_fail, "Could not open file '%s' with mode '%s'.", name, mode);
	closeOnHalt(file);
	return file;
}

FILE *fifo_in, *fifo_out, *logfile;

void registerManager(std::string probName, int argc, char* argv[]) {
	setName("manager for problem %s", probName.c_str());
	__testlib_ensuresPreconditions();
	testlibMode = _checker;
	random_t::version = 1; // Random generator version
	__testlib_set_binary(stdin);
	ouf.mode = _output;

	{//Keep alive on broken pipes
		//signal(SIGPIPE, SIG_IGN);
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigaction(SIGPIPE, &sa, NULL);
	}

	if (argc < 3 || 4 < argc)
		quitf(_fail,
			"Manager for problem %s:\n"
			"Invalid number of arguments: %d\n"
			"Usage: '%s' sol2mgr-pipe mgr2sol-pipe [mgr_log] < input-file",
			probName.c_str(), argc-1, argv[0]);

	inf.init(stdin, _input);
	closeOnHalt(stdout);
	closeOnHalt(stderr);
	fifo_out = openFile(argv[2], "a");
	fifo_in = openFile(argv[1], "r");
	logfile = NULL;
	if (argc > 3)
		logfile = openFile(argv[3], "w");
}

inline string tresult2str(TResult result) {
	switch (result) {
	case _ok: return "OK";
	case _wa: return "WA";
	case _fail: return "FAIL";
	case _points: return "PARTIAL";
	case _sv : return "SV";
	default: return "UNKNOWN";
	}
}
/********************************* End testlib-related material *********************************/


inline void writelnFifo(int i) {
	fprintf(fifo_out, "%d\n", i);
	fflush(fifo_out);
}


#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
void logprintf(const char* msgfmt, ...) {
	if (logfile) {
		FMT_TO_RESULT(msgfmt, msgfmt, message);
		fprintf(logfile, "%s", message.c_str());
		fflush(logfile);
	}
}


unique_ptr<ManagerStrategy> strategy;
vector< pair<VI, int> > queries; //vector(pair(query, response))
vector< pair<int, bool> > answer_optional; //vector(pair(answer, accepted)), 0 <= size <= 1
double _parital_score;


inline bool is_scenario_valid(const VI& species, string& reason) {
	#define verify(condition, msg) do if (!(condition)) { reason = (msg); return false; } while (false)
	//Verifying the species
	verify(sz(species) == n, format("Invalid length of species %d.", sz(species)));
	fori(i, n)
		verify(TYPE_0 <= species[i] && species[i] < N_TYPES, format("Invalid species value %d at position %d", species[i], i));
	verify(species[0] == TYPE_0, format("First element must be of type %d", TYPE_0));
	// Verifying correctness of query responses
	forv(i, queries) {
		int response = queries[i].second;
		int correct_response = count_diffs(species, queries[i].first);
		verify(response == correct_response, format("Wrong response (%d) for query %d (expected %d).", response, i+1, correct_response));
	}
	// Verifying the acceptance of the answer
	if (!answer_optional.empty()) {
		int answer = answer_optional[0].first;
		bool accepted = answer_optional[0].second;
		int correct_answer = count(species, TYPE_0);
		bool matched = (answer == correct_answer);
		verify(matched == accepted, accepted ?
				format("Answer (%d) is accepted, but correct answer is %d.", answer, correct_answer):
				format("Answer (%d) is rejected, but it is correct.", answer));
	}
	return true;
	#undef verify
}


#ifdef __GNUC__
__attribute__ ((format (printf, 3, 4)))
#endif
NORETURN inline void finish(TResult result, bool sendDie, const char* msgfmt, ...) {
	if (sendDie) {
		logprintf("-1\n");
		writelnFifo(-1);
	}

	if (answer_optional.empty())
		logprintf("NA\n");
	else
		logprintf("A %d\n", answer_optional[0].first);

	VI species = strategy->get_species();
	logprintf("%s\n", vec2cstr(species));

	FMT_TO_RESULT(msgfmt, msgfmt, message);
	if (!is_scenario_valid(species, message)) {
		result = _fail;
		message = "Invalid strategy behavior: " + message;
	}

	logprintf("%s\n%s\n", tresult2str(result).c_str(), message.c_str());
	if (result == _points)
		quitp(_parital_score, message);
	else
		quit(result, message);
}

inline int readFifoInteger(const char* name) {
	int x;
	if (1 != fscanf(fifo_in, "%d", &x))
		finish(_fail, false, "Could not read integer %s (IO error).", name);
	return x;
}

NORETURN inline void quit_partial(double grade, int qc) {
	_parital_score = grade/double(100);
	finish(_points, false, "Number of queries: %d", qc);
}

const int min_n = 2;
const int max_n = 20000;
const int max_qc = 20000;
const int max_qs = 100000;

NORETURN inline void finish_with_scoring(int qc) {
	#define qp(grade) quit_partial((grade), qc)
	if (max_qc < qc) finish(_wa, false, "Too many queries.");
	if (10010 < qc) qp(10);
	if (904 < qc)  qp(25);
	if (226 < qc)   qp(226.0/qc*100);
	/* (qc <= 226) */
	//qp(100);
	finish(_ok, false, "%s", "");
	#undef qp
}

int main(int argc, char **argv) {
	registerManager("mushrooms", argc, argv);

	string strategy_type = inf.readToken();
	logprintf("%s\n", strategy_type.c_str());
	strategy = strategyFactoryRegistry.create(strategy_type);
	if (!strategy)
		quitf(_fail, "Invalid strategy type '%s'", strategy_type.c_str());
	registerHaltListener([]() {
		strategy.reset(nullptr);
	});
	strategy->read_input(inf);
	if (logfile)
		strategy->describe_in_one_line(logfile);
	logprintf("\n");

	logprintf("%d\n", n);
	if (n < min_n || max_n < n)
		quitf(_fail, "Invalid value of n: %d", n);

	int qc = 0;
	int qs = 0;
	queries.clear();
	answer_optional.clear();

	writelnFifo(n);
	while (true) {
		string cmd;
		if (!readStr(fifo_in, cmd)) {
			logprintf("N\n\n");
			finish(_wa, false, "No more action (solution possibly terminated).");
		}
		if (cmd=="Q") {// Query
			int k = readFifoInteger("k");
			VI x(k);
			fori(i, k)
				x[i] = readFifoInteger("x[i]");
			logprintf("Q %d %s\n", k, vec2cstr(x));
			// Validating query
			if (k < 2)
				finish(_wa, true, "Too small array for query.");
			if (k > n)
				finish(_wa, true, "Too large array for query.");
			qc++;
			if (qc > max_qc)
				finish(_wa, true, "Too many queries.");
			qs += k;
			if (qs > max_qs)
				finish(_wa, true, "Too many total array sizes as queries.");
			fori(i, k)
				if (x[i] < 0 || n - 1 < x[i])
					finish(_wa, true, "Invalid value %d in the query array.", x[i]);
			vector<bool> used(n, false);
			fori(i, k) {
				if (used[x[i]])
					finish(_wa, true, "Duplicate value %d in the query array.", x[i]);
				used[x[i]] = true;
			}
			// Evaluating, logging, and sending response
			int response = strategy->ask(x);
			logprintf("%d\n", response);
			queries.emplace_back(x, response);
			writelnFifo(response);
		} else if (cmd=="W") {//Wrong query detected in grader
			string reason;
			if ((fscanf(fifo_in, " ") != 0) || !readLine(fifo_in, reason))
				finish(_fail, false, "Could not read reason for WA in grader (protocol error).");
			logprintf("W %s\n\n", reason.c_str());
			finish(_wa, false, "%s", reason.c_str());
		} else if (cmd=="A") {// Answer
			int answer = readFifoInteger("answer");
			bool accept = strategy->checkAnswer(answer);
			answer_optional.emplace_back(answer, accept);
			if (!accept)
				finish(_wa, false, "Answer is not correct.");
			finish_with_scoring(qc);
		} else {
			finish(_fail, false, "Invalid action (protocol error).");
		}
	}
	finish(_fail, false, "Must not reach here!");
}
