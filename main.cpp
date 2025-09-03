#include <iostream>
#include <memory>
#include <string>

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
};

class BoardBuilder {
public:
    BoardBuilder(uint64_t blackKingSquare, uint64_t whiteKingSquare) : board(std::make_unique<Board>()) {
        board->setBlackKing(blackKingSquare);
        board->setWhiteKing(whiteKingSquare);
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
    BoardBuilder boardBuilder = BoardBuilder(1, 128);
    std::unique_ptr<Board> board = boardBuilder
        .setBlackBishops(10)
        .Build();

    std::cout << board.get()->toString() << std::endl;
    return 0;
}