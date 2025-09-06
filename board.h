#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <cmath>

// Forward declaration of the Board class
class Board;

enum class Color {
    WHITE,
    BLACK
};

enum class Square : uint64_t {
    A1 = 1ULL << 0, B1 = 1ULL << 1, C1 = 1ULL << 2, D1 = 1ULL << 3, E1 = 1ULL << 4, F1 = 1ULL << 5, G1 = 1ULL << 6, H1 = 1ULL << 7,
    A2 = 1ULL << 8, B2 = 1ULL << 9, C2 = 1ULL << 10, D2 = 1ULL << 11, E2 = 1ULL << 12, F2 = 1ULL << 13, G2 = 1ULL << 14, H2 = 1ULL << 15,
    A3 = 1ULL << 16, B3 = 1ULL << 17, C3 = 1ULL << 18, D3 = 1ULL << 19, E3 = 1ULL << 20, F3 = 1ULL << 21, G3 = 1ULL << 22, H3 = 1ULL << 23,
    A4 = 1ULL << 24, B4 = 1ULL << 25, C4 = 1ULL << 26, D4 = 1ULL << 27, E4 = 1ULL << 28, F4 = 1ULL << 29, G4 = 1ULL << 30, H4 = 1ULL << 31,
    A5 = 1ULL << 32, B5 = 1ULL << 33, C5 = 1ULL << 34, D5 = 1ULL << 35, E5 = 1ULL << 36, F5 = 1ULL << 37, G5 = 1ULL << 38, H5 = 1ULL << 39,
    A6 = 1ULL << 40, B6 = 1ULL << 41, C6 = 1ULL << 42, D6 = 1ULL << 43, E6 = 1ULL << 44, F6 = 1ULL << 45, G6 = 1ULL << 46, H6 = 1ULL << 47,
    A7 = 1ULL << 48, B7 = 1ULL << 49, C7 = 1ULL << 50, D7 = 1ULL << 51, E7 = 1ULL << 52, F7 = 1ULL << 53, G7 = 1ULL << 54, H7 = 1ULL << 55,
    A8 = 1ULL << 56, B8 = 1ULL << 57, C8 = 1ULL << 58, D8 = 1ULL << 59, E8 = 1ULL << 60, F8 = 1ULL << 61, G8 = 1ULL << 62, H8 = 1ULL << 63
};

// --- Operator Overloads for Square ---

// Bitwise OR
// Combines two squares into a single bitboard
inline uint64_t operator|(Square a, Square b) {
    return static_cast<uint64_t>(a) | static_cast<uint64_t>(b);
}

// Bitwise AND
// Checks if a square exists within a bitboard
inline uint64_t operator&(Square a, Square b) {
    return static_cast<uint64_t>(a) & static_cast<uint64_t>(b);
}

class Board {
public:
    Board();
    Board(const Board& other);
    
    // Public member functions
    std::string toString() const;
    bool move(Square start, Square end);

    // Setter methods
    void setBlackBishops(uint64_t squares);
    void setBlackKing(uint64_t square);
    void setBlackKnights(uint64_t squares);
    void setBlackPawns(uint64_t squares);
    void setBlackQueens(uint64_t squares);
    void setBlackRooks(uint64_t squares);

    void setWhiteBishops(uint64_t squares);
    void setWhiteKing(uint64_t square);
    void setWhiteKnights(uint64_t squares);
    void setWhitePawns(uint64_t squares);
    void setWhiteQueens(uint64_t squares);
    void setWhiteRooks(uint64_t squares);

    void setBlackCastleKingside(bool canCastle);
    void setBlackCastleQueenside(bool canCastle);
    void setWhiteCastleKingside(bool canCastle);
    void setWhiteCastleQueenside(bool canCastle);
    
    void setEnPassent(uint64_t square);

    void setSideToMove(Color sideToMove);

    // Getter methods
    uint64_t getBlackBishops();
    uint64_t getBlackKing();
    uint64_t getBlackKnights();
    uint64_t getBlackPawns();
    uint64_t getBlackQueens();
    uint64_t getBlackRooks();

    uint64_t getWhiteBishops();
    uint64_t getWhiteKing();
    uint64_t getWhiteKnights();
    uint64_t getWhitePawns();
    uint64_t getWhiteQueens();
    uint64_t getWhiteRooks();

    bool getBlackCastleKingside();
    bool getBlackCastleQueenside();
    bool getWhiteCastleKingside();
    bool getWhiteCastleQueenside();
    
    uint64_t getEnPassent();

    Color getSideToMove();

    bool areSquaresAttacked(uint64_t squares, Color kingColor);
private:
    // Internal helper functions
    bool applyMove(Square start, Square end);
    bool isPathClear(uint64_t start_bit, uint64_t end_bit, uint64_t allPieces);
    bool isKingInCheckmate(Color kingColor);
    bool isKingInCheck(Color kingColor);
    uint64_t getPawnAttacks(Color side, uint64_t pawns);
    uint64_t getKnightAttacks(uint64_t knights);
    uint64_t getKingAttacks(uint64_t king);
    uint64_t getSlidingAttacks(uint64_t pieces, uint64_t allPieces, bool isRook);
    
    // Private member variables (bitboards and state flags)
    uint64_t blackBishops;
    uint64_t blackKing;
    uint64_t blackKnights;
    uint64_t blackPawns;
    uint64_t blackQueens;
    uint64_t blackRooks;
    uint64_t whiteBishops;
    uint64_t whiteKing;
    uint64_t whiteKnights;
    uint64_t whitePawns;
    uint64_t whiteQueens;
    uint64_t whiteRooks;

    bool blackCastleKingside;
    bool blackCastleQueenside;
    bool whiteCastleKingside;
    bool whiteCastleQueenside;
    uint64_t enPassent;
    Color sideToMove;
};

class BoardBuilder {
public:
    BoardBuilder(Square blackKingSquare, Square whiteKingSquare, Color sideToMove);

    BoardBuilder& setBlackBishops(uint64_t squares);
    BoardBuilder& setBlackKnights(uint64_t squares);
    BoardBuilder& setBlackPawns(uint64_t squares);
    BoardBuilder& setBlackQueens(uint64_t squares);
    BoardBuilder& setBlackRooks(uint64_t squares);

    BoardBuilder& setWhiteBishops(uint64_t squares);
    BoardBuilder& setWhiteKnights(uint64_t squares);
    BoardBuilder& setWhitePawns(uint64_t squares);
    BoardBuilder& setWhiteQueens(uint64_t squares);
    BoardBuilder& setWhiteRooks(uint64_t squares);

    BoardBuilder& setBlackCastleKingside(bool canCastle);
    BoardBuilder& setBlackCastleQueenside(bool canCastle);
    BoardBuilder& setWhiteCastleKingside(bool canCastle);
    BoardBuilder& setWhiteCastleQueenside(bool canCastle);
    
    BoardBuilder& setEnPassent(uint64_t enPassent);

    std::unique_ptr<Board> Build();

private:
    std::unique_ptr<Board> board;
};

std::unique_ptr<Board> StandardBoard();

#endif // BOARD_H
