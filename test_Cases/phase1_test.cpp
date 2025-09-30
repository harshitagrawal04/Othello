#include <gtest/gtest.h>
#include "GameEngine.h"



// Test suite for GameEngine public functions
TEST(GameEnginePublicTest, CountDiskInitialBoard) {
    GameEngine game;

    // On a fresh board, there should be 2 black and 2 white disks
    auto [black, white] = game.CountDisk();

    EXPECT_EQ(black, 2);
    EXPECT_EQ(white, 2);
}
