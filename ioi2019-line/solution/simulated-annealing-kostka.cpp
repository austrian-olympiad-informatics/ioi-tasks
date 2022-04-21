#include "bits/stdc++.h"

using namespace std;

typedef pair<int, int> pt;
#define x first
#define y second

bool minimalizing = true;
int REHEATS = 5, PHASES = 1000, COUNTS = 100;
double INITIAL_TEMP = 10.0, TEMP_MLTPR = 0.99;
double TEMP;

inline double randdouble(double a, double b) {
  double f = (double)rand() / RAND_MAX;
  return a + f * (b - a);
}

bool prob(double las, double now) {
  if(minimalizing == false) swap(las, now);
  if(now < las) return true;
  double res = exp(-((now-las) / TEMP));
  return (min(1.0, res) > randdouble(0.0, 1.0));
}

struct solution {
  int n;
  pt ret[20000];
  int pkt[20000];
  int score = 0;

  int heuristic() {
    return score;
  }

  void update_pkt(int i) {
    if(i < 0 or i >= n) return;
    score -= pkt[i];
    if(i <= 1 or i+2 >= n) pkt[i] = 1;
    else {
      if((ret[i-1].x < ret[i].x and ret[i].x < ret[i+1].x) or
         (ret[i-1].y < ret[i].y and ret[i].y < ret[i+1].y) or
         (ret[i-1].x > ret[i].x and ret[i].x > ret[i+1].x) or
         (ret[i-1].y > ret[i].y and ret[i].y > ret[i+1].y)) {
        pkt[i] = 0;
      } else pkt[i] = 1;
    }
    score += pkt[i];
  }
};

solution random_move(solution s) {
  solution cur = s;
  if(rand()%10 < 5) {
    int c = rand()%(s.n/2-1);
    if(cur.ret[2*c+1].x == cur.ret[2*c+2].x) {
      cur.ret[2*c+1].x = cur.ret[2*c+2].x;
      cur.ret[2*c+1].y = cur.ret[2*c].y;
    } else {
      cur.ret[2*c+1].y = cur.ret[2*c+2].y;
      cur.ret[2*c+1].x = cur.ret[2*c].x;
    }
    for(int i=2*c-1; i<=2*c+3; i++) cur.update_pkt(i);
  } else {
    int c = rand()%(s.n/2), d = rand()%(s.n/2);
    while(abs(d-c) <= 2) d = rand()%(s.n/2);
    // swap(cur.ret[2*c+1], cur.ret[2*d+1]);
    for(int e = 0; e <= 2; e++) {
      if(s.ret[2*c+e].x == s.ret[2*c+1].x) cur.ret[2*c+e].x = s.ret[2*c+e].x;
      if(s.ret[2*c+e].y == s.ret[2*c+1].y) cur.ret[2*c+e].y = s.ret[2*c+e].y;
    }
    for(int e = 0; e <= 2; e++) {
      if(s.ret[2*d+e].x == s.ret[2*d+1].x) cur.ret[2*d+e].x = s.ret[2*d+e].x;
      if(s.ret[2*d+e].y == s.ret[2*d+1].y) cur.ret[2*d+e].y = s.ret[2*d+e].y;
    }
    for(int i=2*c-1; i<=2*c+3; i++) cur.update_pkt(i);
    for(int i=2*d-1; i<=2*d+3; i++) cur.update_pkt(i);
  }
  return cur;
}

solution simulated_annealing(solution currentSolution) {
  TEMP = INITIAL_TEMP;
  double bestTemperature = TEMP;
  solution bestSolution = currentSolution;
  for(int reheat = 0; reheat < REHEATS; reheat++) {
    for(int phase = 0; phase < PHASES; phase++) {
      for(int count = 0; count < COUNTS; count++) {
        solution newSolution = random_move(currentSolution);
        bool ok = (newSolution.heuristic() < currentSolution.heuristic());
        if(!ok) ok |= prob(currentSolution.heuristic(), newSolution.heuristic());
        if(ok) {
          currentSolution = newSolution;
          if(newSolution.heuristic() < bestSolution.heuristic()) {
            bestSolution = newSolution;
            reheat = 0;
            phase = 0;
            count = 0;
            bestTemperature = TEMP;
          }
        }
      }
      TEMP *= TEMP_MLTPR;
    }
    currentSolution = bestSolution;
    TEMP = 2*bestTemperature;
  }
  return bestSolution;
}

vector <pt> random_ordering_sol(vector <pt> test) {
  int n = (int)test.size();
  solution initial_solution;
  initial_solution.n = 2*n;
  pt cur = {0, 0};
  bool dir = true;
  for(int i=0; i<2*n; i++) {
    if(dir) {
      cur.x = test[i/2].x;
      dir ^= 1;
    } else {
      cur.y = test[i/2].y;
      dir ^= 1;
    }
    if(i&1 and rand()%10 < 5) dir ^= 1;
    initial_solution.ret[i] = cur;
  }
  for(int i=0; i<2*n; i++) {
    initial_solution.update_pkt(i);
    initial_solution.score += initial_solution.pkt[i];
  }
  auto best_solution = simulated_annealing(initial_solution);
  vector <pt> ret;
  for(int i=0; i<2*n; i++) {
    if(best_solution.pkt[i]) ret.push_back(best_solution.ret[i]);
  }
  return ret;
}

int main() {
  srand(15061994);
  int n;
  cin >> n;
  vector <pt> test(n);
  for(int i=0; i<n; i++) cin >> test[i].x >> test[i].y;
  auto sol = random_ordering_sol(test);
  cout << (int)sol.size() << "\n";
  for(auto s : sol) cout << s.x << " " << s.y << "\n";
}
