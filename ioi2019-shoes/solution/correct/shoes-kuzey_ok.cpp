//In the Name of God
//Ya Ali

// --- Ali Ahmadi ali.ahmadi.star27@gmail.com --- \\

// correct solution with fenwick

#include <bits/stdc++.h>
#include "shoes.h"

#define lng long long

using namespace std;

const int maxn = 2002002;

int fen[maxn];
int _pos[maxn];
int *pos = _pos + maxn / 2;

void add(int x)
{
  for (x ++; x < maxn; x += x & -x)
    fen[x] ++;
}

int get(int x)
{
  int r = 0;
  for (x ++; x; x -= x & -x)
    r += fen[x];
  return r;
}

vector<int> change(vector<int> v) {
	int n = (int)v.size() / 2;
	vector<int> cnt(n, 0);
	for(int i = 0; i < 2*n; i++)
		if(v[i] > 0)
			cnt[v[i] - 1]++;

	for(int i = 1; i < n; i++)
		cnt[i] += cnt[i-1];

	vector<int> cntl = cnt, cntr = cnt;

	for(int i = 0; i < 2*n; i++)
		if(v[i] > 0)
			v[i] = (--cntr[v[i] - 1]) + 1;
		else
			v[i] = -((--cntl[-v[i] - 1]) + 1);
	return v;
}


lng count_swaps(vector<int> S)
{
	S = change(S);
  memset(fen, 0, sizeof fen);
  memset(_pos, 0, sizeof _pos);
  
  lng r = 0;
  for (int i = 0; i < S.size(); i ++)
    pos[S[i]] = i;

  for (int i = 0; i < S.size(); i ++)
    if (S[i])
      {
	r += pos[-S[i]] - get(pos[-S[i]]) - (S[i] < 0);	
	add(i); add(pos[-S[i]]); S[pos[-S[i]]] = 0; S[i] = 0;
      }
  return r;
}
