#include <iostream>
#include "game_logic/game_logic.h"
using namespace std;

void displayVector(const vector<Move> &move, const int size) {
    for (int i = 0; i < size; i++) {
        cout << move[i].row << " " << move[i].col << " " << endl;
    }
}

int main() {
    CellState board[BOARD_SIZE][BOARD_SIZE]; // Initialize the board
    ResetBoard(board); // set the board

    auto current_player = CellState::Black; // Set the current player to black

    vector<History> history; // setting up the history

    while (GetLegalMoves(board, CellState::Black).empty() == false ||
           GetLegalMoves(board, CellState::White).empty() == false) {

        DisplayBoard(board);

        // get all the legal move
        map<Move, vector<Move> > legal_moves = GetLegalMoves(board, current_player);

        // if there are no legal moves for the current player change the player
        if (legal_moves.empty()) {
            cout << "No legal moves for this turn" << endl;
            current_player = GetOpponent(current_player);
            continue;
        }

        // print the legal moves
        cout << "all possible legal moves" << endl;
        DisplayMoves(legal_moves);

        Move move{};

        // asker user for the move they want to make
        cout << "Player "
                     << (current_player == CellState::Black ? "Black" : "White")
                     << " enter your move (row col): ";
        cin >> move.row >> move.col;

        // checking if the entered move is legal or not

        // if it is legal then make the move and change the current_player
        if (auto legal = legal_moves.find(move); legal != legal_moves.end()) {
            vector<Move> flips = GetFlipsMap(legal_moves, move);
            MakeMove(board, current_player, move, flips);
            current_player = GetOpponent(current_player);
            history.push_back({current_player, move, flips});
        }
        // else print error message
        else {
            cout << "not valid move" << endl;
        }
    }

    // displaying the history
    for (auto& history1: history) {
        DisplayHistory(history1);
    }

    // counting the number of disk each player have and print the winner
    if (auto [black, white] = CountDisk(board); black >= white) cout << "Black wins" << endl;
    else if (white > black) cout << "White wins" << endl;
    else cout << "Draw" << endl;

    return 0;
}


// datatype to store the history of the moves
// undo function to undo the moves
// function which update the history after the undo
// stopper function which will stop undo and history going beyond initial stage