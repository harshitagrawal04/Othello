#ifndef MOVE_H
#define MOVE_H

#include <functional>  // for std::hash

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
            return std::hash<int>()(m.row) ^ (std::hash<int>()(m.col) << 1);
        }
    };
}

#endif //MOVE_H
