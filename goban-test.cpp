#include "goban.h"

#define BOOST_TEST_MODULE GobanTest
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(point_test) {
  Point p = Point(3, 4);

  BOOST_CHECK_EQUAL(p.x, 3);
  BOOST_CHECK_EQUAL(p.y, 4);

  Point pu = p.up();
  Point pd = p.down();
  Point pl = p.left();
  Point pr = p.right();

  BOOST_CHECK_EQUAL(pu.x, p.x);
  BOOST_CHECK_EQUAL(pu.y, p.y - 1);

  BOOST_CHECK_EQUAL(pd.x, p.x);
  BOOST_CHECK_EQUAL(pd.y, p.y + 1);

  BOOST_CHECK_EQUAL(pl.x, p.x - 1);
  BOOST_CHECK_EQUAL(pl.y, p.y);

  BOOST_CHECK_EQUAL(pr.x, p.x + 1);
  BOOST_CHECK_EQUAL(pr.y, p.y);

  Points ps = p.around();
  BOOST_CHECK(ps->size() == 4);
  BOOST_CHECK(ps->find(pu) != ps->end());
  BOOST_CHECK(ps->find(pd) != ps->end());
  BOOST_CHECK(ps->find(pl) != ps->end());
  BOOST_CHECK(ps->find(pr) != ps->end());

  BOOST_CHECK(!(p < p));
  BOOST_CHECK(pu < p);
  BOOST_CHECK(p < pd);
  BOOST_CHECK(pl < p);
  BOOST_CHECK(p < pr);
}

BOOST_AUTO_TEST_CASE(board_test) {
  Board b = Board(3);

  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      BOOST_CHECK_EQUAL(b.ref(Point(row, col)), empty);
    }
  }

  b.set(Point(1, 1), black);
  b.set(Point(1, 2), white);

  BOOST_CHECK_EQUAL(b.ref(Point(1, 0)), empty);
  BOOST_CHECK_EQUAL(b.ref(Point(1, 1)), black);
  BOOST_CHECK_EQUAL(b.ref(Point(1, 2)), white);
}

BOOST_AUTO_TEST_CASE(board_test_get_chain) {
  Board b = Board(5);

  b.set(Point(1, 1), white);
  b.set(Point(1, 2), white);
  b.set(Point(1, 3), white);
  b.set(Point(2, 1), white);
  b.set(Point(2, 2), black);
  b.set(Point(2, 3), white);
  b.set(Point(3, 1), black);
  b.set(Point(3, 3), black);
  b.set(Point(4, 1), black);
  b.set(Point(4, 2), black);
  b.set(Point(4, 3), black);

  Points ps;
  ps = b.get_chain(Point(2, 2));
  BOOST_CHECK(ps->size() == 1);
  BOOST_CHECK(ps->find(Point(2, 2)) != ps->end());

  ps = b.get_chain(Point(1, 1));
  BOOST_CHECK(ps->size() == 5);
  BOOST_CHECK(ps->find(Point(1, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(1, 2)) != ps->end());
  BOOST_CHECK(ps->find(Point(1, 3)) != ps->end());
  BOOST_CHECK(ps->find(Point(2, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(2, 3)) != ps->end());

  ps = b.get_chain(Point(3, 2));
  BOOST_CHECK(ps->size() == 0);

  ps = b.get_chain(Point(3, 3));
  BOOST_CHECK(ps->size() == 5);
  BOOST_CHECK(ps->find(Point(3, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(3, 3)) != ps->end());
  BOOST_CHECK(ps->find(Point(4, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(4, 2)) != ps->end());
  BOOST_CHECK(ps->find(Point(4, 3)) != ps->end());

  ps = b.get_chain(Point(5, 3));
  BOOST_CHECK(ps->size() == 0);
}
