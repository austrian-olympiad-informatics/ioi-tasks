#include "closing.h"

#include<bits/stdc++.h>

using namespace std;
const int c=200005;
vector<pair<int, long long> > sz[c];
vector<pair<int, long long> > sor;
long long n, dist[c][2], bal, jobb, len, pref[2*c], inf=1e18;
bool ut[c];
void dfs(int a, int el, long long tav, int id) {
    dist[a][id]=tav;
    for (auto p:sz[a]) {
        int x=p.first;
        long long y=tav+p.second;
        if (x!=el) {
            dfs(x, a, y, id);
        }
    }
}
bool dfs2(int a, int el, long long tav) {
    if (a==jobb) {
        sor.push_back({a, tav});
        ut[a]=1;
        return true;
    }
    for (auto p:sz[a]) {
        int x=p.first;
        long long y=tav+p.second;
        if (x!=el) {
            if (dfs2(x, a, y)) {
                sor.push_back({a, tav});
                ut[a]=1;
                return true;
            }
        }
    }
    return false;
}
long long calc(long long ert) {
    if (ert<0) {
        return -inf;
    }
    int lo=0, hi=2*n+1, mid;
    while (hi-lo>1) {
        mid=(hi+lo)/2;
        if (pref[mid]<=ert) {
            lo=mid;
        } else {
            hi=mid;
        }
    }
    //cout << "calc " << calc << " " << ert << " " << lo << " " << pref[lo] << "\n";
    return lo;
}
int max_score(int N, int X, int Y, long long K, std::vector<int> U, std::vector<int> V, std::vector<int> W) {
    n=N;
    for (int i=0; i<n-1; i++) {
        int a=U[i], b=V[i], c=W[i];
        sz[a].push_back({b, c});
        sz[b].push_back({a, c});
    }
    bal=X, jobb=Y;
    dfs(bal, -1, 0, 0), dfs(jobb, -1, 0, 1);
    /*cout << "tavolsagok\n";
    for (int i=0; i<n; i++) {
        cout << dist[i][0] << " ";
    }
    cout << "\n";
    for (int i=0; i<n; i++) {
        cout << dist[i][1] << " ";
    }
    cout << "\n";*/
    len=dist[jobb][0];
    long long spec=0, maxi=0;
    if (true) {
        vector<long long> p;
        for (int i=0; i<n; i++) {
            p.push_back(dist[i][0]);
            p.push_back(dist[i][1]);
        }
        sort(p.begin(), p.end());
        long long sum=0, cnt=0;
        for (auto x:p) {
            if (sum+x<=K) {
                sum+=x;
                cnt++;
            }
        }
        spec=cnt;
    }
    bool s=dfs2(bal, -1, 0);
    reverse(sor.begin(), sor.end());
    /*cout << "sor\n";
    for (auto x:sor) {
        cout << x.first << " " << x.second << "\n";
    }*/
    long long si=sor.size(), sum=0, cnt=0, kezd=0, veg=si-1;
    while (kezd<=veg) {
        if (sor[kezd].second<=len-sor[veg].second) {
            cnt++;
            sum+=sor[kezd].second;
            kezd++;
        } else {
            cnt++;
            sum+=len-sor[veg].second;
            veg--;
        }
    }
    //cout << "vege " << cnt << " " << sum << "\n";
    //cout << "pos " << kezd << " " << veg << "\n";
    vector<long long> add;
    for (int i=kezd; i<si; i++) {
        add.push_back(2*sor[i].second-len);
    }
    for (int i=0; i<=veg; i++) {
        add.push_back(len-2*sor[i].second);
    }


    for (int i=1; i<=2*n; i++) {
        pref[i]=inf;
    }
    vector<pair<long long, long long> > dupla;
    vector<long long> prefmax, sufmin;
    for (int i=0; i<n; i++) {
        if (ut[i]) {
            continue;
        }
        long long a=dist[i][0], b=dist[i][1];
        if (a>b) {
            swap(a, b);
        }
        if (2*a<=b) {
            add.push_back(a);
            add.push_back(b-a);
        } else {
            dupla.push_back({b, a});
        }
    }


    sort(add.begin(), add.end());
    assert(add[0]>=0);

    for (int i=0; i<add.size(); i++) {
        pref[i+1]=pref[i]+add[i];
    }
    if (dupla.size()>0) {
        sort(dupla.begin(), dupla.end());
        int ds=dupla.size();
        prefmax.resize(ds), sufmin.resize(ds);
        prefmax[0]=dupla[0].first-dupla[0].second;
        for (int i=1; i<ds; i++) {
            prefmax[i]=max(prefmax[i-1], dupla[i].first-dupla[i].second);
        }
        sufmin[ds-1]=dupla[ds-1].second;
        for (int i=ds-2; i>=0; i--) {
            sufmin[i]=min(sufmin[i+1], dupla[i].second);
        }

        maxi=max(maxi, cnt+calc(K-sum));
        maxi=max(maxi, cnt+1+calc(K-sum-sufmin[0]));
        for (int i=0; i<ds; i++) {
            sum+=dupla[i].first;
            maxi=max(maxi, cnt+2*i+1+calc(K-sum+prefmax[i]));
            maxi=max(maxi, cnt+2*i+2+calc(K-sum));
            if (i+1<ds) {
                maxi=max(maxi, cnt+2*i+3+calc(K-sum-sufmin[i+1]));
            }
        }
    }
    maxi=max(maxi, cnt+calc(K-sum));

    for (int i=0; i<=n; i++) {
        sz[i].clear();
        ut[i]=0;
    }
    sor.clear();
    len=0;

    //cout << "ertekek " << maxi << " " << spec << "\n";

    return max(maxi, spec);
    // nullázás okosan
}


/*
int main() {

  int Q;
  assert(1 == scanf("%d", &Q));

  std::vector<int> N(Q), X(Q), Y(Q);
  std::vector<long long> K(Q);
  std::vector<std::vector<int>> U(Q), V(Q), W(Q);

  for(int q = 0; q < Q; q++){
    assert(4 == scanf("%d %d %d %lld", &N[q], &X[q], &Y[q], &K[q]));

    U[q].resize(N[q] - 1);
    V[q].resize(N[q] - 1);
    W[q].resize(N[q] - 1);
    for (int i = 0; i < N[q] - 1; ++i) {
      assert(3 == scanf("%d %d %d", &U[q][i], &V[q][i], &W[q][i]));
    }
  }
  fclose(stdin);

  std::vector<int> result(Q);
  for(int q = 0; q < Q; q++){
    result[q] = max_score(N[q], X[q], Y[q], K[q], U[q], V[q], W[q]);
  }

  for(int q = 0; q < Q; q++){
    printf("%d\n", result[q]);
  }
  fclose(stdout);

  return 0;
}
*/
/*
1
7 0 2 10
0 1 2
0 3 3
1 2 4
2 4 2
2 5 5
5 6 3
*/
/*
2
7 0 2 10
0 1 2
0 3 3
1 2 4
2 4 2
2 5 5
5 6 3


4 0 3 20
0 1 18
1 2 1
2 3 19

*/
// NULLAZAS!!!!!!!!!!!!!!!!!!!
