#include <set>
#include <vector>
#include <iostream>
#include <utility>
#include "format.h"

using std::set;
using std::vector;
using std::pair;
using fmt::print;

using Coordinate = pair<size_t, size_t>;

class Sudoku {
private:
  using Cell = set<int>;
  using Board = vector<vector<Cell>>;

  Board board;

public:
  Sudoku() {
    Cell all{1, 2, 3, 4, 5, 6, 7, 8, 9};
    board = {
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
        {all, all, all, all, all, all, all, all, all},
    };
  }

  Sudoku(const vector<vector<int>>& s) {
    Cell all{1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (const auto& row : s) {
      board.push_back(vector<Cell>());
      for (const auto& v : row) {
        if (v == 0)
          board.back().push_back(all);
        else
          board.back().push_back({v});
      }
    }
  }

  Sudoku(const Sudoku& other)
      : board(other.board) {}

  bool isSolved() const {
    for (const auto& row : board)
      for (const auto& cell : row)
        if (cell.size() > 1) return false;
    return true;
  }

  bool isFailed() const {
    for (const auto& row : board)
      for (const auto& cell : row)
        if (cell.size() == 0) return true;
    return false;
  }

  bool solvedCell(size_t i, size_t j) const { return board[i][j].size() == 1; }

  int valueCell(size_t i, size_t j) const {
    assert(solvedCell(i, j));
    return *(board[i][j].begin());
  }

  void reduce() {
    bool r = true;
    while (r) {
      r = false;
      for (size_t x = 0; x < 9; x++)
        for (size_t y = 0; y < 9; y++) {
          r = r || reduceRow(x, y);
          r = r || reduceCol(x, y);
          r = r || reduceBox(x, y);
        }
      // fmt::print_colored(fmt::YELLOW, "Finished pass\n");
    }
  }

  Coordinate nextCellTosolve() const {
    for (size_t x = 0; x < 9; x++)
      for (size_t y = 0; y < 9; y++)
        if (!solvedCell(x, y)) return {x, y};
    assert(false);
    return {0, 0};
  }

  Coordinate smarterNextCellTosolve() const {
    size_t minCard = 9;
    Coordinate result{0, 0};
    for (size_t x = 0; x < 9; x++)
      for (size_t y = 0; y < 9; y++) {
        if (!solvedCell(x, y) && board[x][y].size() < minCard) {
          minCard = board[x][y].size();
          result = {x, y};
        }
      }
    return result;
  }

  int possibleValueForCell(const Coordinate& c) const {
    return *(board[c.first][c.second].begin());
  }

  void removeValueForCell(const Coordinate& c, int v) {
    board[c.first][c.second].erase(v);
  }

  void assignValueForCell(const Coordinate& c, int v) {
    board[c.first][c.second] = {v};
  }

private:
  /**
   * Reduces the cell (i,j) using the row property.
   */
  bool reduceRow(size_t i, size_t j) {
    auto oldSize = board[i][j].size();
    for (size_t x = 0; x < 9; x++) {
      if (x != j && solvedCell(i, x)) {
        // fmt::print_colored(fmt::BLUE, "using {} {} to reduce {} {}\n", i, x,
        // i,j);
        board[i][j].erase(valueCell(i, x));
      }
    }
    auto newSize = board[i][j].size();
    if (newSize < oldSize) {
      // fmt::print_colored(fmt::RED, "Effective row reduction of ({} {})\n", i,
      //                    j);
      return true;
    }
    return false;
  }

  /**
   * Reduces the cell (i,j) using the column property.
   */
  bool reduceCol(size_t i, size_t j) {
    // fmt::print_colored(fmt::RED, "Reducing cell ({},{})\n", i, j);
    auto oldSize = board[i][j].size();
    for (size_t row = 0; row < 9; row++) {
      if (row != i && solvedCell(row, j)) {
        board[i][j].erase(valueCell(row, j));
        // fmt::print("I will use {} {}\n", row, j);
      }
    }
    auto newSize = board[i][j].size();
    if (newSize < oldSize) {
      // fmt::print_colored(fmt::RED, "Effective col reduction of ({} {})\n", i,
      //                    j);
      return true;
    }
    return false;
  }

  bool reduceBox(size_t i, size_t j) {
    auto oldSize = board[i][j].size();

    size_t boxStartRow = i - (i % 3);
    size_t boxStartCol = j - (j % 3);

    for (size_t row = 0; row < 3; row++)
      for (size_t col = 0; col < 3; col++) {
        size_t x = row + boxStartRow;
        size_t y = col + boxStartCol;
        if (!(x == i && y == j) && solvedCell(x, y)) {
          board[i][j].erase(valueCell(x, y));
        }
      }
    auto newSize = board[i][j].size();
    if (newSize < oldSize) {
      // fmt::print_colored(fmt::RED, "Effective box reduction of ({} {})\n", i,
      //                    j);
      return true;
    }
    return false;
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

        if (cell.size() == 0) {
          fmt::print_colored(fmt::RED, "{{}}");
        } else if (cell.size() == 1) {
          auto elem = *(cell.cbegin());
          fmt::print_colored(fmt::GREEN, "{}", elem);
        } else {
          fmt::print("{{");
          for (auto i : cell) fmt::print("{},", i);
          fmt::print("}}");
        }
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
  s.reduce();
  st.reductions++;

  if (s.isFailed()) {
    st.failures++;
    return {s, false};
  } else if (s.isSolved()) {
    st.solutions++;
    return {s, true};
  } else {

    st.decisions++;
    Sudoku copy(s);
    // Make a decision: find the next cell to be solved and assign one of its
    // possible values to it.
    // Coordinate next = copy.nextCellTosolve();
    Coordinate next = copy.smarterNextCellTosolve();
    int val = copy.possibleValueForCell(next);
    copy.assignValueForCell(next, val);
    pair<Sudoku, bool> result = solveOne(copy, st);

    if (result.second)
      return result;
    else {
      s.removeValueForCell(next, val);
      return solveOne(s, st);
    }
  }
}

vector<Sudoku> solveAll(Sudoku& s, Statistics& st) {
  s.reduce();
  st.reductions++;

  if (s.isFailed()) {
    st.failures++;
    return {};
  }
  if (s.isSolved()) {
    st.solutions++;
    return {s};
  }

  st.decisions++;
  Sudoku copy(s);
  // Make a decision: find the next cell to be solved and assign one of its
  // possible values to it.
  // Coordinate next = copy.nextCellTosolve();
  Coordinate next = copy.smarterNextCellTosolve();
  int val = copy.possibleValueForCell(next);
  copy.assignValueForCell(next, val);
  vector<Sudoku> result = solveAll(copy, st);

  s.removeValueForCell(next, val);
  vector<Sudoku> result2 = solveAll(s, st);

  // Aggregate all the solutions
  result.insert(result.end(), result2.begin(), result2.end());
  return result;
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

void solveAll(const Sudoku& s) {
  Statistics st;
  Sudoku root(s);
  vector<Sudoku> sols = solveAll(root, st);
  for (const auto& s : sols) s.print();
  st.print();
}

int main(void) {
  fmt::print("Sudoku solver\n");
  Sudoku a({{0, 0, 3, 0, 2, 0, 6, 0, 0},
            {9, 0, 0, 3, 0, 5, 0, 0, 1},
            {0, 0, 1, 8, 0, 6, 4, 0, 0},
            {0, 0, 8, 1, 0, 2, 9, 0, 0},
            {7, 0, 0, 0, 0, 0, 0, 0, 8},
            {0, 0, 6, 7, 0, 8, 2, 0, 0},
            {0, 0, 2, 6, 0, 9, 5, 0, 0},
            {8, 0, 0, 2, 0, 3, 0, 0, 9},
            {0, 0, 5, 0, 1, 0, 3, 0, 0}});

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
  });
  // solve(a);
  // solve(b);
  // solve(c);
  // solveAll(c);
  solveAll(e);
  // solve(d);
  return 0;
}