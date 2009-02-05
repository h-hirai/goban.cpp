#include "goban.h"

#define BOOST_TEST_MODULE GobanTest
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(point_test) {
  Point p = Point(3, 4);

  BOOST_CHECK_EQUAL(p.row, 3);
  BOOST_CHECK_EQUAL(p.col, 4);

  Point pu = p.up();
  Point pd = p.down();
  Point pl = p.left();
  Point pr = p.right();

  BOOST_CHECK_EQUAL(pu.row, p.row - 1);
  BOOST_CHECK_EQUAL(pu.col, p.col);

  BOOST_CHECK_EQUAL(pd.row, p.row + 1);
  BOOST_CHECK_EQUAL(pd.col, p.col);

  BOOST_CHECK_EQUAL(pl.row, p.row);
  BOOST_CHECK_EQUAL(pl.col, p.col - 1);

  BOOST_CHECK_EQUAL(pr.row, p.row);
  BOOST_CHECK_EQUAL(pr.col, p.col + 1);

  auto_ptr<set<Point> > ps = p.around();
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
