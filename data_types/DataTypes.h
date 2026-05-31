#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <functional> // For std::hash


constexpr int BOARD_SIZE = 8;

enum class CellState {
    Empty,
    Black,
    White
};


struct Move {
    int row;
    int col;

    // For std::map (ordered)
    bool operator<(const Move& other) const {
        if (row == other.row) {
            return col < other.col;
        }
        return row < other.row;
    }

    // For unordered_map/unordered_set (equality)
    bool operator==(const Move& other) const {
        return row == other.row && col == other.col;
    }
};

// Custom hash function for Move
namespace std {
    template <>
    struct hash<Move> {
        size_t operator()(const Move& m) const noexcept {
            // Combine row and col into one hash value
            return (m.row << 3) ^ m.col;
        }
    };
}

struct History {
    CellState player;
    Move move;
    std::vector<Move> flipped;
    CellState board[BOARD_SIZE][BOARD_SIZE];
};

enum class GameState {
    MainMenu,  // Home screen: select 1-player or 2-player
    InGame,    // Currently playing Othello on the board
    GameOver   // Displaying final scores
};


#endif //HISTORY_H
