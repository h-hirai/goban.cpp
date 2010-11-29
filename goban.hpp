// -*- c++ -*-

#include <tr1/array>
#include <set>
#include <tr1/memory>
#include <numeric>

enum color_t {
  empty,
  black,
  white,
  out_of_board
};

class Point;
typedef std::tr1::shared_ptr<std::set<Point> > Points;

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

template<size_t N>
class Board {
private:
  std::tr1::array<std::tr1::array<color_t, N>, N> brd_state;
  bool ko_exist;
  Point ko_point;

  class get_chain_aux {
  private:
    const Board<N>& board;
    const color_t chain_color;
  public:
    get_chain_aux(const Board<N>& board, const color_t chain_color) :
      board(board),
      chain_color(chain_color) {}

    Points operator()(Points ps, const Point& p) const {
      if (board[p] != chain_color) {
        return ps;
      }
      else if (ps->find(p) != ps->end()) {
        return ps;
      }
      else {
        const Points aps = p.around();
        ps->insert(p);
        return accumulate(aps->begin(), aps->end(), ps, *this);
      }
    }
  };

public:
  Board() : ko_exist(false), ko_point(Point(N, N)) {
    for (unsigned int y = 0; y < N; y++) {
      for (unsigned int x = 0; x < N; x++) {
        brd_state[y][x] = empty;
      }
    }
  }

  Board(const Board<N>& other) :
    brd_state(other.brd_state),
    ko_exist(other.ko_exist),
    ko_point(other.ko_point) {}

  std::tr1::array<color_t, N>& operator[](const int y) {
    return brd_state[y];
  }

  color_t& operator[](const Point& p) {
    return brd_state[p.y][p.x];
  }

  color_t operator[](const Point& p) const {
    if (p.x < 0 ||
        p.y < 0 ||
        p.x >= static_cast<int>(N) ||
        p.y >= static_cast<int>(N)) {
      return out_of_board;
    }
    else {
      return brd_state[p.y][p.x];
    }
  }

  Points get_chain(const Point& p) const {
    Points ps(new std::set<Point>);
    const color_t c = (*this)[p];

    if (c == out_of_board || c == empty) {
      return ps;
    }
    else {
      get_chain_aux get_points_of_chain(*this, c);
      return get_points_of_chain(ps, p);
    }
  }

  bool alive_at(const Point& p) const {
    const Points ps = get_chain(p);
    for (std::set<Point>::const_iterator i = ps->begin();
         i != ps->end();
         i++) {
      const Points aps = i->around();
      for(std::set<Point>::const_iterator j = aps->begin();
          j != aps->end();
          j++) {
        if ((*this)[*j] == empty) return true;
      }
    }
    return false;
  }

  int put(const Point& p, const color_t c) {
    (*this)[p] = c;
    const Points aps = p.around();
    Points captured(new std::set<Point>);
    for (std::set<Point>::const_iterator i = aps->begin();
         i != aps->end();
         i++) {
      const color_t neighbor = (*static_cast<const Board*>(this))[*i];
      if (neighbor != c &&
          neighbor != empty &&
          neighbor != out_of_board &&
          !alive_at(*i)) {
        const Points opponents = get_chain(*i);
        for (std::set<Point>::const_iterator j = opponents->begin();
             j != opponents->end();
             j++) {
          captured->insert(*j);
        }
      }
    }

    if (captured->size() == 1 &&
        get_chain(p)->size() == 1 &&
        !alive_at(p)) {
      ko_exist = true;
      ko_point = *(captured->begin());
    }
    else {
      ko_exist = false;
    }

    for (std::set<Point>::const_iterator i = captured->begin();
         i != captured->end();
         i++) {
      (*this)[*i] = empty;
    }

    return captured->size();
  }

  bool can_put(const Point& p, const color_t c) const {
    if ((*this)[p] != empty) {
      return false;
    }
    else if (ko_exist && ko_point == p) {
      return false;
    }
    else {
      Board b = (*this);
      b[p] = c;
      if (b.alive_at(p)) {
        return true;
      }
      else {
        const Points aps = p.around();
        for (std::set<Point>::const_iterator i = aps->begin();
             i != aps->end();
             i++) {
          const color_t neighbor = static_cast<const Board>(b)[*i];
          if (neighbor != c && neighbor != out_of_board && !b.alive_at(*i)) {
            return true;
          }
        }
        return false;
      }
    }
  }
};
