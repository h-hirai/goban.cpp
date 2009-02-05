enum color_t {
  empty,
  black,
  white
};

class Point {
public:
  int row, col;
  Point(const int, const int);
  Point up() const;
  Point down() const;
  Point left() const;
  Point right() const;

  bool operator<(const Point) const;
};

class Board {
private:
  int size;
  color_t** brd_state;

public:
  Board(int size);
  ~Board();
  color_t ref(const Point);
  void set(const Point, color_t);
};
