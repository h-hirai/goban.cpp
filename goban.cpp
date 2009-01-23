#include "goban.hpp"

Point::Point(int row, int col) : row(row), col(col) {}

Point Point::up() {
  return Point(row - 1, col);
}

Point Point::down() {
  return Point(row + 1, col);
}

Point Point::left() {
  return Point(row, col - 1);
}

Point Point::right() {
  return Point(row, col + 1);
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
