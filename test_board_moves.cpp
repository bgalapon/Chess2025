#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "board.h"
#include <iostream>
#include <memory>

TEST_CASE("Pawn Moves", "[move][pawn]") {
    BoardBuilder builder(Square::E8, Square::E1, Color::WHITE);
    auto board = builder.setWhitePawns(static_cast<uint64_t>(Square::D2)).Build();
    REQUIRE(board->move(Square::D2, Square::D3) == true);
    REQUIRE(board->move(Square::D3, Square::D4) == false);
}

TEST_CASE("Invalid Moves", "[move][invalid]") {
    BoardBuilder builder(Square::E8, Square::E1, Color::WHITE);
    auto board = builder.setWhitePawns(static_cast<uint64_t>(Square::A2)).Build();
    REQUIRE(board->move(Square::A3, Square::A4) == false);
    REQUIRE(board->move(Square::H4, Square::A2) == false);
}

TEST_CASE("King in Check", "[move][check]") {
    BoardBuilder builder(Square::E8, Square::E1, Color::WHITE);
    auto board = builder.setBlackRooks(static_cast<uint64_t>(Square::E5)).Build();
    REQUIRE(board->move(Square::E1, Square::D1) == false);
    
    BoardBuilder builder2(Square::E8, Square::E1, Color::WHITE);
    auto board2 = builder2.setBlackRooks(static_cast<uint64_t>(Square::H5)).setWhitePawns(static_cast<uint64_t>(Square::F2)).Build();
    REQUIRE(board2->move(Square::F2, Square::F3) == true);
}

TEST_CASE("Castling", "[move][castling]") {
    BoardBuilder builder(Square::E8, Square::E1, Color::WHITE);
    auto board = builder.setWhiteRooks(static_cast<uint64_t>(Square::A1) | static_cast<uint64_t>(Square::H1))
                       .setWhiteCastleKingside(true)
                       .Build();
    REQUIRE(board->move(Square::E1, Square::G1) == true);
    
    BoardBuilder builder2(Square::E8, Square::E1, Color::WHITE);
    auto board2 = builder2.setWhiteRooks(static_cast<uint64_t>(Square::A1) | static_cast<uint64_t>(Square::H1))
                        .setWhiteKnights(static_cast<uint64_t>(Square::F1))
                        .setWhiteCastleKingside(true)
                        .Build();
    REQUIRE(board2->move(Square::E1, Square::G1) == false);
}
