#include "board.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

// Helper function to get the index (0-63) from a single-bit bitboard
inline uint8_t getSquareIndex(uint64_t bitboard) {
    if (bitboard == 0) return 64; // Sentinel value for invalid square
    return __builtin_ctzll(bitboard);
}

// Bitboard masks for ranks for efficient checking
const uint64_t RANK_2 = 0x000000000000FF00ULL;
const uint64_t RANK_4 = 0x00000000FF000000ULL;
const uint64_t RANK_5 = 0x000000FF00000000ULL;
const uint64_t RANK_7 = 0x00FF000000000000ULL;

// Bitboard masks for castling paths
const uint64_t WHITE_KINGSIDE_CASTLE_PATH = 112ULL;
const uint64_t WHITE_QUEENSIDE_CASTLE_PATH = 14ULL;
const uint64_t BLACK_KINGSIDE_CASTLE_PATH = 6917529027641081856ULL;
const uint64_t BLACK_QUEENSIDE_CASTLE_PATH = 1008806316530991104ULL;

// Precomputed file masks
const uint64_t file_masks[8] = {
    0x0101010101010101ULL, // File A
    0x0202020202020202ULL, // File B
    0x0404040404040404ULL, // File C
    0x0808080808080808ULL, // File D
    0x1010101010101010ULL, // File E
    0x2020202020202020ULL, // File F
    0x4040404040404040ULL, // File G
    0x8080808080808080ULL  // File H
};

// Board class implementations
Board::Board() : blackBishops(0), blackKing(0), blackKnights(0), blackPawns(0), blackQueens(0), blackRooks(0),
              whiteBishops(0), whiteKing(0), whiteKnights(0), whitePawns(0), whiteQueens(0), whiteRooks(0),
              blackCastleKingside(false), blackCastleQueenside(false), whiteCastleKingside(false), whiteCastleQueenside(false),
              enPassent(0), sideToMove(Color::WHITE) {}

Board::Board(const Board& other) = default;

void Board::setBlackBishops(uint64_t squares) { this->blackBishops = squares; }
void Board::setBlackKing(uint64_t square) { this->blackKing = square; }
void Board::setBlackKnights(uint64_t squares) { this->blackKnights = squares; }
void Board::setBlackPawns(uint64_t squares) { this->blackPawns = squares; }
void Board::setBlackQueens(uint64_t squares) { this->blackQueens = squares; }
void Board::setBlackRooks(uint64_t squares) { this->blackRooks = squares; }
void Board::setWhiteBishops(uint64_t squares) { this->whiteBishops = squares; }
void Board::setWhiteKing(uint64_t square) { this->whiteKing = square; }
void Board::setWhiteKnights(uint64_t squares) { this->whiteKnights = squares; }
void Board::setWhitePawns(uint64_t squares) { this->whitePawns = squares; }
void Board::setWhiteQueens(uint64_t squares) { this->whiteQueens = squares; }
void Board::setWhiteRooks(uint64_t squares) { this->whiteRooks = squares; }
void Board::setBlackCastleKingside(bool canCastle) { this->blackCastleKingside = canCastle; }
void Board::setBlackCastleQueenside(bool canCastle) { this->blackCastleQueenside = canCastle; }
void Board::setWhiteCastleKingside(bool canCastle) { this->whiteCastleKingside = canCastle; }
void Board::setWhiteCastleQueenside(bool canCastle) { this->whiteCastleQueenside = canCastle; }
void Board::setEnPassent(uint64_t square) { this->enPassent = square; }
void Board::setSideToMove(Color sideToMove) { this->sideToMove = sideToMove; }

 // Getter methods
uint64_t Board::getBlackBishops() { return this->blackBishops; }
uint64_t Board::getBlackKing() { return this->blackKing; }
uint64_t Board::getBlackKnights() { return this->blackKnights; }
uint64_t Board::getBlackPawns() { return this->blackPawns; }
uint64_t Board::getBlackQueens() { return this->blackQueens; }
uint64_t Board::getBlackRooks() { return this->blackRooks; }

uint64_t Board::getWhiteBishops() { return this->whiteBishops; }
uint64_t Board::getWhiteKing() { return this->whiteKing; }
uint64_t Board::getWhiteKnights() { return this->whiteKnights; }
uint64_t Board::getWhitePawns() { return this->whitePawns; }
uint64_t Board::getWhiteQueens(){ return this->whiteQueens; } 
uint64_t Board::getWhiteRooks() { return this->whiteRooks; }

bool Board::getBlackCastleKingside() { return this->blackCastleKingside; }
bool Board::getBlackCastleQueenside() { return this->blackCastleQueenside; }
bool Board::getWhiteCastleKingside() { return this->whiteCastleKingside; }
bool Board::getWhiteCastleQueenside() { return this->whiteCastleQueenside; }

uint64_t Board::getEnPassent() { return this->enPassent; }

Color Board::getSideToMove() { return this->sideToMove; }

std::string Board::toString() const {
    std::string result = "";
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            uint64_t square_bit = 1ULL << (rank * 8 + file);
            char piece_char = '.';
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
        result += '\n';
    }
    return result;
}

bool Board::applyMove(Square start, Square end) {
    uint64_t start_bit = static_cast<uint64_t>(start);
    uint64_t end_bit = static_cast<uint64_t>(end);
    
    uint64_t friendlyPieces, enemyPieces, allPieces;
    if (this->sideToMove == Color::WHITE) {
        friendlyPieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
        enemyPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
    } else {
        friendlyPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
        enemyPieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
    }
    allPieces = friendlyPieces | enemyPieces;

    // --- Handle Castling Moves First ---
    if (this->sideToMove == Color::WHITE) {
        if ((whiteKing & start_bit) && (whiteRooks & static_cast<uint64_t>(Square::H1)) && start == Square::E1 && end == Square::G1 && this->whiteCastleKingside) {
            if (areSquaresAttacked(WHITE_KINGSIDE_CASTLE_PATH, Color::BLACK)) {
                std::cerr << "white king castling kingside out of, into, or through check" << std::endl;
                return false;
            }
            if (!((friendlyPieces | enemyPieces) & (WHITE_KINGSIDE_CASTLE_PATH &~whiteKing))) {
                whiteKing = static_cast<uint64_t>(Square::G1);
                whiteRooks &= ~static_cast<uint64_t>(Square::H1);
                whiteRooks |= static_cast<uint64_t>(Square::F1);
                return true;
            }
        }
        else if ((whiteKing & start_bit) && (whiteRooks & static_cast<uint64_t>(Square::A1)) && start == Square::E1 && end == Square::C1 && this->whiteCastleQueenside) {
            if (areSquaresAttacked(WHITE_QUEENSIDE_CASTLE_PATH, Color::BLACK)) {
                std::cerr << "white king castling queenside out of, into, or through check" << std::endl;
                return false;
            }
            if (!((friendlyPieces | enemyPieces) & (WHITE_QUEENSIDE_CASTLE_PATH &~whiteKing))) {
                whiteKing = static_cast<uint64_t>(Square::C1);
                whiteRooks &= ~static_cast<uint64_t>(Square::A1);
                whiteRooks |= static_cast<uint64_t>(Square::D1);
                return true;
            }
        }
    } else {
        if ((blackKing & start_bit) && (blackRooks & static_cast<uint64_t>(Square::H8)) && start == Square::E8 && end == Square::G8 && this->blackCastleKingside) {
            if (areSquaresAttacked(BLACK_KINGSIDE_CASTLE_PATH, Color::WHITE)) {
                std::cerr << "black king castling kingside out of, into, or through check" << std::endl;
                return false;
            }
            if (!((friendlyPieces | enemyPieces) & (BLACK_KINGSIDE_CASTLE_PATH &~blackKing))) {
                blackKing = static_cast<uint64_t>(Square::G8);
                blackRooks &= ~static_cast<uint64_t>(Square::H8);
                blackRooks |= static_cast<uint64_t>(Square::F8);
                return true;
            }
        }
        else if ((blackKing & start_bit) && (blackRooks & static_cast<uint64_t>(Square::A8)) && start == Square::E8 && end == Square::C8 && this->blackCastleQueenside) {
            if (areSquaresAttacked(BLACK_QUEENSIDE_CASTLE_PATH, Color::WHITE)) {
                std::cerr << "black king castling queenside out of, into, or through check" << std::endl;
                return false;
            }
            if (!((friendlyPieces | enemyPieces) & (BLACK_QUEENSIDE_CASTLE_PATH &~blackKing))) {
                blackKing = static_cast<uint64_t>(Square::C8);
                blackRooks &= ~static_cast<uint64_t>(Square::A8);
                blackRooks |= static_cast<uint64_t>(Square::D8);
                return true;
            }
        }
    }

    if ((friendlyPieces & start_bit) == 0) return false;
    if ((friendlyPieces & end_bit) != 0) return false;

    uint8_t start_rank = getSquareIndex(start_bit) >> 3;
    uint8_t start_file = getSquareIndex(start_bit) & 7;
    uint8_t end_rank = getSquareIndex(end_bit) >> 3;
    uint8_t end_file = getSquareIndex(end_bit) & 7;
    bool isPawnDoubleStep = false;
    
    if ((whitePawns & start_bit) || (blackPawns & start_bit)) {
        if (this->sideToMove == Color::WHITE) {
            if (end_bit == (start_bit << 8) && (allPieces & end_bit) == 0) {}
            else if ((start_bit & RANK_2) && (end_bit & RANK_4) && (allPieces & (end_bit | (start_bit << 8))) == 0) {
                isPawnDoubleStep = true; 
                this->enPassent = start_bit << 8;
            }
            else if ((end_bit == (start_bit << 7) || end_bit == (start_bit << 9)) && (enemyPieces & end_bit)) {}
            else if (end_bit == this->enPassent) { blackPawns &= ~(end_bit >> 8); }
            else {
                std::cerr << "white invalid pawn move" << std::endl;
                return false; 
            }
            whitePawns &= ~start_bit;
            whitePawns |= end_bit;
        } else {
            if (end_bit == (start_bit >> 8) && (allPieces & end_bit) == 0) {}
            else if ((start_bit & RANK_7) && (end_bit & RANK_5) && (allPieces & (end_bit | (start_bit >> 8))) == 0) { 
                isPawnDoubleStep = true;
                this->enPassent = start_bit >> 8;
            }
            else if ((end_bit == (start_bit >> 7) || end_bit == (start_bit >> 9)) && (enemyPieces & end_bit)) {}
            else if (end_bit == this->enPassent) { whitePawns &= ~(end_bit << 8); }
            else { return false; }
            blackPawns &= ~start_bit;
            blackPawns |= end_bit;
        }
    }
    else if ((whiteKnights & start_bit) || (blackKnights & start_bit)) {
        int rank_diff = std::abs(start_rank - end_rank);
        int file_diff = std::abs(start_file - end_file);
        if (!((rank_diff == 2 && file_diff == 1) || (rank_diff == 1 && file_diff == 2))) return false;
        if (whiteKnights & start_bit) { whiteKnights &= ~start_bit; whiteKnights |= end_bit; }
        else { blackKnights &= ~start_bit; blackKnights |= end_bit; }
    }
    else if ((whiteBishops & start_bit) || (blackBishops & start_bit)) {
        if (std::abs(start_rank - end_rank) != std::abs(start_file - end_file) || !isPathClear(start_bit, end_bit, allPieces)) return false;
        if (whiteBishops & start_bit) { whiteBishops &= ~start_bit; whiteBishops |= end_bit; }
        else { blackBishops &= ~start_bit; blackBishops |= end_bit; }
    }
    else if ((whiteRooks & start_bit) || (blackRooks & start_bit)) {
        bool isStraight = (start_rank == end_rank) || (start_file == end_file);
        if (!isStraight || !isPathClear(start_bit, end_bit, allPieces)) return false;
        if (whiteRooks & start_bit) { whiteRooks &= ~start_bit; whiteRooks |= end_bit; }
        else { blackRooks &= ~start_bit; blackRooks |= end_bit; }
    }
    else if ((whiteQueens & start_bit) || (blackQueens & start_bit)) {
        bool isHorizontal = start_rank == end_rank;
        bool isVertical = start_file == end_file;
        bool isDiagonal = std::abs(start_rank - end_rank) == std::abs(start_file - end_file);
        if ((!isHorizontal && !isVertical && !isDiagonal) || !isPathClear(start_bit, end_bit, allPieces)) return false;
        if (whiteQueens & start_bit) { whiteQueens &= ~start_bit; whiteQueens |= end_bit; }
        else { blackQueens &= ~start_bit; blackQueens |= end_bit; }
    }
    else if ((whiteKing & start_bit) || (blackKing & start_bit)) {
        int rank_diff = std::abs(start_rank - end_rank);
        int file_diff = std::abs(start_file - end_file);
        if (rank_diff > 1 || file_diff > 1) return false;
        if (whiteKing & start_bit) { whiteKing &= ~start_bit; whiteKing |= end_bit; }
        else { blackKing &= ~start_bit; blackKing |= end_bit; }
    } else {
        return false;
    }

    if (this->sideToMove == Color::WHITE) {
        blackPawns &= ~end_bit; blackKnights &= ~end_bit; blackBishops &= ~end_bit;
        blackRooks &= ~end_bit; blackQueens &= ~end_bit; blackKing &= ~end_bit;
    } else {
        whitePawns &= ~end_bit; whiteKnights &= ~end_bit; whiteBishops &= ~end_bit;
        whiteRooks &= ~end_bit; whiteQueens &= ~end_bit; whiteKing &= ~end_bit;
    }
    
    if (!isPawnDoubleStep) this->enPassent = 0;

    return true;
}

bool Board::isPathClear(uint64_t start_bit, uint64_t end_bit, uint64_t allPieces) {
    uint8_t start_index = getSquareIndex(start_bit);
    uint8_t end_index = getSquareIndex(end_bit);
    
    int start_rank = start_index >> 3;
    int start_file = start_index & 7;
    int end_rank = end_index >> 3;
    int end_file = end_index & 7;

    if (start_rank == end_rank) {
        uint64_t pathMask;
        if (start_bit < end_bit) pathMask = (end_bit - 1) & ~((start_bit << 1) - 1);
        else pathMask = (start_bit - 1) & ~((end_bit << 1) - 1);
        return (pathMask & allPieces) == 0;
    }
    if (start_file == end_file) {
        uint64_t pathMask = 0;
        int step = (start_index < end_index) ? 8 : -8;
        for (int i = start_index + step; i != end_index; i += step) pathMask |= (1ULL << i);
        return (pathMask & allPieces) == 0;
    }
    if (std::abs(start_rank - end_rank) == std::abs(start_file - end_file)) {
        int step_rank = (start_rank < end_rank) ? 8 : -8;
        int step_file = (start_file < end_file) ? 1 : -1;
        int step = step_rank + step_file;
        for (int i = start_index + step; i != end_index; i += step) {
            if ((allPieces & (1ULL << i)) != 0) return false;
        }
        return true;
    }
    return false;
}

uint64_t Board::getPawnAttacks(Color side, uint64_t pawns) {
    if (side == Color::WHITE) return ((pawns << 7) & ~file_masks[7]) | ((pawns << 9) & ~file_masks[0]);
    else return ((pawns >> 7) & ~file_masks[0]) | ((pawns >> 9) & ~file_masks[7]);
}

uint64_t Board::getKnightAttacks(uint64_t knights) {
    uint64_t attacks = 0;
    uint64_t knight = knights;
    while(knight) {
        uint64_t single_knight = knight & -knight;
        uint64_t target_attacks = 0;
        if (single_knight & ~file_masks[0] & ~file_masks[1]) target_attacks |= (single_knight << 6) | (single_knight >> 10);
        if (single_knight & ~file_masks[0]) target_attacks |= (single_knight << 15) | (single_knight >> 17);
        if (single_knight & ~file_masks[7] & ~file_masks[6]) target_attacks |= (single_knight << 10) | (single_knight >> 6);
        if (single_knight & ~file_masks[7]) target_attacks |= (single_knight << 17) | (single_knight >> 15);
        attacks |= target_attacks;
        knight &= knight - 1;
    }
    return attacks;
}

uint64_t Board::getKingAttacks(uint64_t king) {
    uint64_t attacks = 0;
    uint64_t right = (king << 1) & ~file_masks[0];
    uint64_t left = (king >> 1) & ~file_masks[7];
    uint64_t up = king << 8;
    uint64_t down = king >> 8;
    uint64_t up_right = (king << 9) & ~file_masks[0];
    uint64_t up_left = (king << 7) & ~file_masks[7];
    uint64_t down_right = (king >> 7) & ~file_masks[0];
    uint64_t down_left = (king >> 9) & ~file_masks[7];
    attacks = right | left | up | down | up_right | up_left | down_right | down_left;
    return attacks;
}

uint64_t Board::getSlidingAttacks(uint64_t pieces, uint64_t allPieces, bool isRook) {
    uint64_t attacks = 0;
    while (pieces) {
        uint64_t single_piece = pieces & -pieces;
        uint64_t occupied = allPieces;
        int shifts[] = { -1, 1, -8, 8 };
        if (!isRook) { shifts[0] = -9; shifts[1] = -7; shifts[2] = 7; shifts[3] = 9; }
        for (int shift : shifts) {
            uint64_t temp_attacks = 0;
            uint64_t current_square = single_piece;
            bool is_diagonal = (shift == 7 || shift == -7 || shift == 9 || shift == -9);
            bool is_left_move = (shift == -1 || shift == -9 || shift == 7);
            bool is_right_move = (shift == 1 || shift == 9 || shift == -7);
            while (true) {
                uint64_t next_square = (shift > 0) ? (current_square << std::abs(shift)) : (current_square >> std::abs(shift));
                if (next_square == 0 || (is_left_move && (next_square & file_masks[7])) || (is_right_move && (next_square & file_masks[0]))) break;
                if (is_diagonal) {
                    int current_rank = getSquareIndex(current_square) >> 3;
                    int next_rank = getSquareIndex(next_square) >> 3;
                    if (std::abs(current_rank - next_rank) != 1) break;
                }
                temp_attacks |= next_square;
                current_square = next_square;
                if (occupied & next_square) break;
            }
            attacks |= temp_attacks;
        }
        pieces &= pieces - 1;
    }
    return attacks;
}

// areSquaresAttacked checks if the given squares are attacked by the pieces of the given color
bool Board::areSquaresAttacked(uint64_t squares, Color opponentColor) {
    uint64_t opponentPawns = (opponentColor == Color::BLACK) ? blackPawns : whitePawns;
    uint64_t opponentKnights = (opponentColor == Color::BLACK) ? blackKnights : whiteKnights;
    uint64_t opponentBishops = (opponentColor == Color::BLACK) ? blackBishops : whiteBishops;
    uint64_t opponentRooks = (opponentColor == Color::BLACK) ? blackRooks : whiteRooks;
    uint64_t opponentQueens = (opponentColor == Color::BLACK) ? blackQueens : whiteQueens;
    uint64_t opponentKing = (opponentColor == Color::BLACK) ? blackKing : whiteKing;
    uint64_t allPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing |
                         whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
    if ((getPawnAttacks(opponentColor, opponentPawns) & squares) != 0) return true;
    if ((getKnightAttacks(opponentKnights) & squares) != 0) return true;
    if ((getKingAttacks(opponentKing) & squares) != 0) return true;
    if ((getSlidingAttacks(opponentRooks | opponentQueens, allPieces, true) & squares) != 0) return true;
    if ((getSlidingAttacks(opponentBishops | opponentQueens, allPieces, false) & squares) != 0) return true;
    return false;
}

bool Board::isKingInCheck(Color kingColor) {
    uint64_t kingSquare = (kingColor == Color::WHITE) ? whiteKing : blackKing;
    Color opponentColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return areSquaresAttacked(kingSquare, opponentColor);
}

bool Board::move(Square start, Square end) {
    Board tempBoard = *this;
    if (!tempBoard.applyMove(start, end)) {
        std::cerr << "Error: Invalid move based on piece rules or board state." << std::endl;
        return false;
    }
    if (tempBoard.isKingInCheck(this->sideToMove)) {
        std::cerr << "Error: This move leaves the king in check. Move is illegal." << std::endl;
        return false;
    }
    if (this->applyMove(start, end)) {
        if (this->sideToMove == Color::WHITE) this->sideToMove = Color::BLACK;
        else this->sideToMove = Color::WHITE;
        return true;
    }
    return false;
}

// BoardBuilder class implementations
BoardBuilder::BoardBuilder(Square blackKingSquare, Square whiteKingSquare, Color sideToMove) : board(std::make_unique<Board>()) {
    board->setBlackKing(static_cast<uint64_t>(blackKingSquare));
    board->setWhiteKing(static_cast<uint64_t>(whiteKingSquare));
    board->setSideToMove(sideToMove);
}

BoardBuilder& BoardBuilder::setBlackBishops(uint64_t squares) { board->setBlackBishops(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackKnights(uint64_t squares) { board->setBlackKnights(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackPawns(uint64_t squares) { board->setBlackPawns(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackQueens(uint64_t squares) { board->setBlackQueens(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackRooks(uint64_t squares) { board->setBlackRooks(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteBishops(uint64_t squares) { board->setWhiteBishops(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteKnights(uint64_t squares) { board->setWhiteKnights(squares); return *this;}
BoardBuilder& BoardBuilder::setWhitePawns(uint64_t squares) { board->setWhitePawns(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteQueens(uint64_t squares) { board->setWhiteQueens(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteRooks(uint64_t squares) { board->setWhiteRooks(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackCastleKingside(bool canCastle) { board->setBlackCastleKingside(canCastle); return *this; }
BoardBuilder& BoardBuilder::setBlackCastleQueenside(bool canCastle) { board->setBlackCastleQueenside(canCastle); return *this; }
BoardBuilder& BoardBuilder::setWhiteCastleKingside(bool canCastle) { board->setWhiteCastleKingside(canCastle); return *this; }
BoardBuilder& BoardBuilder::setWhiteCastleQueenside(bool canCastle) { board->setWhiteCastleQueenside(canCastle); return *this; }
BoardBuilder& BoardBuilder::setEnPassent(uint64_t enPassent) { board->setEnPassent(enPassent); return *this; }

std::unique_ptr<Board> BoardBuilder::Build() { return std::move(board); }

std::unique_ptr<Board> StandardBoard() {
    BoardBuilder boardBuilder(Square::E8, Square::E1, Color::WHITE);
    return boardBuilder
        .setWhitePawns(RANK_2)
        .setWhiteRooks(static_cast<uint64_t>(Square::A1) | static_cast<uint64_t>(Square::H1))
        .setWhiteKnights(static_cast<uint64_t>(Square::B1) | static_cast<uint64_t>(Square::G1))
        .setWhiteBishops(static_cast<uint64_t>(Square::C1) | static_cast<uint64_t>(Square::F1))
        .setWhiteQueens(static_cast<uint64_t>(Square::D1))
        .setWhiteCastleKingside(true)
        .setWhiteCastleQueenside(true)
        .setBlackPawns(RANK_7)
        .setBlackRooks(static_cast<uint64_t>(Square::A8) | static_cast<uint64_t>(Square::H8))
        .setBlackKnights(static_cast<uint64_t>(Square::B8) | static_cast<uint64_t>(Square::G8))
        .setBlackBishops(static_cast<uint64_t>(Square::C8) | static_cast<uint64_t>(Square::F8))
        .setBlackQueens(static_cast<uint64_t>(Square::D8))
        .setBlackCastleKingside(true)
        .setBlackCastleQueenside(true)
        .Build();
}