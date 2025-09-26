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

    SECTION("MinMax player(4) should take free knight") {
        auto board = BoardBuilder(
            "rnbqk.nr"
            "p.pp.ppp"
            ".p..p..."
            "...N...."
            ".b..P..."
            ".....N.."
            "PPPP.PPP"
            "R.BQKB.R", Color::BLACK).Build();

        MinMaxPlayer player(4);
        REQUIRE(player.makeMove(*board));
        std::cout << board->toString() << std::endl;
        REQUIRE(board->getNumWhiteKnights() == 1);
    }

    SECTION("MinMax player should not hang knight") {
        auto board = BoardBuilder(
            "rnbqk.nr"
            "p.pp.ppp"
            ".p..p..."
            "........"
            ".b..P..."
            "..N..N.."
            "PPPP.PPP"
            "R.BQKB.R", Color::WHITE).Build();

        MinMaxPlayer player(5);
        REQUIRE(player.makeMove(*board));
        std::cout << board->toString() << std::endl;
        REQUIRE(((board->getWhiteKnights() & static_cast<uint64_t>(Square::D5)) == 0));
    }
}