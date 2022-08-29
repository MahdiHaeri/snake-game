#include <iostream>
#include <time.h>       /* time */

using namespace std;

int main(int argc, char const *argv[]) {
  srand (time(NULL));

std::cout << rand() << '\n';

  // while (true) {
  //   char x;
  //   cin >> x;
  //   cout << int(x) << endl;
  // }
  return 0;
}
