#include "testlib.h"
#include <bitset>
using namespace std;

static string output_secret = "vjykoftpkrvoi6c4v1vgep879k0xk83kur2rcdvg";

const int m = 100;
const int b = 2000;
bitset<b> reg[m];

int s,n,k;

void append_move(int t, int x) {
    reg[t] = reg[x];
}

void append_store(int t, vector<bool> v) {
    for(int i=0; i<b; i++) {
        reg[t][i] = v[i]; // bit-by-bit copy
    }
}

void append_and(int t, int x, int y) {
    reg[t] = (reg[x]&reg[y]);
}

void append_or(int t, int x, int y) {
    reg[t] = (reg[x]|reg[y]);
}

void append_xor(int t, int x, int y) {
    reg[t] = (reg[x]^reg[y]);
}

void append_not(int t, int x) {
    reg[t] = (~reg[x]);
}

void append_left(int t, int x, int s) {
    reg[t] = (reg[x]<<s);
}

void append_right(int t, int x, int s) {
    reg[t] = (reg[x]>>s);
}

void append_add(int t, int x, int y) {
    bitset<b> tmp;
    bool carry = false;
    for(int i = 0; i < b; i++) {
        tmp[i] = (reg[x][i] ^ reg[y][i] ^ carry);
        carry = (reg[x][i] & reg[y][i]) || (reg[x][i] & carry) || (reg[y][i] & carry); // discard the last carry
    }
    reg[t] = tmp;
}

const int id_move = 0;
const int id_store = 1;
const int id_and = 2;
const int id_or = 3;
const int id_xor = 4;
const int id_not = 5;
const int id_left = 6;
const int id_right = 7;
const int id_add = 8;

struct instruction {
    int type,t,x,y;
    vector<bool> v;
    instruction(int id_type): type(id_type), t(-1), x(-1), y(-1) {}
    void execute() {
        switch(type) {
            case id_move:
                append_move(t, x);
                break;
            case id_store:
                append_store(t, v);
                break;
            case id_and:
                append_and(t, x, y);
                break;
            case id_or:
                append_or(t, x, y);
                break;
            case id_xor:
                append_xor(t, x, y);
                break;
            case id_not:
                append_not(t, x);
                break;
            case id_left:
                append_left(t, x, y);
                break;
            case id_right:
                append_right(t, x, y);
                break;
            case id_add:
                append_add(t, x, y);
                break;
            default:
                quit(_fail, "Unknown instruction");
        }
    }
};

vector<instruction> instructions;
void load_register(bitset<b>& bs, vector<int>& v) {
    bs.reset();
    for (int i = 0; i < (int)v.size(); i++) {
        for (int j = 0; j < k; j++) {
            bs[i * k + j] = (v[i] & (1 << j));
        }
    }
}

void unload_register(bitset<b>& bs, vector<int>& v) {
    v.assign(v.size(), 0);
    for (int i = 0; i < (int)v.size(); i++) {
        for (int j = 0; j < k; j++) {
            v[i] |= (bs[i * k + j] << j);
        }
    }
}

void generate_random(vector<int>& a) {
    for (int i = 0; i < (int)a.size(); i++) {
        a[i] = rnd.next(0, (1 << k) - 1);
    }
}

void run_test(vector<int>& a) {
    load_register(reg[0], a);
    for (int i = 1; i < m; i++) {
        reg[i].reset();
    }
    for (instruction i: instructions) {
        i.execute();
    }

    sort(a.begin(), a.end());
    vector<int> ans(n);
    unload_register(reg[0], ans);
    if (s == 1) {
        for (int i = 0; i < n; i++) {
            if (a[i] != ans[i]) quit(_wa, "Incorrect sorting");
        }
    } else if (a[0] != ans[0]) quit(_wa, "Incorrect min value");
}


NORETURN void quitByGraderResult_customized(TResult result, std::string defaultMessage) {
    std::string msg = "";
    if (!ouf.eof())
        msg = ouf.readLine();
    if (msg.empty())
        ouf.quits(result, defaultMessage);
    if (result == _wa) {
        RESULT_MESSAGE_WRONG = msg;
        ouf.quits(result, defaultMessage);
    }
    ouf.quits(result, msg);
}

void readOutputGraderResult_customized() {
    std::string result = ouf.readWord();
    ouf.eoln();
    if (result == _grader_OK)
        return;
    if (result == _grader_SV)
        quitByGraderResult_customized(_sv, "Security violation detected in grader");
    if (result == _grader_PV)
        quitByGraderResult_customized(_pv, "Protocol violation detected in grader");
    if (result == _grader_WA)
        quitByGraderResult_customized(_wa, "Wrong answer detected in grader");
    if (result == _grader_FAIL)
        quitByGraderResult_customized(_fail, "Failure in grader");
    quitf(_fail, "Unknown grader result");
}

int main(int argc, char * argv[]) {
	registerChecker("registers", argc, argv);

	readBothSecrets(output_secret);
	readOutputGraderResult_customized();

	inf.readLine(); // Input secret
	s = inf.readInt();
	n = inf.readInt();
	k = inf.readInt();

	int q = ouf.readInt();
	int q_allowed = inf.readInt();
	if (q > q_allowed) {
        quit(_wa, "Too many instructions");
	}

	for(int j = 0; j < q; j++) {
        int type = ouf.readInt();
        instruction i(type);
        i.t = ouf.readInt();
        switch(type) {
            case id_move:
            case id_not:
                i.x = ouf.readInt();
                break;
            case id_store:
                i.v.resize(b);
                for(int a=0; a<b; a++) {
                    char tmp;
                    do {
                        tmp = ouf.readChar();
                    } while (tmp != '0' and tmp != '1');
                    if (tmp == '0') {
                        i.v[a] = 0;
                    } else if (tmp == '1') {
                        i.v[a] = 1;
                    } else {
                        quit(_fail, "Unexpected store value");
                    }
                }
                break;
            case id_and:
            case id_or:
            case id_xor:
            case id_left:
            case id_right:
            case id_add:
                i.x = ouf.readInt();
                i.y = ouf.readInt();
                break;
            default:
                quit(_fail, "Unknown instruction");
        }
        instructions.push_back(i);
	}
	quitif(s != 0 and s != 1, _fail, "Invalid subtask");

    vector<int> a(n);
    for (int tc = 0; tc < 5; tc++) {
        generate_random(a);
        run_test(a);
    }

    // sorted
    generate_random(a);
    sort(a.begin(), a.end());
    run_test(a);

    // reverse sorted
    generate_random(a);
    sort(a.begin(), a.end());
    reverse(a.begin(), a.end());
    run_test(a);

    // all same
    a.assign(n, rnd.next(0, (1 << k) - 1));
    run_test(a);

    // all 0
    a.assign(n, 0);
    run_test(a);

    // few unique values
    for (int tc = 2; tc < 5; tc++) {
        vector<int> v(tc);
        generate_random(v);
        for (int i = 0; i < n; i++) {
            a[i] = v[rnd.next(0, tc - 1)];
        }
        run_test(a);
    }

    // almost sorted
    for (int tc = 1; tc < 4; tc++) {
        generate_random(a);
        sort(a.begin(), a.end());
        for (int i = 0; i < tc; i++) {
            swap(a[rnd.next(0, n - 1)], a[rnd.next(0, n - 1)]);
        }
        run_test(a);
    }

    // almost reverse sorted
    for (int tc = 1; tc < 4; tc++) {
        generate_random(a);
        sort(a.begin(), a.end());
        reverse(a.begin(), a.end());
        for (int i = 0; i < tc; i++) {
            swap(a[rnd.next(0, n - 1)], a[rnd.next(0, n - 1)]);
        }
        run_test(a);
    }

    // All MAX except last element is MAX-1
    // This kills prabowo_min_wa.cpp
    {
        a.assign(n, (1 << k) - 1);
        a.back() = (1 << k) - 2;
        run_test(a);
    }

    // All 0 except last element is 1
    {
        a.assign(n, 0);
        a.back() = 1;
        run_test(a);
    }

    // Contains only 0 and MAX
    {
        a.assign(n, 0);
        for (int &elem : a) {
            if (rnd.next(2)) {
                elem = (1 << k) - 1;
            }
        }
        run_test(a);
    }
	quit(_ok);
}
