#include "goban.hpp"
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test_framework;

void point_test() {
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
}

void board_test() {
  Board b = Board(3);

  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      BOOST_CHECK_EQUAL(b.ref(Point(row, col)), empty);
    }
  }
}

test_suite* init_unit_test_suite(int argc, char* argv[]) {
  test_suite* test = BOOST_TEST_SUITE("goban test");
  test->add(BOOST_TEST_CASE(&point_test));
  test->add(BOOST_TEST_CASE(&board_test));
  return test;
}
