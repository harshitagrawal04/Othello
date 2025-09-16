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

        char row; int col;

        // asker user for the move they want to make
        cout << "Player "
                     << (current_player == CellState::Black ? "Black" : "White")
                     << " enter your move (row col) or u for undo: ";
        cin >> row;

        if (row == 'u' || row == 'U') {
            if (!history.empty()) {
                UndoMove(board, history);
                current_player = GetOpponent(current_player);
            }
            else {
                cout << "No moves to undo!" << endl;
            }
            continue;
        }

        cin >> col;

        Move move = {CharToInt(row), col};

        // checking if the entered move is legal or not if yes then make the move
        if (auto legal = legal_moves.find(move); legal != legal_moves.end()) {
            vector<Move> flips = GetFlipsMap(legal_moves, move);
            MakeMove(board, current_player, move, flips);
            history.push_back({current_player, move, flips});
            current_player = GetOpponent(current_player);
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


