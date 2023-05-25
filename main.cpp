#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Scrambler.h"

int main() {
  int size;
  cout << "Enter board size (3-9): " << endl;
  cin >> size;
  
  Scrambler board("dictionary.txt", size);
  cout << board.display_solution() << endl;
  do {
    string input;
    cout << "Enter move: (ex. c2d)" << endl;
    cin >> input;
    board.try_move(input);
    cout << board.str() << endl;
  } while (!board.is_over() && !cin.eof());
}