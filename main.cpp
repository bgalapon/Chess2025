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
        return "TODO";
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
    BoardBuilder boardBuilder = BoardBuilder(0, 0);
    std::unique_ptr<Board> board = boardBuilder
        .setBlackBishops(10)
        .Build();

    std::cout << board.get()->toString() << std::endl;
    return 0;
}