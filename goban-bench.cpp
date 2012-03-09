#include "goban.hpp"
#include <vector>
#include <iostream>
#include <cassert>

struct Turn {
  const Point p;
  const color_t c;

  Turn(const Point _p, const color_t _c) : p(_p), c(_c) {}
};


int main(void) {
  Board<19> b;
  std::vector<Turn> record = {
// converted from http://homepage1.nifty.com/tomospace/hikagokifu/kifu/hikago002b.sgf
// using https://gist.github.com/gists/1955575
#include "hikago002b.h"
  };

  for (auto &t : record) {
    if (b.can_put(t.p, t.c))
      b.put(t.p, t.c);
    else
      assert(false);
  }

  std::cout << b << std::endl;

  return 0;
}
