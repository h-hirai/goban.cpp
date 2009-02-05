#include "goban.h"

//
// class Point

Point::Point(const int row, const int col) : row(row), col(col) {}

Point Point::up() const {
  return Point(row - 1, col);
}

Point Point::down() const {
  return Point(row + 1, col);
}

Point Point::left() const {
  return Point(row, col - 1);
}

Point Point::right() const {
  return Point(row, col + 1);
}

set<Point> Point::around() const {
  set<Point> ps;
  ps.insert(up());
  ps.insert(down());
  ps.insert(left());
  ps.insert(right());

  return ps;
}

bool Point::operator<(const Point p) const {
  return row < p.row || (row == p.row && col < p.col);
}

//
// class Board

Board::Board(const int size) :
  size(size),
  brd_state(vector<vector<color_t> >(size, vector<color_t>(size, empty))) {}

color_t Board::ref(const Point p) const {
  return brd_state[p.row][p.col];
}

void Board::set(const Point p, const color_t c) {
  brd_state[p.row][p.col] = c;
}
