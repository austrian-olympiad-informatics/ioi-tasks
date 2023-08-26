#include "validate.h"

#include <bits/stdc++.h>
using namespace std;


const int MAX_LENGTH = 20'000;
const int MAX_QUERIES = 2'500;

int main(int argc, char **argv) {
  init_io(argc, argv);

  int H, W;
  int box_x, box_y;
  judge_in >> H >> W;
  judge_in >> box_x >> box_y;
  box_x--;
  box_y--; // use 0-indexing
  string group;
  judge_in >> group;
  assert(group == "SAMPLE" || group == "SCORE");

  author_in << H << ' ' << W << endl;

  int queries = 0, tot_length = 0;

  unordered_map<char,pair<int,int>> direction = {
    {'>', {0, +1}},
    {'<', {0, -1}},
    {'v', {+1, 0}},
    {'^', {-1, 0}},
  };

  auto valid = [&](int x, int y) {
    if(x == box_x && y == box_y) return false;
    if(x < 0 || x > H-1) return false;
    if(y < 0 || y > W-1) return false;
    return true;
  };

  auto simulate = [&](string s) {
    int x = 0, y = 0;
    for(char c: s) {
      if(!direction.count(c))
        wrong_answer("[query %d] bad direction (char: \'%c\'; ascii: %d)", queries, c, (int)c);
      int nx = x + direction[c].first;
      int ny = y + direction[c].second;
      if(valid(nx,ny)) tie(x,y) = tie(nx,ny);
    }
    return make_pair(x,y);
  };

  for (;;) {
    char type;
    author_out >> type;
    if(!author_out) wrong_answer("unexpected eof1");
    if(type== '!') {
      int ans_x, ans_y;
      author_out >> ans_x >> ans_y;
      if(!author_out) wrong_answer("unexpected eof2 / not a number");
      if(ans_x != box_x || ans_y != box_y)
        wrong_answer("wrong answer: expected (%d,%d) but got (%d,%d)", box_x, box_y, ans_x, ans_y);
      break;
    }
    else if(type == '?') {
      ++queries;
      if(queries > MAX_QUERIES) wrong_answer("too many queries");
      string instructions;
      author_out >> instructions;
      if(instructions.size() > MAX_LENGTH)
        wrong_answer("[query %d] has too many instructions (length: %d)", queries, instructions.size());
      auto [x,y] = simulate(instructions);
      author_in << x << ' ' << y << endl;
    }
    else {
      wrong_answer("[query %d] bad query type (char: \'%c\'; ascii: %d)", queries, type, (int)type);
    }
  }

  int garbage;
  author_out >> garbage;
  if(author_out) wrong_answer("trailing output, expected eof");

  double score = round(min(100.0, 100.0*sqrt(2.0/max<double>(1,queries))));

  if(group == "SAMPLE") score = 0.0; // hacky way to give sample 0 points...

  judge_message(score / 100.0, "used %d queried; score: %.2f", queries, score);
}
