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

Board::Board(int size) : size(size) {
  brd_state = new color_t*[size];
  for (int row = 0; row < size; row++) {
    brd_state[row] = new color_t[size];
    for (int col = 0; col < size; col++) {
      brd_state[row][col] = empty;
    }
  }
}

Board::~Board() {
  for (int row = 0; row < size; row ++) {
    delete[] brd_state[row];
  }
  delete[] brd_state;
}

color_t Board::ref(Point p) {
  return brd_state[p.row][p.col];
}

void Board::set(Point p, color_t c) {
  brd_state[p.row][p.col] = c;
}
