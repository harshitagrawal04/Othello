#include <iostream>
#include <vector>

using namespace std;

// declaring the states in which each cell can be
enum class CellState {
    Empty,
    Black,
    White
};

// helper function to display the value of cell
char cellToChar(CellState cell) {
    if (cell == CellState::Empty) return '.';
    if (cell == CellState::Black) return 'B';
    return 'W';
}

// setting all the cells to empty
// void resetBoard(cellState[][])


struct Move {
    int row;
    int col;
};



int main() {
    CellState board[8][8];

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = CellState::Empty;
        }
    }


    board[3][3] = CellState::Black;
    board[4][4] = CellState::Black;
    board[3][4] = CellState::White;
    board[4][3] = CellState::White;


    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            cout << cellToChar(board[row][col]) << " ";
        }
        cout << endl;
    }

    vector<Move> moves;

    moves.push_back({0,0});
    moves.push_back({0,1});
    moves.push_back({1,0});

    for (int i = 0; i < moves.size(); i++) {
        cout << moves[i].row << " " << moves[i].col << endl;
    }




    return 0;
}