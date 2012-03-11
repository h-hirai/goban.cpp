#include "goban.hpp"

using namespace std;

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

bool Point::operator==(const Point& p) const {
  return x == p.x && y == p.y;
}

std::ostream& operator<<(std::ostream& os, const color_t c) {
  switch (c) {
  case color_t::empty:
    os << "empty";
    break;
  case color_t::black:
    os << "black";
    break;
  case color_t::white:
    os << "white";
    break;
  case color_t::out_of_board:
    os << "out_of_board";
    break;
  }
  return os;
}
