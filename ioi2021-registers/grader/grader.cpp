#include "registers.h"
#include <cstdio>
#include <cassert>
#include <string>
using namespace std;

#ifdef _MSC_VER
#   define NORETURN __declspec(noreturn)
#elif defined __GNUC__
#   define NORETURN __attribute__ ((noreturn))
#else
#   define NORETURN
#endif


static const int m = 100;
static const int b = 2000;
static const int id_move = 0;
static const int id_store = 1;
static const int id_and = 2;
static const int id_or = 3;
static const int id_xor = 4;
static const int id_not = 5;
static const int id_left = 6;
static const int id_right = 7;
static const int id_add = 8;

struct instruction {
    int type,t,x,y;
    vector<bool> v;
    instruction(int _type): type(_type), t(-1), x(-1), y(-1) {}
};

static int s, n, k, q;
static vector<instruction> output;

NORETURN static inline void general_output(string verdict, string reason = ""){
    const string output_secret = "vjykoftpkrvoi6c4v1vgep879k0xk83kur2rcdvg";

    printf("%s\n", output_secret.c_str());
    printf("%s\n", verdict.c_str());

    if (verdict != "OK") {
        if (reason != "") {
            printf("%s\n", reason.c_str());
        }
    } else {
        printf("%d\n", (int)output.size());
        for(instruction &i: output) {
            printf("%d ", i.type);
            switch(i.type) {
                case id_move:
                case id_not:
                    printf("%d %d\n", i.t, i.x);
                    break;
                case id_store:
                    printf("%d ", i.t);
                    for(bool z: i.v) {
                        putchar(z+'0');
                    }
                    printf("\n");
                    break;
                default:
                    printf("%d %d %d\n", i.t, i.x, i.y);
            }
        }
    }
    fclose(stdout);
    exit(0);
}

NORETURN static inline void WA(string reason) {
    general_output("WA", reason);
}

static inline void check_instructions() {
    if (output.size() >= (size_t)q) {
        WA("Too many instructions");
    }
}

static inline void check_index(int index) {
    if (index < 0 || index >= m) {
        WA("Invalid index");
    }
}

void append_move(int t, int x) {
    check_instructions();
    check_index(t);
    check_index(x);
    instruction i(id_move);
    i.t = t;
    i.x = x;
    output.push_back(i);
}

void append_store(int t, vector<bool> v) {
    check_instructions();
    check_index(t);
    if ((int)v.size() != b) {
        WA("Value to store is not b bits long");
    }
    instruction i(id_store);
    i.t = t;
    i.v = v;
    output.push_back(i);
}

void append_and(int t, int x, int y) {
    check_instructions();
    check_index(t);
    check_index(x);
    check_index(y);
    instruction i(id_and);
    i.t = t;
    i.x = x;
    i.y = y;
    output.push_back(i);
}

void append_or(int t, int x, int y) {
    check_instructions();
    check_index(t);
    check_index(x);
    check_index(y);
    instruction i(id_or);
    i.t = t;
    i.x = x;
    i.y = y;
    output.push_back(i);
}

void append_xor(int t, int x, int y) {
    check_instructions();
    check_index(t);
    check_index(x);
    check_index(y);
    instruction i(id_xor);
    i.t = t;
    i.x = x;
    i.y = y;
    output.push_back(i);
}

void append_not(int t, int x) {
    check_instructions();
    check_index(t);
    check_index(x);
    instruction i(id_not);
    i.t = t;
    i.x = x;
    output.push_back(i);
}

void append_left(int t, int x, int p) {
    check_instructions();
    check_index(t);
    check_index(x);
    if (p < 0 || p > b) {
        WA("Invalid shift value");
    }
    instruction i(id_left);
    i.t = t;
    i.x = x;
    i.y = p;
    output.push_back(i);
}

void append_right(int t, int x, int p) {
    check_instructions();
    check_index(t);
    check_index(x);
    if (p < 0 || p > b) {
        WA("Invalid shift value");
    }
    instruction i(id_right);
    i.t = t;
    i.x = x;
    i.y = p;
    output.push_back(i);
}

void append_add(int t, int x, int y) {
    check_instructions();
    check_index(t);
    check_index(x);
    check_index(y);
    instruction i(id_add);
    i.t = t;
    i.x = x;
    i.y = y;
    output.push_back(i);
}

void append_print(int) {
    // print functions are ignored
    return;
}

int main() {
    const string input_secret = "sc56g857b25lzvk5b9ky5v8ymzqjal4owtshd9qv";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != input_secret) {
        general_output("PV", "Possible tampering with the input");
    }
    assert(4 == scanf("%d %d %d %d\n", &s, &n, &k, &q));
    fclose(stdin);

    construct_instructions(s, n, k, q);
    general_output("OK");
    return 0;
}
