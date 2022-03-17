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
}
int get_distance(int i,int j)
{
    Matrix mat;
    for (int k = i; k <= j; k++) {
        int x = charValues[int(globalA[k])];
        int y = charValues[int(globalB[k])];
        mat.a[x][y] += 1;
    }
    if (mat.a[0][1] != mat.a[1][0]) {
        return -1;
    } else {
        return mat.a[0][1];
    }
}
