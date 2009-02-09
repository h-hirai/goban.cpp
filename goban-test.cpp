#include "goban.h"

#define BOOST_TEST_MODULE GobanTest
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(point_test) {
  BOOST_TEST_CHECKPOINT("reference x, y");

  Point p = Point(3, 4);

  BOOST_CHECK_EQUAL(p.x, 3);
  BOOST_CHECK_EQUAL(p.y, 4);

  BOOST_TEST_CHECKPOINT("up, down, left, right");

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

  BOOST_TEST_CHECKPOINT("around");

  Points ps = p.around();
  BOOST_CHECK(ps->size() == 4);
  BOOST_CHECK(ps->find(pu) != ps->end());
  BOOST_CHECK(ps->find(pd) != ps->end());
  BOOST_CHECK(ps->find(pl) != ps->end());
  BOOST_CHECK(ps->find(pr) != ps->end());

  BOOST_TEST_CHECKPOINT("operator<");

  BOOST_CHECK(!(p < p));
  BOOST_CHECK(pu < p);
  BOOST_CHECK(p < pd);
  BOOST_CHECK(pl < p);
  BOOST_CHECK(p < pr);
}

BOOST_AUTO_TEST_CASE(board_test) {
  BOOST_TEST_CHECKPOINT("initialization");

  Board b = Board(3);

  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      BOOST_CHECK_EQUAL(b[Point(x, y)], empty);
    }
  }

  BOOST_TEST_CHECKPOINT("setting and reference");

  b[Point(1, 1)] = black;
  b[Point(1, 2)] = white;

  BOOST_CHECK_EQUAL(b[Point(1, 0)], empty);
  BOOST_CHECK_EQUAL(b[Point(1, 1)], black);
  BOOST_CHECK_EQUAL(b[Point(1, 2)], white);
}

BOOST_AUTO_TEST_CASE(board_test_get_chain) {
  Board b = Board(5);

  b[Point(1, 1)] = white;
  b[Point(1, 2)] = white;
  b[Point(1, 3)] = white;
  b[Point(2, 1)] = white;
  b[Point(2, 2)] = black;
  b[Point(2, 3)] = white;
  b[Point(3, 1)] = black;
  b[Point(3, 3)] = black;
  b[Point(4, 1)] = black;
  b[Point(4, 2)] = black;
  b[Point(4, 3)] = black;

  Points ps;

  BOOST_TEST_CHECKPOINT("alone stone");

  ps = b.get_chain(Point(2, 2));
  BOOST_CHECK(ps->size() == 1);
  BOOST_CHECK(ps->find(Point(2, 2)) != ps->end());

  BOOST_TEST_CHECKPOINT("many stones");

  ps = b.get_chain(Point(1, 1));
  BOOST_CHECK(ps->size() == 5);
  BOOST_CHECK(ps->find(Point(1, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(1, 2)) != ps->end());
  BOOST_CHECK(ps->find(Point(1, 3)) != ps->end());
  BOOST_CHECK(ps->find(Point(2, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(2, 3)) != ps->end());

  BOOST_TEST_CHECKPOINT("no stones");

  ps = b.get_chain(Point(3, 2));
  BOOST_CHECK(ps->size() == 0);

  BOOST_TEST_CHECKPOINT("many stones on an edge");

  ps = b.get_chain(Point(3, 3));
  BOOST_CHECK(ps->size() == 5);
  BOOST_CHECK(ps->find(Point(3, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(3, 3)) != ps->end());
  BOOST_CHECK(ps->find(Point(4, 1)) != ps->end());
  BOOST_CHECK(ps->find(Point(4, 2)) != ps->end());
  BOOST_CHECK(ps->find(Point(4, 3)) != ps->end());

  BOOST_TEST_CHECKPOINT("out of the board");

  ps = b.get_chain(Point(5, 3));
  BOOST_CHECK(ps->size() == 0);
}

BOOST_AUTO_TEST_CASE(board_test_alive_at) {
  BOOST_TEST_CHECKPOINT("preparing test data...");

  Board b1 = Board(5);
  Board b2 = Board(5);
  Board b3 = Board(5);

  b1[0][1] = black;
  b1[0][3] = black;
  b1[1][2] = black;
  b1[2][2] = black;
  b1[3][2] = black;
  b1[4][1] = black;
  b1[4][3] = black;
  b1[0][2] = white;
  b1[1][1] = white;
  b1[1][3] = white;
  b1[2][1] = white;
  b1[2][3] = white;
  b1[3][1] = white;
  b1[3][3] = white;
  b1[4][2] = white;

  b2[0][1] = black;
  b2[0][3] = black;
  b2[1][1] = black;
  b2[1][3] = black;
  b2[2][1] = black;
  b2[2][3] = black;
  b2[3][2] = black;
  b2[0][2] = white;
  b2[1][2] = white;
  b2[2][2] = white;
  b2[3][1] = white;
  b2[3][3] = white;

  b3[0][4] = black;
  b3[1][4] = black;
  b3[2][2] = black;
  b3[3][2] = black;
  b3[4][2] = black;
  b3[0][3] = white;
  b3[2][4] = white;
  b3[3][4] = white;
  b3[4][4] = white;

  BOOST_TEST_CHECKPOINT("preparing test data done");

  BOOST_CHECK( b1.alive_at(Point(1, 2)));
  BOOST_CHECK(!b1.alive_at(Point(2, 2)));
  BOOST_CHECK( b2.alive_at(Point(1, 2)));
  BOOST_CHECK(!b2.alive_at(Point(2, 2)));
  BOOST_CHECK( b3.alive_at(Point(4, 0)));
  BOOST_CHECK(!b3.alive_at(Point(4, 4)));
  BOOST_CHECK(!b3.alive_at(Point(0, 4)));
  BOOST_CHECK(!b3.alive_at(Point(5, 5)));
}
