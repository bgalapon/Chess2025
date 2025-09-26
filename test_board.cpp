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
        REQUIRE(board->makeMove({Square::E2, Square::E4}));
        REQUIRE(board->getSideToMove() == Color::BLACK);
        REQUIRE(board->getWhitePawns() == 0x000000001000EF00ULL);
        // The en passant square should be set to the square behind the pawn's destination.
        REQUIRE(board->getEnPassent() == static_cast<uint64_t>(Square::E3));
    }

    SECTION("Illegal pawn move is not allowed") {
        REQUIRE_FALSE(board->makeMove({Square::E2, Square::E5}));
        REQUIRE(board->getSideToMove() == Color::WHITE);
    }

    SECTION("Knight move is a legal move") {
        REQUIRE(board->makeMove({Square::B1, Square::C3}));
        REQUIRE(board->getSideToMove() == Color::BLACK);
        REQUIRE(board->getWhiteKnights() == (static_cast<uint64_t>(Square::C3) | static_cast<uint64_t>(Square::G1)));
    }
    
    SECTION("Move by the wrong side is not allowed") {
        REQUIRE_FALSE(board->makeMove({Square::E7, Square::E5}));
        REQUIRE(board->getSideToMove() == Color::WHITE);
    }

    SECTION("Pawn capture is a legal move") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhitePawns(static_cast<uint64_t>(Square::E4))
            .setBlackPawns(static_cast<uint64_t>(Square::D5))
            .Build();
        
        REQUIRE(customBoard->makeMove({Square::E4, Square::D5}));
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
            
        REQUIRE(customBoard->makeMove({Square::D4, Square::E3}));
        REQUIRE(customBoard->getSideToMove() == Color::WHITE);
        REQUIRE(customBoard->getBlackPawns() == static_cast<uint64_t>(Square::E3));
        REQUIRE(customBoard->getWhitePawns() == 0ULL);
    }

    SECTION("White castling kingside is a legal move") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::H1))
            .setWhiteCastleKingside(true)
            .Build();

        REQUIRE(customBoard->makeMove({Square::E1, Square::G1}));
        REQUIRE(customBoard->getWhiteKing() == static_cast<uint64_t>(Square::G1));
        REQUIRE(customBoard->getWhiteRooks() == static_cast<uint64_t>(Square::F1));
    }

    SECTION("White castling kingside is not allowed if a square in the path is attacked") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::H1))
            .setBlackRooks(static_cast<uint64_t>(Square::F3))
            .setWhiteCastleKingside(true)
            .Build();

        REQUIRE_FALSE(customBoard->makeMove({Square::E1, Square::G1}));
    }

    SECTION("White castling kingside is not allowed if king is in check") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::H1))
            .setBlackRooks(static_cast<uint64_t>(Square::E3))
            .setWhiteCastleKingside(true)
            .Build();

        REQUIRE_FALSE(customBoard->makeMove({Square::E1, Square::G1}));
    }

    SECTION("White castling kingside is not allowed if king goes into check") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::H1))
            .setBlackRooks(static_cast<uint64_t>(Square::G3))
            .setWhiteCastleKingside(true)
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E1, Square::G1}));
    }

    SECTION("White castling queenside is allowed") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::A1))
            .setWhiteCastleQueenside(true)
            .Build();
    
        REQUIRE(customBoard->makeMove({Square::E1, Square::C1}));
    }

    SECTION("White castling queenside is not allowed if king goes into check") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::A1))
            .setBlackRooks(static_cast<uint64_t>(Square::C3))
            .setWhiteCastleQueenside(true)
            .Build();
    
        REQUIRE_FALSE(customBoard->makeMove({Square::E1, Square::C1}));
    }

    SECTION("White castling queenside is not allowed if king goes through check with rook") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::A1))
            .setBlackRooks(static_cast<uint64_t>(Square::D3))
            .setWhiteCastleQueenside(true)
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E1, Square::C1}));
    }

    SECTION("White castling queenside is not allowed if king goes through check with pawn") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::A1))
            .setBlackPawns(static_cast<uint64_t>(Square::E2))
            .setWhiteCastleQueenside(true)
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E1, Square::C1}));
    }

    SECTION("White castling queenside is allowed if no squares in path are attacked") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::A1))
            .setWhitePawns(static_cast<uint64_t>(Square::D2 | Square::E2))
            .setBlackRooks(static_cast<uint64_t>(Square::D3))
            .setWhiteCastleQueenside(true)
            .Build();

        REQUIRE(customBoard->makeMove({Square::E1, Square::C1}));
    }

    SECTION("White castling queenside is not allowed if no rook exists on A1") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::H1))
            .setWhitePawns(static_cast<uint64_t>(Square::D2 | Square::E2))
            .setBlackRooks(static_cast<uint64_t>(Square::D3))
            .setWhiteCastleQueenside(true)
            .Build();

        REQUIRE_FALSE(customBoard->makeMove({Square::E1, Square::C1}));
    }

    SECTION("Illegal move that leaves the king in check is not allowed") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhitePawns(static_cast<uint64_t>(Square::E2))
            .setBlackRooks(static_cast<uint64_t>(Square::H1))
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E2, Square::E4}));
    }

    SECTION("Sliding piece cannot jump over another piece") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::WHITE)
            .setWhiteRooks(static_cast<uint64_t>(Square::A1))
            .setWhiteKnights(static_cast<uint64_t>(Square::A2))
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::A1, Square::A3}));
    }

    SECTION("Black castling kingside is a legal move") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::H8))
            .setBlackCastleKingside(true)
            .Build();

        REQUIRE(customBoard->makeMove({Square::E8, Square::G8}));
        REQUIRE(customBoard->getBlackKing() == static_cast<uint64_t>(Square::G8));
        REQUIRE(customBoard->getBlackRooks() == static_cast<uint64_t>(Square::F8));
    }

    SECTION("Black castling kingside is not allowed if a square in the path is attacked") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::H8))
            .setWhiteRooks(static_cast<uint64_t>(Square::F6))
            .setBlackCastleKingside(true)
            .Build();

        REQUIRE_FALSE(customBoard->makeMove({Square::E8, Square::G8}));
    }

    SECTION("Black castling kingside is not allowed if king is in check") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::H8))
            .setWhiteRooks(static_cast<uint64_t>(Square::E6))
            .setBlackCastleKingside(true)
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E8, Square::G8}));
    }

    SECTION("Black castling kingside is not allowed if king goes into check") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::H8))
            .setWhiteRooks(static_cast<uint64_t>(Square::G6))
            .setBlackCastleKingside(true)
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E8, Square::G8}));
    }

    SECTION("Black castling queenside is allowed") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::A8))
            .setBlackCastleQueenside(true)
            .Build();
    
        REQUIRE(customBoard->makeMove({Square::E8, Square::C8}));
    }

    SECTION("Black castling queenside is not allowed if king goes into check") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::A8))
            .setWhiteRooks(static_cast<uint64_t>(Square::C6))
            .setBlackCastleQueenside(true)
            .Build();

        REQUIRE_FALSE(customBoard->makeMove({Square::E8, Square::C8}));
    }

    SECTION("Black castling queenside is not allowed if king goes through check with rook") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::A8))
            .setWhiteRooks(static_cast<uint64_t>(Square::D6))
            .setBlackCastleQueenside(true)
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E8, Square::C8}));
    }

    SECTION("Black castling queenside is not allowed if king goes through check with pawn") {
        auto customBoard = BoardBuilder(Square::E8, Square::E1, Color::BLACK)
            .setBlackRooks(static_cast<uint64_t>(Square::A8))
            .setWhitePawns(static_cast<uint64_t>(Square::E7))
            .setBlackCastleQueenside(true)
            .Build();
        
        REQUIRE_FALSE(customBoard->makeMove({Square::E8, Square::C8}));
    }

    SECTION("Black pawn capture is a legal move") {
         auto customBoard = BoardBuilder(
            "rnbqk.nr"
            "p.pp.ppp"
            ".p..p..."
            "...N...."
            ".b..P..."
            ".....N.."
            "PPPP.PPP"
            "R.BQKB.R", Color::BLACK).Build();

        REQUIRE(customBoard->makeMove({Square::E6, Square::D5}));
    }
}

TEST_CASE("Board::isKingInCheckmate", "[isKingInCheckmate]") {
    SECTION("Stalemate is not checkmate") {
        auto board = BoardBuilder(
            "....K..."
            "........"
            "...r.q.."
            "........"
            "........"
            "........"
            "........"
            ".......k", Color::WHITE).Build();
        
        REQUIRE_FALSE(board->isKingInCheckmate(Color::WHITE));
    }

    SECTION("Checkmate 1") {
        auto board = BoardBuilder(
            "....k..."
            "........"
            "...RQQ.."
            "........"
            "........"
            "........"
            "........"
            ".......K", Color::BLACK).Build();
        
        REQUIRE(board->isKingInCheckmate(Color::BLACK));
    }

    SECTION("Checkmate 2") {
        auto board = BoardBuilder(
            "....k..."
            "........"
            "........"
            "........"
            "........"
            "........"
            ".r......"
            "r......K", Color::WHITE).Build();
        
        REQUIRE(board->isKingInCheckmate(Color::WHITE));
    }

    SECTION("Checkmate 3") {
        auto board = BoardBuilder(
            "....k..."
            "........"
            "........"
            "........"
            "....B..."
            "...P...."
            "r......."
            ".r.....K", Color::WHITE).Build();
        
        REQUIRE(board->isKingInCheckmate(Color::WHITE));
    }

    SECTION("Not Checkmate 1") {
        auto board = BoardBuilder(
            "....k..."
            "........"
            "........"
            "........"
            "B......."
            ".....N.."
            "........"
            "b.....K.", Color::BLACK).Build();
        
        REQUIRE_FALSE(board->isKingInCheckmate(Color::BLACK));
    }

    SECTION("Not Checkmate 2") {
        auto board = BoardBuilder(
            "....k..."
            "r......."
            "...RQQ.."
            "........"
            "......."
            "........"
            "........"
            ".......K", Color::BLACK).Build();
        
        REQUIRE_FALSE(board->isKingInCheckmate(Color::BLACK));
    }

    SECTION("Not Checkmate 3") {
        auto board = BoardBuilder(
            "....K..."
            "........"
            "...r.q.."
            "........"
            "........"
            "........"
            "P......."
            ".......k", Color::WHITE).Build();
        
        REQUIRE_FALSE(board->isKingInCheckmate(Color::WHITE));
    }

    SECTION("Not Checkmate 4") {
        auto board = BoardBuilder(
            "....k..."
            "........"
            "........"
            "........"
            "....B..."
            "........"
            "r......."
            ".r.....K", Color::WHITE).Build();
        
        REQUIRE_FALSE(board->isKingInCheckmate(Color::WHITE));
    }
}

TEST_CASE("Board::generateLegalMoves", "[generateLegalMoves]") {
    SECTION("Standard board has 20 legal moves") {
        std::unique_ptr<Board> board = StandardBoard();

        REQUIRE(board->generateLegalMoves().size() == 20);
    }

    SECTION("Pawn promotion and capture has 4 legal moves") {
        auto board = BoardBuilder(
            ".rr....."
            ".P......"
            "........"
            "........"
            "........"
            ".k......"
            "........"
            ".......K", Color::WHITE).Build();
        
        // includes the 3 king moves.
        REQUIRE(board->generateLegalMoves().size() == 7);
    }

    SECTION("Pawn promotion has 4 legal moves") {
        auto board = BoardBuilder(
            "........"
            ".P......"
            "........"
            "........"
            "........"
            ".k......"
            "........"
            ".......K", Color::WHITE).Build();
        
        // includes the 3 king moves.
        REQUIRE(board->generateLegalMoves().size() == 7);
    }

    SECTION("Pawn promotion and capture has 8 legal moves") {
        auto board = BoardBuilder(
            "..r....."
            ".P......"
            "........"
            "........"
            "........"
            ".k......"
            "........"
            ".......K", Color::WHITE).Build();
        
        // includes the 3 king moves.
        REQUIRE(board->generateLegalMoves().size() == 11);
    }
}