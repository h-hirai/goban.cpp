#include "goban.h"

#define BOOST_TEST_MODULE GobanTest
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(point_test) {
  BOOST_TEST_CHECKPOINT("reference x, y");

  Point p(3, 4);

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

  Board b(3);

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
  Board b(5);

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

  Board b1(5);
  Board b2(5);
  Board b3(5);

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
  b3[2][3] = black;
  b3[3][3] = black;
  b3[4][3] = black;
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

BOOST_AUTO_TEST_CASE(board_test_put_1) {
  Board b(5);
  int num_captured = b.put(Point(1, 3), black);
  BOOST_CHECK_EQUAL(num_captured, 0);
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) {
      color_t expect;
      if (x == 1 && y == 3) {
        expect = black;
      }
      else {
        expect = empty;
      }
      BOOST_CHECK_EQUAL(b[y][x], expect);
    }
  }
}

auto_ptr<Board> make_test_board_2() {
  auto_ptr<Board> b(new Board(5));
  (*b)[0][1] = black;
  (*b)[0][2] = white;
  (*b)[1][0] = black;
  (*b)[1][1] = white;
  (*b)[2][3] = black;
  (*b)[2][4] = white;
  (*b)[3][2] = black;
  (*b)[3][3] = white;
  (*b)[3][4] = black;
  (*b)[4][2] = black;
  (*b)[4][3] = white;
  return b;
}

BOOST_AUTO_TEST_CASE(board_test_put_2_1) {
  auto_ptr<Board> b = make_test_board_2();
  int num_captured = b->put(Point(4, 1), black);
  BOOST_CHECK_EQUAL(num_captured, 1);
  BOOST_CHECK_EQUAL((*b)[0][0], empty);
  BOOST_CHECK_EQUAL((*b)[0][1], black);
  BOOST_CHECK_EQUAL((*b)[0][2], white);
  BOOST_CHECK_EQUAL((*b)[0][3], empty);
  BOOST_CHECK_EQUAL((*b)[0][4], empty);
  BOOST_CHECK_EQUAL((*b)[1][0], black);
  BOOST_CHECK_EQUAL((*b)[1][1], white);
  BOOST_CHECK_EQUAL((*b)[1][2], empty);
  BOOST_CHECK_EQUAL((*b)[1][3], empty);
  BOOST_CHECK_EQUAL((*b)[1][4], black);
  BOOST_CHECK_EQUAL((*b)[2][0], empty);
  BOOST_CHECK_EQUAL((*b)[2][1], empty);
  BOOST_CHECK_EQUAL((*b)[2][2], empty);
  BOOST_CHECK_EQUAL((*b)[2][3], black);
  BOOST_CHECK_EQUAL((*b)[2][4], empty);
  BOOST_CHECK_EQUAL((*b)[3][0], empty);
  BOOST_CHECK_EQUAL((*b)[3][1], empty);
  BOOST_CHECK_EQUAL((*b)[3][2], black);
  BOOST_CHECK_EQUAL((*b)[3][3], white);
  BOOST_CHECK_EQUAL((*b)[3][4], black);
  BOOST_CHECK_EQUAL((*b)[4][0], empty);
  BOOST_CHECK_EQUAL((*b)[4][1], empty);
  BOOST_CHECK_EQUAL((*b)[4][2], black);
  BOOST_CHECK_EQUAL((*b)[4][3], white);
  BOOST_CHECK_EQUAL((*b)[4][4], empty);
}

BOOST_AUTO_TEST_CASE(board_test_put_2_2) {
  auto_ptr<Board> b = make_test_board_2();
  int num_captured = b->put(Point(4, 4), black);
  BOOST_CHECK_EQUAL(num_captured, 2);
  BOOST_CHECK_EQUAL((*b)[0][0], empty);
  BOOST_CHECK_EQUAL((*b)[0][1], black);
  BOOST_CHECK_EQUAL((*b)[0][2], white);
  BOOST_CHECK_EQUAL((*b)[0][3], empty);
  BOOST_CHECK_EQUAL((*b)[0][4], empty);
  BOOST_CHECK_EQUAL((*b)[1][0], black);
  BOOST_CHECK_EQUAL((*b)[1][1], white);
  BOOST_CHECK_EQUAL((*b)[1][2], empty);
  BOOST_CHECK_EQUAL((*b)[1][3], empty);
  BOOST_CHECK_EQUAL((*b)[1][4], empty);
  BOOST_CHECK_EQUAL((*b)[2][0], empty);
  BOOST_CHECK_EQUAL((*b)[2][1], empty);
  BOOST_CHECK_EQUAL((*b)[2][2], empty);
  BOOST_CHECK_EQUAL((*b)[2][3], black);
  BOOST_CHECK_EQUAL((*b)[2][4], white);
  BOOST_CHECK_EQUAL((*b)[3][0], empty);
  BOOST_CHECK_EQUAL((*b)[3][1], empty);
  BOOST_CHECK_EQUAL((*b)[3][2], black);
  BOOST_CHECK_EQUAL((*b)[3][3], empty);
  BOOST_CHECK_EQUAL((*b)[3][4], black);
  BOOST_CHECK_EQUAL((*b)[4][0], empty);
  BOOST_CHECK_EQUAL((*b)[4][1], empty);
  BOOST_CHECK_EQUAL((*b)[4][2], black);
  BOOST_CHECK_EQUAL((*b)[4][3], empty);
  BOOST_CHECK_EQUAL((*b)[4][4], black);
}

BOOST_AUTO_TEST_CASE(board_test_put_2_3) {
  auto_ptr<Board> b = make_test_board_2();
  int num_captured = b->put(Point(0, 0), white);
  BOOST_CHECK_EQUAL(num_captured, 1);
  BOOST_CHECK_EQUAL((*b)[0][0], white);
  BOOST_CHECK_EQUAL((*b)[0][1], empty);
  BOOST_CHECK_EQUAL((*b)[0][2], white);
  BOOST_CHECK_EQUAL((*b)[0][3], empty);
  BOOST_CHECK_EQUAL((*b)[0][4], empty);
  BOOST_CHECK_EQUAL((*b)[1][0], black);
  BOOST_CHECK_EQUAL((*b)[1][1], white);
  BOOST_CHECK_EQUAL((*b)[1][2], empty);
  BOOST_CHECK_EQUAL((*b)[1][3], empty);
  BOOST_CHECK_EQUAL((*b)[1][4], empty);
  BOOST_CHECK_EQUAL((*b)[2][0], empty);
  BOOST_CHECK_EQUAL((*b)[2][1], empty);
  BOOST_CHECK_EQUAL((*b)[2][2], empty);
  BOOST_CHECK_EQUAL((*b)[2][3], black);
  BOOST_CHECK_EQUAL((*b)[2][4], white);
  BOOST_CHECK_EQUAL((*b)[3][0], empty);
  BOOST_CHECK_EQUAL((*b)[3][1], empty);
  BOOST_CHECK_EQUAL((*b)[3][2], black);
  BOOST_CHECK_EQUAL((*b)[3][3], white);
  BOOST_CHECK_EQUAL((*b)[3][4], black);
  BOOST_CHECK_EQUAL((*b)[4][0], empty);
  BOOST_CHECK_EQUAL((*b)[4][1], empty);
  BOOST_CHECK_EQUAL((*b)[4][2], black);
  BOOST_CHECK_EQUAL((*b)[4][3], white);
  BOOST_CHECK_EQUAL((*b)[4][4], empty);

  BOOST_CHECK(!b->can_put(Point(1, 0), black));
}

BOOST_AUTO_TEST_CASE(board_test_canput_1) {
  Board b(7);
  b[0][1] = white;
  b[1][0] = white;
  b[1][3] = white;
  b[2][1] = white;
  b[3][0] = white;
  b[3][6] = black;
  b[4][5] = black;
  b[5][4] = black;
  b[5][6] = black;
  b[6][5] = black;

  BOOST_CHECK(!b.can_put(Point(1, 0), black));
  BOOST_CHECK(!b.can_put(Point(9, 0), black));
  BOOST_CHECK( b.can_put(Point(3, 3), black));
  BOOST_CHECK(!b.can_put(Point(0, 0), black));
  BOOST_CHECK( b.can_put(Point(0, 0), white));
}
