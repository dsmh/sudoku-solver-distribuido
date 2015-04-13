#include <set>
#include <vector>
#include <iostream>
#include <utility>
#include "format.h"

using std::vector;
using std::pair;
using fmt::print;

using Coordinate = pair<size_t, size_t>;

class Sudoku {
private:
  using Board = vector<vector<int>>;
  Board board;

public:
  Sudoku() {
    board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
  }

  Sudoku(const vector<vector<int>>& s)
      : board(s) {}

  Sudoku(const Sudoku& other)
      : board(other.board) {}

  bool isSolved() const {
    for (size_t i = 0; i < 9; i++)
      for (size_t j = 0; j < 9; j++)
        if (board[i][j] == 0 || !check(i, j)) return false;
    return true;
  }

  bool check(size_t i, size_t j) const {
    return checkRow(i, j) && checkCol(i, j) && checkBox(i, j);
  }

  pair<bool, Coordinate> nextToSolve() const {
    for (size_t i = 0; i < 9; i++)
      for (size_t j = 0; j < 9; j++)
        if (board[i][j] == 0) return {false, {i, j}};
    return {true, {0, 0}};
  }

  void tryValue(Coordinate c, int v) { board[c.first][c.second] = v; }

private:
  bool checkRow(size_t i, size_t j) const {
    for (size_t x = 0; x < 9; x++) {
      if (x != j && board[i][j] == board[i][x]) {
        // fmt::print_colored(fmt::RED,
        //                    "[CheckRow] Failed for cell {} {} w.r.t. {} {} \n",
        //                    i, j, i, x);
        return false;
      }
    }
    return true;
  }
  bool checkCol(size_t i, size_t j) const {
    for (size_t row = 0; row < 9; row++) {
      if (row != i && board[i][j] == board[row][j]) {
        // fmt::print_colored(fmt::RED,
        //                    "[CheckCol] Failed for cell {} {} w.r.t. {} {} \n",
        //                    i, j, row, j);
        // fmt::print("Board cell: {} other value {}\n", board[i][j],
        //            board[row][j]);
        return false;
      }
    }
    return true;
  }
  bool checkBox(size_t i, size_t j) const {
    size_t boxStartRow = i - (i % 3);
    size_t boxStartCol = j - (j % 3);

    for (size_t row = 0; row < 3; row++)
      for (size_t col = 0; col < 3; col++) {
        size_t x = row + boxStartRow;
        size_t y = col + boxStartCol;
        if (!(x == i && y == j) && (board[i][j] == board[x][y])) return false;
      }
    return true;
  }

public:
  void print() const {
    fmt::print("Sudoku\n");
    size_t i = 0;
    for (const auto& row : board) {
      size_t j = 0;
      if (i % 3 == 0) fmt::print("{:-^13}\n", "");
      for (const auto& cell : row) {
        if (j % 3 == 0) fmt::print("|");
        fmt::print("{}", cell);
        j++;
      }
      fmt::print("|\n");
      i++;
    }
    fmt::print("{:-^13}\n", "");
  }
};

struct Statistics {
  size_t solutions;
  size_t failures;
  size_t decisions;
  size_t reductions;

  Statistics()
      : solutions(0)
      , failures(0)
      , decisions(0)
      , reductions(0) {}
  void print() const {
    fmt::print_colored(
        fmt::GREEN,
        "Solutions: {}\t Failures: {}\t Decisions: {}\t Reductions: {}\n",
        solutions, failures, decisions, reductions);
  }
};

pair<Sudoku, bool> solveOne(Sudoku& s, Statistics& st) {
  auto result = s.nextToSolve();
  bool done = result.first;
  if (done) {
    if (s.isSolved())
      return {s, true};
    else
      return {s, false};
  } else {
    Coordinate c = result.second;
    for (int i = 1; i <= 9; i++) {
      Sudoku copy(s);
      copy.tryValue(c,i);
      auto r = solveOne(copy,st);
      if (r.second) return r;
    }
    // No solution found
    return {s,false};
  }
}

void solve(const Sudoku& s) {
  Statistics st;
  Sudoku root(s);
  pair<Sudoku, bool> sol = solveOne(root, st);
  if (sol.second) {
    sol.first.print();
  } else {
    print("No solution found\n");
  }
  st.print();
}

int main(void) {
  fmt::print("Sudoku solver\n");
  /*
  Sudoku t({{1, 2, 7, 3, 5, 6, 8, 4, 9},
            {9, 3, 8, 2, 1, 4, 5, 6, 7},
            {4, 6, 5, 8, 9, 7, 3, 1, 2},
            {6, 7, 3, 5, 4, 2, 9, 8, 1},
            {8, 1, 9, 6, 7, 3, 2, 5, 4},
            {5, 4, 2, 1, 8, 9, 7, 3, 6},
            {2, 5, 6, 7, 3, 1, 4, 9, 8},
            {3, 9, 1, 4, 2, 8, 6, 7, 5},
            {7, 8, 4, 9, 6, 5, 1, 2, 3}});
  t.print();
  // t.checkCol(0, 1);
  fmt::print("Solution? {}\n", t.isSolved());
  */
  
  Sudoku a({{0, 0, 3, 0, 2, 0, 6, 0, 0},
            {9, 0, 0, 3, 0, 5, 0, 0, 1},
            {0, 0, 1, 8, 0, 6, 4, 0, 0},
            {0, 0, 8, 1, 0, 2, 9, 0, 0},
            {7, 0, 0, 0, 0, 0, 0, 0, 8},
            {0, 0, 6, 7, 0, 8, 2, 0, 0},
            {0, 0, 2, 6, 0, 9, 5, 0, 0},
            {8, 0, 0, 2, 0, 3, 0, 0, 9},
            {0, 0, 5, 0, 1, 0, 3, 0, 0}});
/*
  Sudoku b({{2, 0, 0, 0, 8, 0, 3, 0, 0},
            {0, 6, 0, 0, 7, 0, 0, 8, 4},
            {0, 3, 0, 5, 0, 0, 2, 0, 9},
            {0, 0, 0, 1, 0, 5, 4, 0, 8},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {4, 0, 2, 7, 0, 6, 0, 0, 0},
            {3, 0, 1, 0, 0, 7, 0, 4, 0},
            {7, 2, 0, 0, 4, 0, 0, 6, 0},
            {0, 0, 4, 0, 1, 0, 0, 0, 3}});

  Sudoku c({
      {0, 0, 0, 0, 0, 0, 9, 0, 7},
      {0, 0, 0, 4, 2, 0, 1, 8, 0},
      {0, 0, 0, 7, 0, 5, 0, 2, 6},
      {1, 0, 0, 9, 0, 4, 0, 0, 0},
      {0, 5, 0, 0, 0, 0, 0, 4, 0},
      {0, 0, 0, 5, 0, 7, 0, 0, 9},
      {9, 2, 0, 1, 0, 8, 0, 0, 0},
      {0, 3, 4, 0, 5, 9, 0, 0, 0},
      {5, 0, 7, 0, 0, 0, 0, 0, 0},
  });

  Sudoku d({
      {0, 3, 0, 0, 5, 0, 0, 4, 0},
      {0, 0, 8, 0, 1, 0, 5, 0, 0},
      {4, 6, 0, 0, 0, 0, 0, 1, 2},
      {0, 7, 0, 5, 0, 2, 0, 8, 0},
      {0, 0, 0, 6, 0, 3, 0, 0, 0},
      {0, 4, 0, 1, 0, 9, 0, 3, 0},
      {2, 5, 0, 0, 0, 0, 0, 9, 8},
      {0, 0, 1, 0, 2, 0, 6, 0, 0},
      {0, 8, 0, 0, 6, 0, 0, 2, 0},
  });

  Sudoku e({
      {0, 0, 0, 0, 5, 0, 0, 4, 0},
      {0, 0, 8, 0, 1, 0, 5, 0, 0},
      {4, 6, 0, 0, 0, 0, 0, 1, 2},
      {0, 7, 0, 5, 0, 2, 0, 8, 0},
      {0, 0, 0, 6, 0, 3, 0, 0, 0},
      {0, 4, 0, 1, 0, 9, 0, 3, 0},
      {2, 5, 0, 0, 0, 0, 0, 9, 8},
      {0, 0, 1, 0, 2, 0, 6, 0, 0},
      {0, 8, 0, 0, 6, 0, 0, 2, 0},
  });}*/

  solve(a);
  // solve(b);
  // solve(c);
  // solveAll(c);
  // solveAll(e);
  // solve(d);
  return 0;
}