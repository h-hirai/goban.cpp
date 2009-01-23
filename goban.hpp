enum color_t {
  empty,
  black,
  white
};

class Point {
public:
  int row, col;
  Point(int, int);
  Point up();
  Point down();
  Point left();
  Point right();
};

class Board {
private:
  int size;
  color_t** brd_state;

public:
  Board(int size);
  ~Board();
  color_t ref(const Point);
};
