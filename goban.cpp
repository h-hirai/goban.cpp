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

Points Point::around() const {
  Points ps(new set<Point>);
  ps->insert(up());
  ps->insert(down());
  ps->insert(left());
  ps->insert(right());

  return ps;
}

bool Point::operator<(const Point& p) const {
  return row < p.row || (row == p.row && col < p.col);
}

bool Point::out_of_board(const int size) const {
  return row < 0 || col < 0 || row >= size || col >= size;
}

//
// class Board

Board::Board(const int size) :
  size(size),
  brd_state(vector<vector<color_t> >(size, vector<color_t>(size, empty))) {}

color_t Board::ref(const Point& p) const {
  return brd_state[p.row][p.col];
}

void Board::set(const Point& p, const color_t c) {
  brd_state[p.row][p.col] = c;
}

Points Board::get_chain(const Point& p) const {
  Points ps(new std::set<Point>);

  if (p.out_of_board(size)) {
    return ps;
  }
  else if (ref(p) == empty) {
    return ps;
  }
  else {
    get_chain_aux get_points_of_chain(this, ref(p));
    return get_points_of_chain(ps, p);
  }
}

Board::get_chain_aux::get_chain_aux(const Board* board,
                                    const color_t chain_color) :
  board(board),
  chain_color(chain_color) {}

Points Board::get_chain_aux::operator()(Points ps, const Point& p) const {
  if (p.out_of_board(board->size)) {
    return ps;
  }
  else if (board->ref(p) != chain_color) {
    return ps;
  }
  else if (ps->find(p) != ps->end()) {
    return ps;
  }
  else {
    Points aps = p.around();
    ps->insert(p);
    return accumulate(aps->begin(), aps->end(), ps, *this);
  }
}
