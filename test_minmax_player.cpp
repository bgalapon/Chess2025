#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "player.h"
#include <iostream>
#include <memory>

TEST_CASE("MinMaxPlayer::makeMove", "[makeMove]") {
    SECTION("MinMax player should take free queen") {
        auto board = BoardBuilder(
            "....k..."
            "........"
            "........"
            "........"
            "........"
            ".R......"
            "........"
            ".q....K.", Color::WHITE).Build();

        MinMaxPlayer player(4);
        REQUIRE(player.makeMove(*board));
        // std::cout << board->toString() << std::endl;
        REQUIRE(board->getNumBlackQueens() == 0);
    }

    SECTION("MinMax player should take free pawns") {
        auto board = BoardBuilder(
            "rnbqkbnr"
            "pppp.ppp"
            "........"
            "....p..."
            "...P...."
            "........"
            "PPP.PPPP"
            "RNBQKBNR", Color::WHITE).Build();

        MinMaxPlayer player(5);
        REQUIRE(player.makeMove(*board));
        std::cout << board->toString() << std::endl;
        REQUIRE(board->getNumBlackPawns() == 7);
    }
}