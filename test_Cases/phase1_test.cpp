#include <gtest/gtest.h>
#include "../game_logic/GameEngine.h"  // adjust include path
#include <algorithm>

// Helper: check if two Move vectors are equal (order doesn't matter)
bool MovesEqual(std::vector<Move> a, std::vector<Move> b) {
    if (a.size() != b.size()) return false;
    std::sort(a.begin(), a.end(), [](const Move& m1, const Move& m2) {
        return (m1.row < m2.row) || (m1.row == m2.row && m1.col < m2.col);
    });
    std::sort(b.begin(), b.end(), [](const Move& m1, const Move& m2) {
        return (m1.row < m2.row) || (m1.row == m2.row && m1.col < m2.col);
    });
    return a == b;
}

// ---------- Tests ----------

TEST(GetFlipsTest, IllegalOnOccupiedCell) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // (3,3) already Black
    auto flips = GetFlips(board, Move(3,3), CellState::White);
    EXPECT_TRUE(flips.empty());
}

TEST(GetFlipsTest, LegalMoveInitialPosition) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // White move at (2,3) should flip (3,3)
    auto flips = GetFlips(board, Move(2,3), CellState::White);
    std::vector<Move> expected = { Move(3,3) };
    EXPECT_TRUE(MovesEqual(flips, expected));
}

TEST(GetFlipsTest, IllegalIsolatedMove) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // White move far away at (0,0) should flip nothing
    auto flips = GetFlips(board, Move(0,0), CellState::White);
    EXPECT_TRUE(flips.empty());
}

TEST(GetFlipsTest, HorizontalFlip) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // Create a row: B W W _ (row 3, cols 2..5)
    board[3][2] = CellState::Black;
    board[3][3] = CellState::White;
    board[3][4] = CellState::White;
    board[3][5] = CellState::Empty;

    // Black plays at (3,5) → should flip (3,3), (3,4)
    auto flips = GetFlips(board, Move(3,5), CellState::Black);
    std::vector<Move> expected = { Move(3,3), Move(3,4) };
    EXPECT_TRUE(MovesEqual(flips, expected));
}

TEST(GetFlipsTest, VerticalFlip) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // Create a column: B W W _ (col 3, rows 2..5)
    board[2][3] = CellState::Black;
    board[3][3] = CellState::White;
    board[4][3] = CellState::White;
    board[5][3] = CellState::Empty;

    // Black plays at (5,3) → should flip (3,3), (4,3)
    auto flips = GetFlips(board, Move(5,3), CellState::Black);
    std::vector<Move> expected = { Move(3,3), Move(4,3) };
    EXPECT_TRUE(MovesEqual(flips, expected));
}

TEST(GetFlipsTest, DiagonalFlip) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // Diagonal: B W W _ from (2,2) to (5,5)
    board[2][2] = CellState::Black;
    board[3][3] = CellState::White;
    board[4][4] = CellState::White;
    board[5][5] = CellState::Empty;

    // Black plays at (5,5) → should flip (3,3), (4,4)
    auto flips = GetFlips(board, Move(5,5), CellState::Black);
    std::vector<Move> expected = { Move(3,3), Move(4,4) };
    EXPECT_TRUE(MovesEqual(flips, expected));
}

TEST(GetFlipsTest, NoFlipIfNotBounded) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // Row: W W B _ (row 3, cols 2..5)
    board[3][2] = CellState::White;
    board[3][3] = CellState::White;
    board[3][4] = CellState::Black;
    board[3][5] = CellState::Empty;

    // White plays at (3,5) → should flip nothing (no bounding white disc)
    auto flips = GetFlips(board, Move(3,5), CellState::White);
    EXPECT_TRUE(flips.empty());
}

TEST(GetFlipsTest, CornerFlip) {
    CellState board[BOARD_SIZE][BOARD_SIZE];
    ResetBoard(board);

    // Setup diagonal into corner
    board[6][6] = CellState::Black;
    board[5][5] = CellState::White;
    board[4][4] = CellState::White;
    board[7][7] = CellState::Empty;

    // Black plays (7,7) → should flip (5,5), (4,4)
    auto flips = GetFlips(board, Move(7,7), CellState::Black);
    std::vector<Move> expected = { Move(5,5), Move(4,4) };
    EXPECT_TRUE(MovesEqual(flips, expected));
}
