#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "board.h"
#include <iostream>
#include <memory>

// Use a TEST_CASE with sections for related tests.
TEST_CASE("Board::move", "[move]") {
    // Setup a standard board for each section.
    auto board = StandardBoard();

    SECTION("Pawn Double Step is a legal move") {
        REQUIRE(board->move(Square::E2, Square::E4));
        REQUIRE(board->getSideToMove() == Color::BLACK);
        REQUIRE(board->getWhitePawns() == 0x000000001000EF00ULL);
        // The en passant square should be set to the square behind the pawn's destination.
        REQUIRE(board->getEnPassent() == static_cast<uint64_t>(Square::E3));
    }

    SECTION("Illegal pawn move is not allowed") {
        REQUIRE_FALSE(board->move(Square::E2, Square::E5));
        REQUIRE(board->getSideToMove() == Color::WHITE);
    }

    SECTION("Knight move is a legal move") {
        REQUIRE(board->move(Square::B1, Square::C3));
        REQUIRE(board->getSideToMove() == Color::BLACK);
        REQUIRE(board->getWhiteKnights() == (static_cast<uint64_t>(Square::C3) | static_cast<uint64_t>(Square::G1)));
    }
    
    SECTION("Move by the wrong side is not allowed") {
        REQUIRE_FALSE(board->move(Square::E7, Square::E5));
        REQUIRE(board->getSideToMove() == Color::WHITE);
    }

    SECTION("Pawn capture is a legal move") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhitePawns(static_cast<uint64_t>(Square::E4))
            .setBlackPawns(static_cast<uint64_t>(Square::D5))
            .Build();
        
        REQUIRE(customBoard->move(Square::E4, Square::D5));
        REQUIRE(customBoard->getSideToMove() == Color::BLACK);
        REQUIRE(customBoard->getWhitePawns() == static_cast<uint64_t>(Square::D5));
        REQUIRE(customBoard->getBlackPawns() == 0ULL);
    }

    SECTION("En passant capture is a legal move") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setWhitePawns(static_cast<uint64_t>(Square::E4))
            .setBlackPawns(static_cast<uint64_t>(Square::D4))
            .setEnPassent(static_cast<uint64_t>(Square::E3))
            .Build();
            
        REQUIRE(customBoard->move(Square::D4, Square::E3));
        REQUIRE(customBoard->getSideToMove() == Color::WHITE);
        REQUIRE(customBoard->getBlackPawns() == static_cast<uint64_t>(Square::E3));
        REQUIRE(customBoard->getWhitePawns() == 0ULL);
    }

    SECTION("White kingside castling is a legal move") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::H1))
            .setWhiteCastleKingside(true)
            .Build();

        REQUIRE(customBoard->move(Square::E1, Square::G1));
        REQUIRE(customBoard->getWhiteKing() == static_cast<uint64_t>(Square::G1));
        REQUIRE(customBoard->getWhiteRooks() == static_cast<uint64_t>(Square::F1));
    }

    SECTION("Castling is not allowed if a square in the path is attacked") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::H1))
            .setBlackRooks(static_cast<uint64_t>(Square::F2))
            .setWhiteCastleKingside(true)
            .Build();

        REQUIRE_FALSE(customBoard->move(Square::E1, Square::G1));
    }

    SECTION("Illegal move that leaves the king in check is not allowed") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhitePawns(static_cast<uint64_t>(Square::E2))
            .setBlackRooks(static_cast<uint64_t>(Square::H1))
            .Build();
        
        REQUIRE_FALSE(customBoard->move(Square::E2, Square::E4));
    }

    SECTION("Sliding piece cannot jump over another piece") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::A1))
            .setWhiteKnights(static_cast<uint64_t>(Square::A2))
            .Build();
        
        REQUIRE_FALSE(customBoard->move(Square::A1, Square::A3));
    }
}