#include <iostream>
#include <string>
#include <cassert>

using namespace std;

const int MAX = 5;

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

    void show() const
    {
        for (int i=0;i<MAX;i++) {
            for (int j=0;j<MAX;j++) {
                cout << a[i][j] << " ";
            }
            cout << "\n";
        }
    }

    int swapdistance() const
    {
        int mtot = 0;
        int si[MAX], sj[MAX];
        for (int i=0;i<MAX;i++)
            si[i] = 0;
        for (int j=0;j<MAX;j++)
            sj[j] = 0;
        for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
        {
            mtot += a[i][j];
            si[i] += a[i][j];
            sj[j] += a[i][j];
        }
        for (int node=0;node<MAX;node++)
        if (si[node] != sj[node])
            return -1;

        Matrix aux = *this;
        int cycles = 0;

        // 1-cycles
        for (int i=0;i<MAX;i++)
        {
            cycles += aux.a[i][i];
            aux.a[i][i] = 0;
        }
        // 2-cycles
        for (int i=0;i<MAX;i++)
        for (int j=0;j<i;j++)
        {
            int c = min(aux.a[i][j], aux.a[j][i]);
            cycles += c;
            aux.a[i][j] -= c;
            aux.a[j][i] -= c;
        }

        // 3-cycles
        for (int i=0;i<MAX;i++)
        for (int j=0;j<i;j++)
        for (int k=0;k<j;k++)
        {
            int c;
            #define do3(i,j,k) c = min(aux.a[i][j], min(aux.a[j][k], aux.a[k][i])); cycles += c; aux.a[i][j] -= c; aux.a[j][k] -= c; aux.a[k][i] -= c;
            do3(i,j,k);
            do3(i,k,j);
        }
        // 4-cycles
        for (int i=0;i<MAX;i++)
        for (int j=0;j<i;j++)
        for (int k=0;k<j;k++)
        for (int l=0;l<k;l++)
        {
            int c;
            #define do4(i,j,k,l) c = min(min(aux.a[i][j], aux.a[j][k]), min(aux.a[k][l], aux.a[l][i])); cycles += c; aux.a[i][j] -= c; aux.a[j][k] -= c; aux.a[k][l] -= c; aux.a[l][i] -= c;
            do4(i,j,k,l);
            do4(i,j,l,k);
            do4(i,k,j,l);
            do4(i,k,l,j);
            do4(i,l,j,k);
            do4(i,l,k,j);
        }
        // 5-cycles
        int remainingEdges = 0;
        for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            remainingEdges += aux.a[i][j];
        assert(remainingEdges % 5 == 0);
        cycles += remainingEdges / 5;
        return mtot - cycles;
    }
};

const int MAXN = 250000;

Matrix fenwick[MAXN+1];

// logical fenwick array has indexes from 0 to MAXN-1, inclusive
// fenwickAdd(pos) adds the given value to index pos
// fenwickQuery(pos) returns the sum of range [0, pos)

void fenwickAdd(int pos, const Matrix &value)
{
    assert(0 <= pos);
    assert(pos < MAXN);
    for (pos++; pos <= MAXN ; pos += pos&(-pos))
        fenwick[pos].add(value);
}

Matrix fenwickQuery(int pos)
{
    assert(0 <= pos);
    assert(pos <= MAXN);
    Matrix ret;
    for (; pos > 0 ; pos -= pos&(-pos))
        ret.add(fenwick[pos]);
    return ret;
}

int charValues[256];

string globalA, globalB;

void init(string a, string b)
{
    globalA = a;
    globalB = b;
    charValues['A'] = 0;
    charValues['C'] = 1;
    charValues['G'] = 2;
    charValues['T'] = 3;
    charValues['U'] = 4;
    Matrix mat;
    for (int i=0;i<int(a.size());i++)
    {
        int x = charValues[int(a[i])];
        int y = charValues[int(b[i])];
        mat.a[x][y] = 1;
        fenwickAdd(i, mat);
        mat.a[x][y] = 0;
    }
}

void modify(int k, int i, char xChar)
{
    Matrix mat;
    int x = charValues[int(globalA[i])];
    int y = charValues[int(globalB[i])];
    mat.a[x][y] -= 1;
    if (k == 0)
        globalA[i] = xChar;
    else
        globalB[i] = xChar;
    x = charValues[int(globalA[i])];
    y = charValues[int(globalB[i])];
    mat.a[x][y] += 1;
    fenwickAdd(i, mat);
}

int get_distance(int i,int j)
{
    Matrix mat = fenwickQuery(j+1);
    mat.sub(fenwickQuery(i));
    return mat.swapdistance();
}
