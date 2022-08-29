#include <iostream>
#include <queue>

using namespace std;

#define WIDTH 30
#define HEIGHT 20
#define WALL '#'
#define SPACE ' '
#define FOOD '+'
#define BODY '*'

#define UP 119
#define DOWN 115
#define LEFT 97
#define RIGHT 100

class Map {
public:

  Map() {
    config_map();
  }

  void update_map(int i, int j, char character) {
    map[i][j] = character;
  }

  void config_map() {
    for (int j = 0; j < HEIGHT; j++) {
      for (int i = 0; i < WIDTH; i++) {
        if (is_wall(i, j)) {
          map[i][j] = WALL;
        } else {
          map[i][j] = SPACE;
        }
      }
    }
  }

  void print_map() {
    for (int j = 0; j < HEIGHT; j++) {
      cout << endl;
      for (int i = 0; i < WIDTH; i++) {
        cout << map[i][j] << map[i][j];
      }
    }
    cout << endl;
  }

  bool is_wall(int i, int j) {
    if (i == 0 || j == 0 || i == (WIDTH - 1) || j == (HEIGHT - 1)) {
      return true;
    }
    return false;
  }

private:
  char map[WIDTH][HEIGHT];

};

class Snake {
public:

  Snake(int _i, int _j, Map* _map) {
    i = _i;
    j = _j;
    length = 0;
    map = _map;
    map->update_map(i, j, BODY);
  }

  void move(int direction) {
    tail.push(pair(i, j));
    switch (direction) {
      case UP:
        j--;
        break;
      case DOWN:
        j++;
        break;
      case LEFT:
        i--;
        break;
      case RIGHT:
        i++;
        break;
      default:
        cout << "there is an error in Snake >> move" << endl;
    }
    map->update_map(i, j, BODY);
    move_tail();
  }

  void move_tail() {
    pair<int, int> moved_tail = tail.back();
    map->update_map(moved_tail.first, moved_tail.second, SPACE);
  }

int i;
int j;
int length;
queue<pair<int, int>> tail;
Map* map;

private:

};

class Food {
public:

private:

};

class Game_play {
public:

private:

};

int main(int argc, char const *argv[]) {
  Map main_map;
  Snake snake(WIDTH / 2, HEIGHT / 2, &main_map);
  while (true) {
    char x;
    cin >> x;
    snake.move(int(x));
    main_map.print_map();
  }
  return 0;
}
