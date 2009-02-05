#include "goban.h"

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

bool Point::operator<(const Point p) const {
  return row < p.row || (row == p.row && col < p.col);
}

Board::Board(int size) :
  size(size),
  brd_state(vector<vector<color_t> >(size, vector<color_t>(size, empty))) {}

color_t Board::ref(Point p) {
  return brd_state[p.row][p.col];
}

void Board::set(Point p, color_t c) {
  brd_state[p.row][p.col] = c;
}
