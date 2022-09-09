#include <iostream>
#include <queue>
#include <random>
#include <time.h>       /* time */
#include <string>
#include <stdlib.h>


#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

#define WIDTH 30
#define HEIGHT 20
#define WALL '#'
#define SPACE ' '
#define FOOD 'f'
#define BODY '*'

#define UP 119
#define DOWN 115
#define LEFT 97
#define RIGHT 100

class Food;
class Snake;
class Map;

// ----------------------------------- additional function -----------------------------

void gotoxy(int x, int y) {
  printf("%c[%d;%df", 0x1B, y, x);
}

bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

//------------------------------------------------------------------------------


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

  void print_map(int length) {
    // gotoxy(0, 0);
    system("clear");
    for (int j = 0; j < HEIGHT; j++) {
      cout << endl;
      for (int i = 0; i < WIDTH; i++) {
        if (map[i][j] == BODY) {
          cout << map[i][j]<< ' ' ;
        } else {
          cout << map[i][j] << map[i][j];
        }
      }
    }
    cout << endl;
    cout << "length: " << length << endl;
  }

  bool is_wall(int i, int j) {
    if (i == 0 || j == 0 || i == (WIDTH - 1) || j == (HEIGHT - 1)) {
      return true;
    }
    return false;
  }

  bool is_snake(int i, int j) {
    if (map[i][j] == BODY) {
      return true;
    }
    return false;
  }

  bool is_space(int i, int j) {
    if (map[i][j] == SPACE) {
      return true;
    }
    return false;
  }

  bool is_food(int i, int j) {
    if (map[i][j] == FOOD) {
      return true;
    }
    return false;
  }

  bool is_dead(int i, int j) {
    if (is_wall(i, j) || is_snake(i, j)) {
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

  void move_head(int i, int j) {
    map->update_map(i, j, BODY);
  }

  pair<int, int> get_target(int direction) {
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
    return pair(i, j);
  }

  void move_tail() {
    pair<int, int> moved_tail = tail.front();
    tail.pop();
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
  Food(Map* _map) {
    map = _map;
    random_food();
  }

  void random_food() {
    do {
      i = (rand() % WIDTH + 1);
      j = (rand() % HEIGHT + 1);
    } while (!map->is_space(i, j));
    map->update_map(i, j, FOOD);
  }

  int i;
  int j;
  Map* map;
private:

};

class Game_play {
public:
  Game_play(Map* _map, Snake* _snake, Food* _food) {
    map = _map;
    snake = _snake;
    food = _food;
  }

  bool play() {
    if (kbhit()) {
      int current_direction = int(getch());
      if (currect_input(current_direction) && check_direction(current_direction)) {
        direction = current_direction;
      }
      // direction = int(getch());
    }
    pair<int, int> target = snake->get_target(direction);
    int i = target.first;
    int j = target.second;

    if (map->is_dead(i, j)) {
      return false;
    }

    if (map->is_food(i, j)) {
      ++snake->length;
      food->random_food();
    } else {
      snake->move_tail();
    }
    snake->move_head(i, j);
    map->print_map(snake->length);
    return true;
  }

  bool check_direction(int current_direction) {
    if ((direction == current_direction || direction == UP && current_direction == DOWN) || (direction == DOWN && current_direction == UP) || (direction == LEFT && current_direction == RIGHT) || (direction == RIGHT && current_direction == LEFT)) {
      return false;
    }
    return true;
  }

  bool currect_input(int current_direction) {
    if (current_direction == UP || current_direction == DOWN || current_direction == LEFT || current_direction == RIGHT) {
      return true;
    }
    return false;
  }

  Map* map;
  Food* food;
  Snake* snake;
  int direction = UP;
private:

};

int main(int argc, char const *argv[]) {
  srand (time(NULL));

  Map map;
  Snake snake(WIDTH / 2, HEIGHT / 2, &map);
  Food food(&map);

  Game_play game_play(&map, &snake, &food);
  map.print_map(snake.length);
  while (game_play.play()) {
    float length = snake.length;
    float time = 2.0 - (length / 10);
    cout <<"speed: " << (length / 10) << endl;
    usleep(time * 100000);
  }
  cout << "game over!" << endl;
  return 0;
}
