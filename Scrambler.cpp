#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

#include "Scrambler.h"

// constructor (sets up the randomized board by calling multiple functions)
Scrambler::Scrambler(const string &path, const int size) {
  vector<string> loaded = load(path);
  board = setUpBoard(size, loaded);
  temp = formatBoard(board);
  temp = jumbleBoard(board);
}

// this function takes the parameter and opens the document
ifstream Scrambler::open(const string &src_file) {
  ifstream src{src_file};
  // if statement to check whether file is open or not.
  return src;
}

// this function takes in a parameter which is the list of 3-5 letter words and
// returns a vector of strings that have a word in each line
vector<string> Scrambler::load(string filename) {
  vector<string> words;

  ifstream file;
  file = open(filename);
  string line;

  getline(file, line);
  while (getline(file, line)) {
    words.push_back(line);
  }
  return words;
}

// this function randomly generates words based on the size of the board and
// adds them to a vector<vector<char>> variable by going through an algorithm
// provided by Professors David Hayes and George Maratos in pseudocode 
// this function also considers choosing proper words that will fit the row size
// properly with no more than two spaces in between
vector<vector<char>> Scrambler::setUpBoard(int size, vector<string> loaded) {
  vector<vector<char>> board(size, vector<char>(size, ' '));
  int row = 0;
  while (row < size) {
    int spacesRemaining = size;
    while (spacesRemaining >= 3) {
      // Select a random word from the list of words
      string word = loaded[rand() % loaded.size()];

      // Check if the word fits in the row
      if (word.length() <= spacesRemaining) {
        bool fits = true;
        int col = size - spacesRemaining;
        for (char letter : word) {
          if (board[row][col] != ' ' && board[row][col] != letter) {
            fits = false;
            break;
          }
          col++;
        }
        if (fits) {
          // Insert the word into the row
          col = size - spacesRemaining;
          genWords.push_back(word); // add word to public member vector for
                                    // get_words() function
          for (char letter : word) {
            board[row][col] = letter;
            col++;
          }
          spacesRemaining -= word.length();
          if (spacesRemaining > 0) {
            board[row][col] = ' ';
            spacesRemaining--;
          }
        }
      }
      if (spacesRemaining < 3) {
        break;
      }
    }
    row++;
  }

  // Add spacing between words in the same row
  for (int r = 0; r < size; r++) {
    int consecutiveSpaces = 0;
    for (int c = 0; c < size; c++) {
      if (board[r][c] == ' ') {
        consecutiveSpaces++;
      } else {
        if (consecutiveSpaces > 2) {
          board[r][c - consecutiveSpaces + 2] = ' ';
        }
        consecutiveSpaces = 0;
      }
    }
  }
  return board;
}

// this function changes the vector<vector<char>> parameter into a string which
// is formatted like the board
string getBoardAsString(const vector<vector<char>> &board) {
  int size = board.size();
  string formatted = " ";
  for (int c = 1; c <= size; c++) {
    formatted += "   " + to_string(c);
  }
  formatted += "\n";
  formatted += "   " + string(4 * size, '-') + "\n";
  for (int r = 0; r < size; r++) {
    formatted += to_string(r + 1) + " | ";
    for (int c = 0; c < size; c++) {
      formatted += board[r][c];
      formatted += " | ";
    }
    formatted.pop_back();
    formatted += "\n";
    formatted += "   " + string(4 * size, '-') + "\n";
  }
  formatted.pop_back();
  return formatted;
}

// This function takes a parameter vector<vector<char>> and formats it to show
// as a grid in the terminal when building the constructor
string Scrambler::formatBoard(const vector<vector<char>> &board) {
  string formatted = getBoardAsString(board);
  unshuffled = formatted; // public string member  that saves the unscrambled version
  return formatted;
}

// this function displays the board in the terminal for the user to see after
// they make a move using the getBoardAsString() function
string Scrambler::str() const {
  string formatted = getBoardAsString(board);
  return formatted;
}

// this function takes the move that the user wants to make as a parameter and
// shifts the correct column or row in the right direction using shift functions 
// this function also checks whether the user wants to restart the game by 
// checking if the input is R/r
void Scrambler::try_move(const string &command) {
  // if else statement to check the choice that the user made
  if (command == "r" || command == "R") {
    board = ogShuffleboard; // change board to originally shuffled board
    return;
  } 
  else {
    int col_row = command[1] - '0' -
                  1; // convert char to int, subtract 1 for zero-based indexing
    char direction = command[2];
    int size = board.size();

    // if else statement for column or row
    if (command[0] == 'c') {
      // if statement for improper column number
      if (col_row < 0 || col_row >= size) {
        cout << "Invalid column index." << endl;
        return;
      }
      // if else statement for direction
      if (direction == 'u' || direction == 'd') {
        shiftColumn(board, col_row, direction);
      } 
      else {
        cout << "Invalid direction." << endl;
        return;
      }
    } 
    else if (command[0] == 'r') {
      // if statement for improper column number
      if (col_row < 0 || col_row >= size) {
        cout << "Invalid row index." << endl;
        return;
      } 
      // if else statement for direction
      if (direction == 'l' || direction == 'r') {
        shiftRow(board, col_row, direction);
      }  
      else {
        cout << "Invalid direction." << endl;
        return;
      }
    } 
    else {
      cout << "Invalid command." << endl;
      return;
    }
  }
}

// this function checks whether the board at the current state matches the
// unscrambled version of the board by comparing the string versions
bool Scrambler::is_over() const {
  if (getBoardAsString(board) == unshuffled) {
    return true;
  } else {
    return false;
  }
}

// this function returns a vector<string> that contains the unscrambled words
// that are in the board
vector<string> Scrambler::get_words() const {
  for (int i = 0; i < genWords.size(); i++) {
    cout << genWords.at(i) << endl;
  }
  return genWords;
}

// this function shifts a column in the given direction without duplicating or
// replacing letters
void Scrambler::shiftColumn(vector<vector<char>> &board, int col, char dir) {
  int size = board.size();
  vector<char> colLetters;
  for (int i = 0; i < size; i++) {
    colLetters.push_back(board[i][col]);
  }

  // if else statement for direction
  if (dir == 'u') {
    for (int i = 0; i < size; i++) {
      board[i][col] = colLetters[(i + 1) % size];
    }
  } else {
    for (int i = size - 1; i >= 0; i--) {
      board[i][col] = colLetters[(i - 1 + size) % size];
    }
  }
}

// this function shifts a column in the given direction without duplicating or
// replacing letters
void Scrambler::shiftRow(vector<vector<char>> &board, int row, char dir) {
  int size = board.size();
  vector<char> rowLetters;
  for (int i = 0; i < size; i++) {
    rowLetters.push_back(board[row][i]);
  }

  // if else statement for direction
  if (dir == 'l') {
    for (int i = 0; i < size; i++) {
      board[row][i] = rowLetters[(i + 1) % size];
    }
  } else {
    for (int i = size - 1; i >= 0; i--) {
      board[row][i] = rowLetters[(i - 1 + size) % size];
    }
  }
}

// this function jumbles/shuffles the board and returns it as a string using an
// algorithm provided by Professors David Hayes and George Maratos in
// pseudocode. this function also collects the randomly generated moves and
// adds/reverses them to vector<string>'s in different formats to help make the
// display_soluion() function
string Scrambler::jumbleBoard(vector<vector<char>> &board) {
  // Randomly shuffle the board
  int size = board.size();
  int nmoves = rand() % 8 + 3;
  for (int i = 1; i <= nmoves; i++) {
    int idx = rand() % size;
    if (i == 1) {
      idx = 0;
    }
    if (idx % 2 == 0) {
      char dir = rand() % 2 == 0 ? 'u' : 'd';
      // if else statement for display_solutions() function
      if (dir == 'u') {
        string add = "(c," + to_string(idx + 1) + ",d)";
        string add2 = "c" + to_string(idx + 1) + "d";
        string add3 = "c" + to_string(idx + 1) + "u";
        solution.push_back(add);
        output.push_back(add2);
        scramble.push_back(add3);
      } else {
        string add = "(c," + to_string(idx + 1) + ",u)";
        string add2 = "c" + to_string(idx + 1) + "u";
        string add3 = "c" + to_string(idx + 1) + "d";
        solution.push_back(add);
        output.push_back(add2);
        scramble.push_back(add3);
      }
      shiftColumn(board, idx, dir);
    } else {
      char dir = rand() % 2 == 0 ? 'r' : 'l';
      // if else statement for display_solutions() function
      if (dir == 'r') {
        string add = "(r," + to_string(idx + 1) + ",l)";
        string add2 = "r" + to_string(idx + 1) + "l";
        string add3 = "r" + to_string(idx + 1) + "r";
        solution.push_back(add);
        output.push_back(add2);
        scramble.push_back(add3);
      } else {
        string add = "(r," + to_string(idx + 1) + ",r)";
        string add2 = "r" + to_string(idx + 1) + "r";
        string add3 = "r" + to_string(idx + 1) + "l";
        solution.push_back(add);
        output.push_back(add2);
        scramble.push_back(add3);
      }
      shiftRow(board, idx, dir);
    }
  }
  ogShuffleboard = board;
  // Create a formatted string representation of the board
  string formatted = getBoardAsString(board);
  original = formatted;
  return formatted;
}

// this function creates a string that shows the user how to solve the scrambled
// board, then lets the user try it themselves
string Scrambler::display_solution() {
  string displaySol;
  int num = 1;
  displaySol += original;
  displaySol += "\n";

  // for loop to add each move line and turn to the string
  for (int i = (solution.size() - 1); i >= 0; i--) {
    displaySol += "*** Move " + to_string(num) + " " + solution.at(i);
    displaySol += "\n";
    num += 1;

    // if else statement to shift the board before adding it as a string to
    // displaySol
    if (output.at(i)[0] == 'r') {
      shiftRow(board, int(output.at(i)[1] - '0' - 1), output.at(i)[2]);
    } else {
      shiftColumn(board, int(output.at(i)[1] - '0' - 1), output.at(i)[2]);
    }

    displaySol += getBoardAsString(board);
    displaySol += "\n";
  }

  // revert back to shuffled board
  for (int i = 0; i < solution.size(); i++) {
    if (scramble.at(i)[0] == 'r') {
      shiftRow(board, int(scramble.at(i)[1] - '0' - 1), scramble.at(i)[2]);
    } else {
      shiftColumn(board, int(scramble.at(i)[1] - '0' - 1), scramble.at(i)[2]);
    }
  }
  return displaySol;
}