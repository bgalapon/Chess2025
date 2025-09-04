#include <iostream>
#include <memory>
#include <string>

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

class Board {
public:
    void setBlackBishops(uint64_t squares) { this->blackBishops = squares; }
    void setBlackKing(uint64_t square) { this->blackKing = square; }
    void setBlackKnights(uint64_t squares) { this->blackKnights = squares; }
    void setBlackPawns(uint64_t squares) { this->blackPawns = squares; }
    void setBlackQueens(uint64_t squares) { this->blackQueens = squares; }
    void setBlackRooks(uint64_t squares) { this->blackRooks = squares; }

    void setWhiteBishops(uint64_t squares) { this->whiteBishops = squares; }
    void setWhiteKing(uint64_t square) { this->whiteKing = square; }
    void setWhiteKnights(uint64_t squares) { this->whiteKnights = squares; }
    void setWhitePawns(uint64_t squares) { this->whitePawns = squares; }
    void setWhiteQueens(uint64_t squares) { this->whiteQueens = squares; }
    void setWhiteRooks(uint64_t squares) { this->whiteRooks = squares; }

    void setBlackCastleKingside(bool canCastle) { this->blackCastleKingside = canCastle; }
    void setBlackCastleQueenside(bool canCastle) { this->blackCastleQueenside = canCastle; }
    void setWhiteCastleKingside(bool canCastle) { this->whiteCastleKingside = canCastle; }
    void setWhiteCastleQueenside(bool canCastle) { this->whiteCastleQueenside = canCastle; }
    
    void setEnPassent(uint64_t square) { this->enPassent = square; }

    void setSideToMove(Color sideToMove) { this->sideToMove = sideToMove; }

    std::string toString() const {
        std::string result = "";
        for (int rank = 7; rank >= 0; --rank) { // Iterate ranks from 8 to 1
            for (int file = 0; file < 8; ++file) { // Iterate files from A to H
                uint64_t square_bit = 1ULL << (rank * 8 + file);
                
                char piece_char = '.'; // Default for an empty square

                // Check for each piece type
                if ((blackRooks & square_bit) != 0) piece_char = 'r';
                else if ((blackKnights & square_bit) != 0) piece_char = 'n';
                else if ((blackBishops & square_bit) != 0) piece_char = 'b';
                else if ((blackQueens & square_bit) != 0) piece_char = 'q';
                else if ((blackKing & square_bit) != 0) piece_char = 'k';
                else if ((blackPawns & square_bit) != 0) piece_char = 'p';
                
                else if ((whiteRooks & square_bit) != 0) piece_char = 'R';
                else if ((whiteKnights & square_bit) != 0) piece_char = 'N';
                else if ((whiteBishops & square_bit) != 0) piece_char = 'B';
                else if ((whiteQueens & square_bit) != 0) piece_char = 'Q';
                else if ((whiteKing & square_bit) != 0) piece_char = 'K';
                else if ((whitePawns & square_bit) != 0) piece_char = 'P';

                result += piece_char;
            }
            result += '\n'; // Add a newline after each rank
        }
        return result;
    }

private:
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
    BoardBuilder(Square blackKingSquare, Square whiteKingSquare, Color sideToMove) : board(std::make_unique<Board>()) {
        board->setBlackKing(static_cast<uint64_t>(blackKingSquare));
        board->setWhiteKing(static_cast<uint64_t>(whiteKingSquare));
        board->setSideToMove(sideToMove);
    }

    BoardBuilder& setBlackBishops(uint64_t squares) { board->setBlackBishops(squares); return *this;}
    BoardBuilder& setBlackKnights(uint64_t squares) { board->setBlackKnights(squares); return *this;}
    BoardBuilder& setBlackPawns(uint64_t squares) { board->setBlackPawns(squares); return *this;}
    BoardBuilder& setBlackQueens(uint64_t squares) { board->setBlackQueens(squares); return *this;}
    BoardBuilder& setBlackRooks(uint64_t squares) { board->setBlackRooks(squares); return *this;}

    BoardBuilder& setWhiteBishops(uint64_t squares) { board->setWhiteBishops(squares); return *this;}
    BoardBuilder& setWhiteKnights(uint64_t squares) { board->setWhiteKnights(squares); return *this;}
    BoardBuilder& setWhitePawns(uint64_t squares) { board->setWhitePawns(squares); return *this;}
    BoardBuilder& setWhiteQueens(uint64_t squares) { board->setWhiteQueens(squares); return *this;}
    BoardBuilder& setWhiteRooks(uint64_t squares) { board->setWhiteRooks(squares); return *this;}

    BoardBuilder& setBlackCastleKingside(bool canCastle) { board->setBlackCastleKingside(canCastle); return *this; }
    BoardBuilder& setBlackCastleQueenside(bool canCastle) { board->setBlackCastleQueenside(canCastle); return *this; }
    BoardBuilder& setWhiteCastleKingside(bool canCastle) { board->setWhiteCastleKingside(canCastle); return *this; }
    BoardBuilder& setWhiteCastleQueenside(bool canCastle) { board->setWhiteCastleQueenside(canCastle); return *this; }

    BoardBuilder& setEnPassent(uint64_t enPassent) { board->setEnPassent(enPassent); return *this; }

    std::unique_ptr<Board> Build() { return std::move(board); }

private:
    std::unique_ptr<Board> board;
};

int main() {
    BoardBuilder boardBuilder(Square::A1, Square::B4, Color::WHITE);
    std::unique_ptr<Board> board = boardBuilder
        .setBlackBishops(10)
        .Build();

    std::cout << board.get()->toString() << std::endl;
    return 0;
}