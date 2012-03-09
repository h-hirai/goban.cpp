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
  Board<19> bs[1000];
  std::vector<Turn> record = {
// converted from http://homepage1.nifty.com/tomospace/hikagokifu/kifu/hikago002b.sgf
// using https://gist.github.com/gists/1955575
#include "hikago002b.h"
  };

  for (auto &t : record) {
    for (auto &b: bs) {
      if (b.can_put(t.p, t.c))
        b.put(t.p, t.c);
      else
        assert(false);
    }
  }

  std::cout << bs[999] << std::endl;

  return 0;
}
