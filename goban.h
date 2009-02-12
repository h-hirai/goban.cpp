// -*- c++ -*-

#include <vector>
#include <set>
#include <memory>
#include <numeric>

using namespace std;

enum color_t {
  empty,
  black,
  white,
  out_of_board
};

class Point;
typedef auto_ptr<set<Point> > Points;

class Point {
public:
  int x, y;
  Point(const int, const int);

  Point up() const;
  Point down() const;
  Point left() const;
  Point right() const;

  Points around() const;

  bool operator<(const Point&) const;
  bool operator==(const Point&) const;
};

class Board {
private:
  const int size;
  vector<vector<color_t> > brd_state;
  bool ko_exist;
  Point ko_point;
  class get_chain_aux {
  private:
    const Board& board;
    const color_t chain_color;
  public:
    get_chain_aux(const Board&, const color_t);
    Points operator()(Points, const Point&) const;
  };

public:
  Board(const int);
  Board(const Board&);

  vector<color_t>& operator[](const int);
  color_t& operator[](const Point&);
  color_t operator[](const Point&) const;

  Points get_chain(const Point&) const;
  bool alive_at(const Point&) const;
  int put(const Point&, color_t);
  bool can_put(const Point&, color_t) const;
};
