#include <iostream>
#include <string>
#include <cassert>
using namespace std;

const int MAX = 4;

struct Matrix
{
    int a[MAX][MAX];

    Matrix()
    {
        for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            a[i][j] = 0;
    }

    void add(const Matrix &o)
    {
        for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            a[i][j] += o.a[i][j];
    }

    void sub(const Matrix &o)
    {
        for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            a[i][j] -= o.a[i][j];
    }

    int swapdistance() const
    {
        int mtot = 0;
        int si[MAX];
        for (int i=0;i<MAX;i++)
            si[i] = 0;
        for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
        {
            mtot += a[i][j];
            si[i] += a[i][j];
            si[j] -= a[i][j];
        }
        for (int node=0;node<MAX;node++)
        if (si[node])
            return -1;

        Matrix aux = *this;
        int cycles = 0;


        for (int i=0;i<MAX;i++)
        {
            cycles += aux.a[i][i];
            aux.a[i][i] = 0;
        }

        for (int i=0;i<MAX;i++)
        for (int j=0;j<i;j++)
        {
            int c = min(aux.a[i][j], aux.a[j][i]);
            cycles += c;
            aux.a[i][j] -= c;
            aux.a[j][i] -= c;
        }


        for (int i=0;i<MAX;i++)
        for (int j=0;j<i;j++)
        for (int k=0;k<j;k++)
        {
            int c;

            c = min(aux.a[i][j], min(aux.a[j][k], aux.a[k][i])); cycles += c; aux.a[i][j] -= c; aux.a[j][k] -= c; aux.a[k][i] -= c;;
            c = min(aux.a[i][k], min(aux.a[k][j], aux.a[j][i])); cycles += c; aux.a[i][k] -= c; aux.a[k][j] -= c; aux.a[j][i] -= c;;
        }

        int remainingEdges = 0;
        for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            remainingEdges += aux.a[i][j];
        assert(remainingEdges % MAX == 0);
        cycles += remainingEdges / MAX;
        return mtot - cycles;
    }
};


const int MAXN = 250000;


Matrix prefix[MAXN+1];

void prefixAdd(int pos, const Matrix &value) {
    assert(0 <= pos);
    assert(pos < MAXN);
    pos++;
    prefix[pos].add(prefix[pos-1]);
    prefix[pos].add(value);
}

Matrix prefixQuery(int pos) {
    assert(0 <= pos);
    assert(pos <= MAXN);
    Matrix ret;
    ret.add(prefix[pos]);
    return ret;
}


int charValues[256];

string globalA, globalB;

void init(string a, string b)
{
    globalA = a;
    globalB = b;
    charValues['A'] = 0;
    charValues['T'] = 1;
    charValues['C'] = 2;
    charValues['G'] = 3;

    Matrix mat;
    for (int i=0;i<int(a.size());i++)
    {
        int x = charValues[int(a[i])];
        int y = charValues[int(b[i])];
        mat.a[x][y] = 1;
        prefixAdd(i, mat);
        mat.a[x][y] = 0;
    }

}
int get_distance(int p,int q)
{
    Matrix mat = prefixQuery(q+1);
    mat.sub(prefix[p]);
    int swaps = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < i; j++) {

            int c = min(mat.a[i][j], mat.a[j][i]);
            swaps += c;
            mat.a[i][j] -= c;
            mat.a[j][i] -= c;

            int k = 3 - i - j;
            c = min(mat.a[i][j], mat.a[j][k]);
            swaps += c;
            mat.a[i][j] -= c;
            mat.a[j][k] -= c;
            mat.a[i][k] += c;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < i; j++) {
            if (mat.a[i][j] != 0 || mat.a[j][i] != 0) {
                return -1;
            }
        }
    }
    return swaps;
}
