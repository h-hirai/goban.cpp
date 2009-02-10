#include "goban.h"

//
// class Point

Point::Point(const int x, const int y) : x(x), y(y) {}

Point Point::up() const    {return Point(x,     y - 1);}
Point Point::down() const  {return Point(x,     y + 1);}
Point Point::left() const  {return Point(x - 1, y);}
Point Point::right() const {return Point(x + 1, y);}

Points Point::around() const {
  Points ps(new set<Point>);
  ps->insert(up());
  ps->insert(down());
  ps->insert(left());
  ps->insert(right());

  return ps;
}

bool Point::operator<(const Point& p) const {
  return y < p.y || (y == p.y && x < p.x);
}

//
// class Board

Board::Board(const int size) :
  size(size),
  brd_state(vector<vector<color_t> >(size, vector<color_t>(size, empty))) {}

vector<color_t>& Board::operator[](const int y) {return brd_state[y];}
color_t& Board::operator[](const Point& p)      {return brd_state[p.y][p.x];}
color_t Board::operator[](const Point& p) const {
  if (p.x < 0 || p.y < 0 || p.x >= size || p.y >= size) {
    return out_of_board;
  }
  else {
    return brd_state[p.y][p.x];
  }
}

Points Board::get_chain(const Point& p) const {
  Points ps(new set<Point>);
  color_t c = (*this)[p];

  if (c == out_of_board || c == empty) {
    return ps;
  }
  else {
    get_chain_aux get_points_of_chain(*this, c);
    return get_points_of_chain(ps, p);
  }
}

Board::get_chain_aux::get_chain_aux(const Board& board,
                                    const color_t chain_color) :
  board(board),
  chain_color(chain_color) {}

Points Board::get_chain_aux::operator()(Points ps, const Point& p) const {
  if (board[p] != chain_color) {
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

bool Board::alive_at(const Point& p) const {
  Points ps = get_chain(p);
  for (set<Point>::iterator i = ps->begin(); i != ps->end(); i++) {
    Points aps = i->around();
    for(set<Point>::iterator j = aps->begin(); j != aps->end(); j++) {
      if ((*this)[*j] == empty) return true;
    }
  }
  return false;
}

int Board::put(const Point& p, color_t c) {
  (*this)[p] = c;
  return 0;
}
