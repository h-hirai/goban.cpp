// -*- c++ -*-

#include <vector>
#include <set>
#include <memory>
#include <numeric>

using namespace std;

enum color_t {
  empty,
  black,
  white
};

class Point;
typedef auto_ptr<set<Point> > Points;

class Point {
public:
  const int row, col;
  Point(const int, const int);

  Point up() const;
  Point down() const;
  Point left() const;
  Point right() const;

  Points around() const;

  bool operator<(const Point&) const;
  bool out_of_board(const int) const;
};

class Board {
public:
  const int size;
private:
  vector<vector<color_t> > brd_state;
  class get_chain_aux {
  private:
    const Board& board;
    const color_t chain_color;
  public:
    get_chain_aux(const Board&, const color_t);
    Points operator()(Points, const Point&) const;
  };

public:
  Board(const int size);

  color_t ref(const Point&) const;
  void set(const Point&, const color_t);

  Points get_chain(const Point&) const;
};
