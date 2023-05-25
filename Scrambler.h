#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef SCRAMBLER_H
#define SCRAMBLER_H

class Scrambler {
public:
  ifstream open(const string &src_file);
  Scrambler(const string &path, const int size);

  vector<string> load(string filename);
  vector<vector<char>> setUpBoard(const int size, vector<string> loaded);
  vector<string> genWords;

  string formatBoard(const vector<vector<char>> &board);
  vector<vector<char>> board;

  string original;
  string temp;
  string unshuffled;

  string str() const;
  vector<string> get_words() const;

  string jumbleBoard(vector<vector<char>> &board);
  void shiftRow(vector<vector<char>> &board, int row, char dir);
  void shiftColumn(vector<vector<char>> &board, int col, char dir);
  vector<string> solution;
  vector<string> output;
  vector<string> scramble;

  void try_move(const string &cmd);
  bool is_over() const;
  vector<vector<char>> ogShuffleboard;
  string display_solution();
};

#endif