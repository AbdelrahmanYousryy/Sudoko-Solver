/*****************************************************************************/
// Title: Sudoko Solver
// Author: Abdelrahman Yousry
// Date: 20/9/2024
// Version: V1
/****************************** Include Files*
 * **********************************/
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

/* Class Board that Contains all the values of the board and helper functions of
 * the solver*/

class board {
public:
  // Iniatializing the board with zeros(empty)
  board() : board_values(initalize_board_values()) {}
  board(vector<vector<int>> board_values) {
    this->board_values = board_values;
    check_given_values();
  };
  // read the values from the user
  void get_board() {
    cout << "       Please Start To fill the board" << endl;
    print_board();
    for (int row_ind = 0; row_ind < 9; row_ind++) {
      for (int col_ind = 0; col_ind < 9; col_ind++) {
        while (1) {
          cout << " Enter the value for row " << row_ind + 1 << " and column "
               << col_ind + 1 << " : ";
          cout << " Enter 0 for no filling" << endl;

          // fill the board with the given values
          cin >> board_values[row_ind][col_ind];

          // check if the value is valid
          if (board_values[row_ind][col_ind] > 9 ||
              board_values[row_ind][col_ind] < 0) {
            cout << "Invalid Input" << endl;
            continue;
          } else
            break;
        }
        print_board();
      }
    }

    // Mark the Given values in the board
    check_given_values();
  }
  // print values of the board
  void print_board() {

    // Clear The Console
    int result = system("clear");
    if (result != 0) {
      std::cerr << "Error clearing the screen!"
                << std::endl; // Indicate an error occurre
    }

    int row_double = 4;
    cout << " =========================================" << endl;
    for (int i = 0; i < 9; i++) {

      cout << " || ";
      int col_double = 4;
      for (int x = 0; x < 9; x++) {
        if (board_values[i][x] != 0)
          cout << board_values[i][x];
        else
          cout << " ";
        if (col_double % 3 == 0)
          cout << " || ";
        else
          cout << " | ";
        col_double++;
      }
      cout << endl;
      if (row_double % 3 == 0)
        cout << " =========================================" << endl;
      else
        cout << " -----------------------------------------" << endl;
      row_double++;
    }
  }
  // Solve Using Backtracking Technique
  void solve_board_backtracking() {
    // Store which solution is taken in every route to change them when
    // backtracking
    vector<vector<int>> solutions_taken(9, vector<int>(9, 0));
    while (1) {
      print_board();
      static int row_ind = 1;
      static int col_ind = 1;

      cout << "Working with element " << row_ind << " " << col_ind << endl;
      // sleep for 200ms so the user can see the board
      //std::this_thread::sleep_for(std::chrono::milliseconds(200));

      // Check if the current element is already filled
      if (board_values[row_ind - 1][col_ind - 1] != 0) {
        col_ind++;
        if (col_ind == 10) {
          row_ind++;
          col_ind = 1;
        }
        // Check if the board is solved
        if (row_ind == 10) {
          cout << "Solved!" << endl;
          break;
        }
        continue;
      }

      // Get all the possible solutions for current element
      vector<int> possible_sol = check_possible_nums(row_ind, col_ind);
      // store the used solution in this step
      int solution_index = solutions_taken[row_ind - 1][col_ind - 1];

      // check if there is no solution for the current element
      if (possible_sol.empty() || solution_index >= possible_sol.size()) {
        // no possible solutions return to the previous decision node
        solutions_taken[row_ind - 1][col_ind - 1] = 0;
        board_values[row_ind - 1][col_ind - 1] = 0;
        cout << "No Solutions for row " << row_ind << " and column " << col_ind
             << endl;
        cout << "Trying diffrent route " << endl;
        cout << "Backtracking to the latest solved element" << endl;
        // returning to the previous decision node
        while (1) {
          col_ind--;
          if (col_ind == 0) {
            col_ind = 9;
            row_ind--;
          }
          if (given_values[row_ind - 1][col_ind - 1] == false) {
            cout << " Backtracking to the latest solved element" << endl;
            cout << "BackTracked to " << row_ind << " " << col_ind << endl;
            // sleeps for 200ms so the user can see the board
            //std::this_thread::sleep_for(std::chrono::milliseconds(300));
            // fill the unsolved element with 0 to resolve it again
            board_values[row_ind - 1][col_ind - 1] = 0;
            break;
          }
        }
        continue;

      } else {
        // if there is a solution for the current element
        // fill the element with the selected index
        board_values[row_ind - 1][col_ind - 1] = possible_sol[solution_index];
        // store the selected index and change it for the next use
        solutions_taken[row_ind - 1][col_ind - 1] += 1;
        print_board();
      }
      print_board();

      // check corner cases
      if (row_ind == 9 && col_ind == 9) {
        cout << "Solved" << endl;
        break;
      }
      // go to the next element
      col_ind++;
      if (col_ind == 10) {
        row_ind++;
        col_ind = 1;
        continue;
      }
    }
  }
  /********************Private and Helper
   * Functions********************************/
private:
  vector<vector<int>> board_values;
  vector<vector<bool>> given_values;
  // initializing the board with zeros
  vector<vector<int>> initalize_board_values() {
    vector<vector<int>> board_values(9, vector<int>(9, 0));
    return board_values;
  }
  // flatten the box to use the same function that columns and rows use
  vector<int> flatten(const vector<vector<int>> &orig) {
    vector<int> ret;
    for (const auto &v : orig)
      ret.insert(ret.end(), v.begin(), v.end());
    return ret;
  }

  // extract block from the board
  vector<vector<int>> extract_block(int block_num) {
    vector<vector<int>> block;
    int block_start_row = (block_num - 1) / 3 * 3;
    int block_start_col = (block_num - 1) % 3 * 3;
    for (int i = 0; i < 3; i++) {
      vector<int> row;
      for (int j = 0; j < 3; j++) {
        row.push_back(board_values[block_start_row + i][block_start_col + j]);
      }
      block.push_back(row);
    }
    return block;
  }
  // etract Column from the board
  vector<int> get_col(int col_num) {
    vector<int> col;
    for (int i = 0; i < 9; i++)
      col.push_back(board_values[i][col_num - 1]);
    return col;
  }
  int select_block(int row_num, int col_num) {
    int block_num = (((row_num - 1) / 3) * 3) + 1 + ((col_num - 1) / 3);
    return block_num;
  }

  // check possible solutions in the vector
  vector<int> check_available_vector(vector<int> nums) {
    vector<int> init_values(9, 1);
    for (int i = 0; i < nums.size(); i++) {
      init_values[nums[i] - 1] = 0;
    }
    return init_values;
  }

  // check possible solutions for the element
  vector<int> check_possible_nums(int row_num, int col_num) {
    vector<int> possible_nums;
    // first check the row
    vector<int> possible_nums_row = check_available_row(row_num);
    // then check column
    vector<int> possible_nums_col = check_available_col(col_num);

    // then check the  block
    vector<int> possible_nums_block = check_available_block(row_num, col_num);

    // then check the possible numbers
    for (int i = 0; i < 9; i++) {
      if (possible_nums_row[i] == 1 && possible_nums_col[i] == 1 &&
          possible_nums_block[i] == 1) {
        possible_nums.push_back(i + 1);
      }
    }
    return possible_nums;
  }

  vector<int> check_available_row(int row_num) {
    return check_available_vector(board_values[row_num - 1]);
  };
  vector<int> check_available_col(int col_num) {
    return check_available_vector(get_col(col_num));
  }
  vector<int> check_available_block(int row, int col) {
    int block_num = select_block(row, col);
    vector<vector<int>> block = extract_block(block_num);
    vector<int> block_flattened = flatten(block);
    vector<int> possible_nums_block = check_available_vector(block_flattened);
    return possible_nums_block;
  }
  void check_given_values() {
    given_values = vector<vector<bool>>(9, vector<bool>(9, false));
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (board_values[i][j] != 0) {
          given_values[i][j] = true;
        }
      }
    }
  }
};
int main() {
  vector<vector<int>> test_board = {
      {0, 0, 0, 6, 0, 8, 9, 1, 0}, {0, 9, 0, 4, 3, 2, 6, 8, 7},
      {0, 6, 3, 9, 0, 0, 2, 0, 4}, {9, 0, 0, 3, 0, 4, 0, 0, 2},
      {3, 1, 0, 0, 0, 0, 0, 7, 9}, {0, 7, 0, 0, 0, 9, 5, 0, 0},
      {0, 0, 1, 0, 9, 6, 3, 0, 0}, {0, 0, 0, 0, 0, 1, 0, 2, 0},
      {0, 0, 6, 7, 4, 0, 8, 0, 1}};

  board new_board;
  new_board.get_board();
  new_board.print_board();
  new_board.solve_board_backtracking();
}