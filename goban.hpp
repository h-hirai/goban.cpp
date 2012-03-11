// -*- c++ -*-

#include <array>
#include <set>
#include <memory>
#include <numeric>
#include <iostream>
#include <cassert>

enum class color_t {
  empty,
  black,
  white,
  out_of_board
};

class Point;
typedef std::shared_ptr<std::set<Point> > Points;
typedef std::shared_ptr<Point> Point_ptr;

class Point {
public:
  const int x, y;
  Point(const int, const int);

  Point up() const;
  Point down() const;
  Point left() const;
  Point right() const;

  std::set<Point> around() const;

  bool operator<(const Point&) const;
  bool operator==(const Point&) const;
};

template<size_t N>
class Board {
private:
  std::array<std::array<color_t, N>, N> brd_state;
  Point_ptr ko_point;

  class Chain {
  private:
    const Board<N>& board;

    class get_chain_aux {
    private:
      const Board<N>& board;
      const color_t chain_color;
    public:
      get_chain_aux(const Board<N>& _board, const color_t _chain_color)
        : board(_board)
        , chain_color(_chain_color)
      {}

      Points operator()(Points ps, const Point& p) const {
        if (board[p] != chain_color) {
          return ps;
        }
        else if (ps->find(p) != ps->end()) {
          return ps;
        }
        else {
          const std::set<Point> aps = p.around();
          ps->insert(p);
          return accumulate(aps.begin(), aps.end(), ps, *this);
        }
      }
    };

    Points get_chain(const Board<N>& board, const Point& p) const {
      Points ps(new std::set<Point>);
      const color_t c = board[p];

      if (c == color_t::out_of_board || c == color_t::empty) {
        return ps;
      }
      else {
        get_chain_aux get_points_of_chain(board, c);
        return get_points_of_chain(ps, p);
      }
    }

  public:
    const Points points;

    Chain(const Board<N>& _board, const Point& p)
      : board(_board)
      , points(get_chain(_board, p))
    {}

    size_t size() const {
      return points->size();
    }

    bool is_alive() const {
      for (auto i : *points) {
        for(auto j : i.around()) {
          if (board[j] == color_t::empty) return true;
        }
      }
      return false;
    }
  };

public:
  Board() {
    for (unsigned int y = 0; y < N; y++) {
      for (unsigned int x = 0; x < N; x++) {
        brd_state[y][x] = color_t::empty;
      }
    }
  }

  Board(const Board<N>& other) :
    brd_state(other.brd_state),
    ko_point(other.ko_point) {}

  std::array<color_t, N>& operator[](const int y) {
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
      return color_t::out_of_board;
    }
    else {
      return brd_state[p.y][p.x];
    }
  }

  int put(const Point& p, const color_t c) {
    (*this)[p] = c;
    Points captured(new std::set<Point>);
    for (auto i : p.around()) {
      const color_t neighbor = (static_cast<const Board>(*this))[i];
      const Chain opponents(*this, i);
      if (neighbor != c &&
          neighbor != color_t::empty &&
          neighbor != color_t::out_of_board &&
          !opponents.is_alive()) {
        for (auto j : *(opponents.points)) {
          captured->insert(j);
        }
      }
    }

    const Chain put_chain(*this, p);
    if (captured->size() == 1 &&
        put_chain.size() == 1 &&
        !put_chain.is_alive()) {
      ko_point = Point_ptr(new Point(*(captured->begin())));
    }
    else {
      ko_point = Point_ptr();
    }

    for (auto i : *captured) {
      (*this)[i] = color_t::empty;
    }

    return captured->size();
  }

  bool can_put(const Point& p, const color_t c) const {
    if ((*this)[p] != color_t::empty) {
      return false;
    }
    else if (ko_point && *ko_point == p) {
      return false;
    }
    else {
      Board b = (*this);
      b[p] = c;
      const Chain point_chain(b, p);
      if (point_chain.is_alive()) {
        return true;
      }
      else {
        for (auto i : p.around()) {
          const color_t neighbor = static_cast<const Board>(b)[i];
          const Chain neighbor_chain(b, i);
          if (neighbor != c &&
              neighbor != color_t::out_of_board &&
              !neighbor_chain.is_alive()) {
            return true;
          }
        }
        return false;
      }
    }
  }
};

template <size_t N>
std::ostream& operator<<(std::ostream& os, const Board<N> b) {
  for (size_t y = 0; y < N; y++) {
    for (size_t x = 0; x < N; x++) {
      switch (b[Point(x, y)]) {
      case color_t::empty:
        os << '.';
        break;
      case color_t::black:
        os << 'x';
        break;
      case color_t::white:
        os << 'o';
        break;
      case color_t::out_of_board:
        assert(false); // should never reach here
      }
    }
    os << std::endl;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const color_t c);
