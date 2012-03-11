#include "goban.hpp"

//
// class Point

Point::Point(const int x, const int y) : x(x), y(y) {}

inline Point Point::up() const    {return Point(x,     y - 1);}
inline Point Point::down() const  {return Point(x,     y + 1);}
inline Point Point::left() const  {return Point(x - 1, y);}
inline Point Point::right() const {return Point(x + 1, y);}

std::set<Point> Point::around() const {
  return {up(), down(), left(), right()};
}

bool Point::operator<(const Point& p) const {
  return y < p.y || (y == p.y && x < p.x);
}

bool Point::operator==(const Point& p) const {
  return x == p.x && y == p.y;
}


//
// enum class color_t

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
