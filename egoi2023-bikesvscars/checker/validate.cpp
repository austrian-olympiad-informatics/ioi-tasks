#include "validate.h"

#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef long double ld;

void toupper_string(string &s){
    for (char& c : s) c = (char)toupper(c);
}

struct Edge {
  int u;
  int v;
  int d;

  Edge(int u, int v, int d){
    this->u = min(u,v);
    this->v = max(u,v);
    this->d = d;
  }
  bool operator<(const Edge& e) const {
    return (d < e.d);
  }
};

int find(int a, vector<int>& UF){
  if(UF[a] == a) return a;
  else {
    int temp = find(UF[a], UF);
    UF[a] = temp;
    return UF[a];
  }
}

int unite(int a, int b, vector<int>& UF){
  int pa = find(a,UF);
  int pb = find(b,UF);

  if(pa == pb) return 0;

  UF[pa] = pb;
  return 1;
}

void dfs_max(int v, int p, vector<vector<pair<int,int>>>& g, vector<int>& dist){
  // cout << v << ' ' << dist[v] <<'\n';
  for(pair<int,int> e : g[v]){
    if(e.first == p) continue;
    dist[e.first] = max(dist[v],e.second);
    dfs_max(e.first,v,g,dist);
  }
}

void dfs_min(int v, int p, vector<vector<pair<int,int>>>& g, vector<int>& dist){
  for(pair<int,int> e : g[v]){
    if(e.first == p) continue;
    dist[e.first] = min(dist[v],e.second);
    dfs_min(e.first,v,g,dist);
  }
}

int main(int argc, char **argv) {
    init_io(argc, argv);
    //judge_message("started testcase");    
    string sj, sc, dummy;
    judge_ans >> sj;
    
    if(!(author_out >> sc)){
      wrong_answer("Empty output");
    }
    toupper_string(sc);
    if(sc == "NO"){
      if(sj != "NO"){
        wrong_answer("Contestant said no, judge found solution");
      }
    }
    else{
      int n,m,w;

      try{
        size_t pos;
        m = stoi(sc, &pos);
        if (pos != sc.size()) throw 0;
      }
      catch(...){
        wrong_answer("m was not an integer");
      }

      judge_in >> n >> w;
      if(m > 2023){
        wrong_answer("Used more than 2023 edges: %d", m);
      }
      if(m < 0){
        wrong_answer("Used negative number of edges: %d", m);
      }

      vector<Edge> edge_list;

      for(int i = 0; i<m; ++i){
        int u,v,d;
        author_out >> u >> v >> d;
        if (!author_out) wrong_answer("unexpected eof");
        if(u < 0 || u >= n || v < 0 || v >= n){
          wrong_answer("vertex index out of bounds");
        }
        if(u == v){
          wrong_answer("loops are not allowed: %d - %d: %d", u,v,d);
        }
        if(d<0){
          wrong_answer("negative edge length %d - %d: %d",u,v,d);
        }
        if(d>w){
          wrong_answer("edge length more than max %d - %d: %d",u,v,d);
        }
        edge_list.push_back(Edge(u,v,d)); 
      }


      // sort by increasing d
      sort(edge_list.begin(),edge_list.end());
      vector<vector<pair<int,int>>> mst(n);
      vector<int> mst_UF;
      for(int i = 0;i<n;++i){
          mst_UF.push_back(i);
      }
      for(Edge e: edge_list){
        if(unite(e.u,e.v,mst_UF)){
          mst[e.u].push_back(make_pair(e.v,e.d));
          mst[e.v].push_back(make_pair(e.u,e.d));
          // cout << e.v << ' ' << e.u <<  ' ' << e.d << '\n';
        }
      }

      for(int i = 1; i<n;i++){
        if(find(i-1,mst_UF)!= find(i,mst_UF)){
          wrong_answer("The graph is not connected");
        }
      }
      for(int i = 1;i<n;++i){
        vector<int> dist(n,-1);
        dfs_max(i,-1,mst,dist);
        for(int j = 0;j<i;++j){
          int aij;
          judge_in >> aij;
          aij = w-aij;
          
          int res = dist[j];
          //judge_message("i %d, j  %d, res %d, aij %d",i,j,res,aij);
          // cout << "Done " << i << ' ' << j << ' ' << res << ' '<<aij<< '\n';
          if(res != aij){
            wrong_answer("Min Max %d - %d not equal aij, is %d, should be %d",i,j,res,aij);
          }

        }
      }
      
      // sort by decreasing d
      reverse(edge_list.begin(),edge_list.end());
      vector<vector<pair<int,int>>> MST(n);
      vector<int> MST_UF;
      for(int i = 0;i<n;++i){
          MST_UF.push_back(i);
      }
      for(Edge e: edge_list){
        if(unite(e.u,e.v,MST_UF)){
          MST[e.u].push_back(make_pair(e.v,e.d));
          MST[e.v].push_back(make_pair(e.u,e.d));
          // cout << e.v << ' ' << e.u << '\n';
        }
      }


      for(int i = 1;i<n;++i){
        vector<int> dist(n,INT_MAX);
        dfs_min(i,-1,MST,dist);
        for(int j = 0;j<i;++j){
          int bij;
          judge_in >> bij;
          int res = dist[j];
          // cout << "Done " << i << ' ' << j << ' ' << res << ' '<<bij<< '\n';
          if (res != bij){
            wrong_answer("Max Min %d - %d not equal bij is %d should be %d",i,j,res,bij);
          }

        }
      }
      if(sj == "NO"){
        judge_error("Contestant found a solution but judge said no");
      }
    }
    if (author_out >> dummy) wrong_answer("expected eof after YES answer");
    
    accept();
}
