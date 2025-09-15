#include "board.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>

// Helper function to get the index (0-63) from a single-bit bitboard
inline uint8_t getSquareIndex(uint64_t bitboard) {
    if (bitboard == 0) return 64; // Sentinel value for invalid square
    return __builtin_ctzll(bitboard);
}

// Converts a Square enum value to its algebraic notation string.
std::string toAlgebraicNotation(Square square) {
    uint64_t square_bitboard = static_cast<uint64_t>(square);
    int square_index = 63 - __builtin_clzll(square_bitboard);

    int file = square_index % 8;
    int rank = square_index / 8;

    char file_char = 'A' + file;
    char rank_char = '1' + rank;

    return std::string(1, file_char) + std::string(1, rank_char);
}

// Bitboard masks for ranks for efficient checking
const uint64_t RANK_2 = 0x000000000000FF00ULL;
const uint64_t RANK_4 = 0x00000000FF000000ULL;
const uint64_t RANK_5 = 0x000000FF00000000ULL;
const uint64_t RANK_7 = 0x00FF000000000000ULL;

// Bitboard masks for castling paths
const uint64_t WHITE_KINGSIDE_CASTLE_PATH = 112ULL;
const uint64_t WHITE_QUEENSIDE_CASTLE_PATH = 14ULL;
const uint64_t BLACK_KINGSIDE_CASTLE_PATH = 8070450532247928832ULL;
const uint64_t BLACK_QUEENSIDE_CASTLE_PATH = 2017612633061982208ULL;

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

bool Board::applyMove(Move move) {
    uint64_t start_bit = static_cast<uint64_t>(move.start);
    uint64_t end_bit = static_cast<uint64_t>(move.end);
    
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
        if ((whiteKing & start_bit) && (whiteRooks & static_cast<uint64_t>(Square::H1)) && move.start == Square::E1 && move.end == Square::G1 && this->whiteCastleKingside) {
            if (areSquaresAttacked(WHITE_KINGSIDE_CASTLE_PATH, Color::BLACK)) {
                if (verbose) std::cout << "white king castling kingside out of, into, or through check" << std::endl;
                return false;
            }
            if (!((friendlyPieces | enemyPieces) & (WHITE_KINGSIDE_CASTLE_PATH &~whiteKing))) {
                whiteKing = static_cast<uint64_t>(Square::G1);
                whiteRooks &= ~static_cast<uint64_t>(Square::H1);
                whiteRooks |= static_cast<uint64_t>(Square::F1);
                return true;
            }
        }
        else if ((whiteKing & start_bit) && (whiteRooks & static_cast<uint64_t>(Square::A1)) && move.start == Square::E1 && move.end == Square::C1 && this->whiteCastleQueenside) {
            if (areSquaresAttacked(WHITE_QUEENSIDE_CASTLE_PATH, Color::BLACK)) {
                if (verbose) std::cout << "white king castling queenside out of, into, or through check" << std::endl;
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
        if ((blackKing & start_bit) && (blackRooks & static_cast<uint64_t>(Square::H8)) && move.start == Square::E8 && move.end == Square::G8 && this->blackCastleKingside) {
            if (areSquaresAttacked(BLACK_KINGSIDE_CASTLE_PATH, Color::WHITE)) {
                if (verbose) std::cout << "black king castling kingside out of, into, or through check" << std::endl;
                return false;
            }
            if (!((friendlyPieces | enemyPieces) & (BLACK_KINGSIDE_CASTLE_PATH &~blackKing))) {
                blackKing = static_cast<uint64_t>(Square::G8);
                blackRooks &= ~static_cast<uint64_t>(Square::H8);
                blackRooks |= static_cast<uint64_t>(Square::F8);
                return true;
            }
        }
        else if ((blackKing & start_bit) && (blackRooks & static_cast<uint64_t>(Square::A8)) && move.start == Square::E8 && move.end == Square::C8 && this->blackCastleQueenside) {
            if (areSquaresAttacked(BLACK_QUEENSIDE_CASTLE_PATH, Color::WHITE)) {
                if (verbose) std::cout << "black king castling queenside out of, into, or through check" << std::endl;
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

    if ((friendlyPieces & start_bit) == 0) {
        if (verbose) std::cout << "no friendly piece on start square: " << toAlgebraicNotation(static_cast<Square>(start_bit)) << " moving to " <<  toAlgebraicNotation(static_cast<Square>(end_bit)) << std::endl;
        return false;
    }
    if ((friendlyPieces & end_bit) != 0) {
        if (verbose) std::cout << "can't move onto friendly piece" << std::endl;
        return false;
    }

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
                if (verbose) std::cout << "white invalid pawn move" << std::endl;
                return false; 
            }
            whitePawns &= ~start_bit;

            // Check for white pawn promotion
            if ((end_bit & 0xFF00000000000000ULL) != 0) {
                // Promote to the specified piece type
                switch (move.promotionPiece) {
                    case PieceType::QUEEN:
                        whiteQueens |= end_bit;
                        break;
                    case PieceType::ROOK:
                        whiteRooks |= end_bit;
                        break;
                    case PieceType::KNIGHT:
                        whiteKnights |= end_bit;
                        break;
                    case PieceType::BISHOP:
                        whiteBishops |= end_bit;
                        break;
                }
            } else {
                // Otherwise, place pawn on new square
                whitePawns |= end_bit;
            }
        } else {
            if (end_bit == (start_bit >> 8) && (allPieces & end_bit) == 0) {}
            else if ((start_bit & RANK_7) && (end_bit & RANK_5) && (allPieces & (end_bit | (start_bit >> 8))) == 0) { 
                isPawnDoubleStep = true;
                this->enPassent = start_bit >> 8;
            }
            else if ((end_bit == (start_bit >> 7) || end_bit == (start_bit >> 9)) && (enemyPieces & end_bit)) {}
            else if (end_bit == this->enPassent) { whitePawns &= ~(end_bit << 8); }
            else {
                if (verbose) std::cout << "black invalid pawn move" << std::endl;
                return false; 
            }
            blackPawns &= ~start_bit;

            // Check for black pawn promotion
            if ((end_bit & 0x00000000000000FFULL) != 0) {
                // Promote to the specified piece type
                switch (move.promotionPiece) {
                    case PieceType::QUEEN:
                        blackQueens |= end_bit;
                        break;
                    case PieceType::ROOK:
                        blackRooks |= end_bit;
                        break;
                    case PieceType::KNIGHT:
                        blackKnights |= end_bit;
                        break;
                    case PieceType::BISHOP:
                        blackBishops |= end_bit;
                        break;
                }
            } else {
                // Otherwise, place pawn on new square
                blackPawns |= end_bit;
            }
        }
    }
    else if ((whiteKnights & start_bit) || (blackKnights & start_bit)) {
        int rank_diff = std::abs(start_rank - end_rank);
        int file_diff = std::abs(start_file - end_file);
        if (!((rank_diff == 2 && file_diff == 1) || (rank_diff == 1 && file_diff == 2))) {
            if (verbose) std::cout << "white invalid knight move" << std::endl;
            return false;
        }
        if (whiteKnights & start_bit) { whiteKnights &= ~start_bit; whiteKnights |= end_bit; }
        else { blackKnights &= ~start_bit; blackKnights |= end_bit; }
    }
    else if ((whiteBishops & start_bit) || (blackBishops & start_bit)) {
        if (std::abs(start_rank - end_rank) != std::abs(start_file - end_file) || !isPathClear(start_bit, end_bit, allPieces)) {
            if (verbose) std::cout << "white invalid bishop move" << std::endl;
            return false;
        }
        if (whiteBishops & start_bit) { whiteBishops &= ~start_bit; whiteBishops |= end_bit; }
        else { blackBishops &= ~start_bit; blackBishops |= end_bit; }
    }
    else if ((whiteRooks & start_bit) || (blackRooks & start_bit)) {
        bool isStraight = (start_rank == end_rank) || (start_file == end_file);
        if (!isStraight || !isPathClear(start_bit, end_bit, allPieces)) {
            if (verbose) std::cout << "black invalid knight move" << std::endl;
            return false;
        }
        if (whiteRooks & start_bit) { whiteRooks &= ~start_bit; whiteRooks |= end_bit; }
        else { blackRooks &= ~start_bit; blackRooks |= end_bit; }
    }
    else if ((whiteQueens & start_bit) || (blackQueens & start_bit)) {
        bool isHorizontal = start_rank == end_rank;
        bool isVertical = start_file == end_file;
        bool isDiagonal = std::abs(start_rank - end_rank) == std::abs(start_file - end_file);
        if ((!isHorizontal && !isVertical && !isDiagonal) || !isPathClear(start_bit, end_bit, allPieces)) {
            if (verbose) std::cout << "invalid queen move" << std::endl;
            return false;
        }
        if (whiteQueens & start_bit) { whiteQueens &= ~start_bit; whiteQueens |= end_bit; }
        else { blackQueens &= ~start_bit; blackQueens |= end_bit; }
    }
    else if ((whiteKing & start_bit) || (blackKing & start_bit)) {
        int rank_diff = std::abs(start_rank - end_rank);
        int file_diff = std::abs(start_file - end_file);
        if (rank_diff > 1 || file_diff > 1) {
            if (verbose) std::cout << "invalid king move" << std::endl;
            return false;
        }
        if (whiteKing & start_bit) { whiteKing &= ~start_bit; whiteKing |= end_bit; }
        else { blackKing &= ~start_bit; blackKing |= end_bit; }
    } else {
        if (verbose) std::cout << "invalid move, can't detect any piece" << std::endl;
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

void Board::setVerbose(bool verbose) {
    this->verbose = verbose;
}

bool Board::isInsufficientMaterial() {
    int numWhiteKnights = std::popcount(whiteKnights);
    int numWhiteRooks = std::popcount(whiteRooks);
    int numWhiteQueens = std::popcount(whiteQueens);
    int numWhitePawns = std::popcount(whitePawns);
    int numWhiteBishops = std::popcount(whiteBishops);

    int numBlackKnights = std::popcount(blackKnights);
    int numBlackRooks = std::popcount(blackRooks);
    int numBlackQueens = std::popcount(blackQueens);
    int numBlackPawns = std::popcount(blackPawns);
    int numBlackBishops = std::popcount(blackBishops);

    int numWhitePieces = numWhiteKnights + numWhiteRooks + numWhiteQueens + numWhitePawns + numWhiteBishops;
    int numBlackPieces = numBlackKnights + numBlackRooks + numBlackQueens + numBlackPawns + numBlackBishops;

    // King vs King
    if (numWhitePieces == 0 && numBlackPieces == 0) {
        return true;
    }

    // King and one Knight vs King
    if (numWhitePieces == 0 && numBlackPieces == 1 && numBlackKnights == 1) {
        return true;
    }
    if (numBlackPieces == 0 && numWhitePieces == 1 && numWhiteKnights == 1) {
        return true;
    }

    // King and one Bishop vs King
    if (numWhitePieces == 0 && numBlackPieces == 1 && numBlackBishops == 1) {
        return true;
    }
    if (numBlackPieces == 0 && numWhitePieces == 1 && numWhiteBishops == 1) {
        return true;
    }

    if (numWhitePieces == 1 && numWhiteBishops == 1 && numBlackPieces == 1 && numBlackBishops == 1) {
        if (getSquareIndex(whiteBishops) + getSquareIndex(blackBishops) % 2 == 0) {
            return true;
        }
    }

    // More complex scenarios (e.g., King & Bishop vs. King & Bishop on same color squares)
    // could be added here for a more comprehensive check.
    // For now, this is a sufficient start.

    return false;
}

bool Board::makeMove(Move move) {
    Board tempBoard = *this;
    if (!tempBoard.applyMove(move)) {
        std::cout << "Error: Invalid move based on piece rules or board state." << std::endl;
        return false;
    }
    if (tempBoard.isKingInCheck(this->sideToMove)) {
        std::cout << "Error: This move leaves the king in check. Move is illegal." << std::endl;
        return false;
    }
    if (this->applyMove(move)) {
        if (this->sideToMove == Color::WHITE) this->sideToMove = Color::BLACK;
        else this->sideToMove = Color::WHITE;
        return true;
    }
    return false;
}

bool Board::isKingInCheckmate(Color kingColor) {
    // 1. First, check if the king is even in check. If not, it can't be checkmate.
    Color opponentColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    uint64_t kingSquare = (kingColor == Color::WHITE) ? whiteKing : blackKing;

    if (!isKingInCheck(kingColor)) {
        // std::cout << "I'm not in check on: " << toAlgebraicNotation(static_cast<Square>(kingSquare)) << std::endl;
        return false;
    }

    // 2. Generate and check all possible king moves.
    // This is the fastest way to get out of check.
    uint64_t kingAttacks = getKingAttacks(kingSquare);
    uint64_t friendlyPieces = (kingColor == Color::WHITE) ? 
        (whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing) :
        (blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing);
    uint64_t kingLegalMoves = kingAttacks & ~friendlyPieces;
    
    while (kingLegalMoves) {
        uint64_t end_bit = kingLegalMoves & -kingLegalMoves;
        // std::cout << "Considering king move: " << toAlgebraicNotation(static_cast<Square>(kingSquare)) << " -> " << toAlgebraicNotation(static_cast<Square>(end_bit)) << std::endl;
        Board tempBoard = *this;
        if (tempBoard.applyMove({static_cast<Square>(kingSquare), static_cast<Square>(end_bit)})) {
            if (!tempBoard.isKingInCheck(kingColor)) {
                return false; // King can move to a safe square
            }
            // std::cout << "king is attacked on: " << toAlgebraicNotation(static_cast<Square>(end_bit)) << std::endl;
        }
        kingLegalMoves &= kingLegalMoves - 1;
    }
    
    // 3. Find the attacking piece(s).
    // Note: This logic assumes a single check. Handling double checks is a special case.
    uint64_t checkers = 0;
    
    uint64_t opponentPawns = (opponentColor == Color::BLACK) ? blackPawns : whitePawns;
    uint64_t opponentKnights = (opponentColor == Color::BLACK) ? blackKnights : whiteKnights;
    uint64_t opponentBishops = (opponentColor == Color::BLACK) ? blackBishops : whiteBishops;
    uint64_t opponentRooks = (opponentColor == Color::BLACK) ? blackRooks : whiteRooks;
    uint64_t opponentQueens = (opponentColor == Color::BLACK) ? blackQueens : whiteQueens;
    uint64_t allPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing |
                         whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;

    // Check for pawn attacks
    if (opponentColor == Color::BLACK) {
        checkers |= ((kingSquare << 7) & ~file_masks[7]) & opponentPawns;
        checkers |= ((kingSquare << 9) & ~file_masks[0]) & opponentPawns;
    } else {
        checkers |= ((kingSquare >> 7) & ~file_masks[0]) & opponentPawns;
        checkers |= ((kingSquare >> 9) & ~file_masks[7]) & opponentPawns;
    }

    // Check for knight attacks
    if (getKnightAttacks(opponentKnights) & kingSquare) {
        checkers |= getKnightAttacks(kingSquare) & opponentKnights;
    }

    // Check for rook/queen attacks
    if (getSlidingAttacks(opponentRooks | opponentQueens, allPieces, true) & kingSquare) {
        checkers |= getSlidingAttacks(kingSquare, allPieces, true) & (opponentRooks | opponentQueens);
    }
    
    // Check for bishop/queen attacks
    if (getSlidingAttacks(opponentBishops | opponentQueens, allPieces, false) & kingSquare) {
        checkers |= getSlidingAttacks(kingSquare, allPieces, false) & (opponentBishops | opponentQueens);
    }
    // Don't check for king-on-king attacks, as they are not legal.

    // If there is more than one checking piece, it's a double check.
    // In a double check, the only way out is to move the king. Since we
    // already checked all king moves above, if we're here, it's checkmate.
    if (__builtin_popcountll(checkers) > 1) {
        return true;
    }

    uint64_t checker_square = checkers;

    // 4. Check for captures of the checking piece.
    // Iterate through all friendly pieces (except the king) to see if they can capture the checker.
    uint64_t friendlyPiecesExceptKing = friendlyPieces & ~kingSquare;
    
    while(friendlyPiecesExceptKing) {
        uint64_t start_bit = friendlyPiecesExceptKing & -friendlyPiecesExceptKing;
        Board tempBoard = *this;
        // The applyMove function should handle the capture
        if (tempBoard.applyMove({static_cast<Square>(start_bit), static_cast<Square>(checker_square)})) {
            if (!tempBoard.isKingInCheck(kingColor)) {
                return false; // Found a legal capture
            }
        }
        friendlyPiecesExceptKing &= friendlyPiecesExceptKing - 1;
    }
    
    // 5. Check for blocks if the checker is a sliding piece.
    if (!((blackKnights | whiteKnights | blackPawns | whitePawns) & checker_square)) {
        // Only sliding pieces can be blocked.
        uint64_t path = 0;
        uint8_t start_index = getSquareIndex(checker_square);
        uint8_t end_index = getSquareIndex(kingSquare);
        
        // This is a rough-and-ready way to get the path. A robust implementation
        // would have precomputed tables or better pathfinding.
        if (start_index / 8 == end_index / 8) { // Horizontal
            path = ((1ULL << std::max(start_index, end_index)) - 1) ^ ((1ULL << std::min(start_index, end_index)) - 1);
            path &= ~(kingSquare | checker_square);
        } else if (start_index % 8 == end_index % 8) { // Vertical
            uint64_t start_file = file_masks[start_index % 8];
            path = ((start_file << std::min(start_index, end_index)) - 1) & ~((start_file << std::max(start_index, end_index)) - 1);
            path &= ~(kingSquare | checker_square);
        } else { // Diagonal
            int step = (start_index < end_index) ? 
                ( (start_index % 8 < end_index % 8) ? 9 : 7 ) :
                ( (start_index % 8 < end_index % 8) ? -7 : -9 );
            for (int i = start_index + step; i != end_index; i += step) {
                path |= (1ULL << i);
            }
        }

        uint64_t friendlyBlockers = (kingColor == Color::WHITE) ?
            (whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens) :
            (blackPawns | blackKnights | blackBishops | blackRooks | blackQueens);

        while(friendlyBlockers) {
            uint64_t start_bit = friendlyBlockers & -friendlyBlockers;
            uint64_t end_squares = path & ~friendlyPieces;
            
            while(end_squares) {
                uint64_t end_bit = end_squares & -end_squares;
                Board tempBoard = *this;
                if (tempBoard.applyMove({static_cast<Square>(start_bit), static_cast<Square>(end_bit)})) {
                    if (!tempBoard.isKingInCheck(kingColor)) {
                        return false; // Found a legal block
                    }
                }
                end_squares &= end_squares - 1;
            }
            friendlyBlockers &= friendlyBlockers - 1;
        }
    }
    
    // If we've reached this point, no legal moves were found to get out of check.
    return true;
}

std::vector<Move> Board::generatePseudoLegalMoves() {
    std::vector<Move> pseudoLegalMoves;

    std::vector<Move> pawnMoves = generatePawnMoves();
    // for (const Move& move : pawnMoves) {
    //     std::cout << "pawn move (" << toAlgebraicNotation(move.start) << ", " << toAlgebraicNotation(move.end) << ")" << std::endl;
    // }

    std::vector<Move> knightMoves = generateKnightMoves();
    // for (const Move& move : knightMoves) {
    //     std::cout << "knight move (" << toAlgebraicNotation(move.start) << ", " << toAlgebraicNotation(move.end) << ")" << std::endl;
    // }

    std::vector<Move> bishopMoves = generateBishopMoves();
    // for (const Move& move : bishopMoves) {
    //     std::cout << "bishop move (" << toAlgebraicNotation(move.start) << ", " << toAlgebraicNotation(move.end) << ")" << std::endl;
    // }

    std::vector<Move> rookMoves = generateRookMoves();
    // for (const Move& move : rookMoves) {
    //     std::cout << "rook move (" << toAlgebraicNotation(move.start) << ", " << toAlgebraicNotation(move.end) << ")" << std::endl;
    // }

    std::vector<Move> queenMoves = generateQueenMoves();
    // for (const Move& move : queenMoves) {
    //     std::cout << "queen move (" << toAlgebraicNotation(move.start) << ", " << toAlgebraicNotation(move.end) << ")" << std::endl;
    // }

    std::vector<Move> kingMoves = generateKingMoves();
    // for (const Move& move : kingMoves) {
    //     std::cout << "king move (" << toAlgebraicNotation(move.start) << ", " << toAlgebraicNotation(move.end) << ")" << std::endl;
    // }
    
    // Generate all pseudo-legal moves for the current side
    pseudoLegalMoves.insert(pseudoLegalMoves.end(), pawnMoves.begin(), pawnMoves.end());
    pseudoLegalMoves.insert(pseudoLegalMoves.end(), knightMoves.begin(), knightMoves.end());
    pseudoLegalMoves.insert(pseudoLegalMoves.end(), bishopMoves.begin(), bishopMoves.end());
    pseudoLegalMoves.insert(pseudoLegalMoves.end(), rookMoves.begin(), rookMoves.end());
    pseudoLegalMoves.insert(pseudoLegalMoves.end(), queenMoves.begin(), queenMoves.end());
    pseudoLegalMoves.insert(pseudoLegalMoves.end(), kingMoves.begin(), kingMoves.end());

    return pseudoLegalMoves;
}

// --- Move Generation Helper Functions ---
std::vector<Move> Board::generatePawnMoves() {
    std::vector<Move> moves;
    uint64_t pawns = (sideToMove == Color::WHITE) ? whitePawns : blackPawns;
    uint64_t allPieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing |
                         blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
    
    while (pawns) {
        uint64_t start_bit = pawns & -pawns;
        Square start = static_cast<Square>(start_bit);

        if (sideToMove == Color::WHITE) {
            // Single push
            uint64_t end_bit = start_bit << 8;
            bool isWhitePromoting = (end_bit & 0xFF00000000000000ULL) != 0;
            if (!(allPieces & end_bit)) {
                if (!isWhitePromoting) {
                    moves.push_back({start, static_cast<Square>(end_bit)});
                } else {
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::QUEEN});
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::KNIGHT});
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::ROOK});
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::BISHOP});
                }
            }
                
            // Double push
            end_bit = start_bit << 16;
            if ((start_bit & RANK_2) && !(allPieces & (start_bit << 8)) && !(allPieces & end_bit)) {
                moves.push_back({start, static_cast<Square>(end_bit)});
            }
                
            // Captures
            uint64_t capture1 = (start_bit << 7) & ~file_masks[7];
            if ((blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing) & capture1) {
                if (!isWhitePromoting) {
                    moves.push_back({start, static_cast<Square>(capture1)});
                } else {
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::QUEEN});
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::KNIGHT});
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::ROOK});
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::BISHOP});
                }
            }
                
            uint64_t capture2 = (start_bit << 9) & ~file_masks[0];
            if ((blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing) & capture2) {
                if (!isWhitePromoting) {
                    moves.push_back({start, static_cast<Square>(capture2)});
                } else {
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::QUEEN});
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::KNIGHT});
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::ROOK});
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::BISHOP});
                }
            }
               
            // En Passant
            if (enPassent && (capture1 == enPassent || capture2 == enPassent)) {
                 moves.push_back({start, static_cast<Square>(enPassent)});
            }
        } else { // Black
            // Single push
            uint64_t end_bit = start_bit >> 8;
            bool isBlackPromoting = (end_bit & 0x00000000000000FFULL) != 0;
            if (!(allPieces & end_bit)) {
                if (!isBlackPromoting) {
                    moves.push_back({start, static_cast<Square>(end_bit)});
                } else {
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::QUEEN});
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::KNIGHT});
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::ROOK});
                    moves.push_back({start, static_cast<Square>(end_bit), PieceType::BISHOP});
                }
            }
            // Double push
            end_bit = start_bit >> 16;
            if ((start_bit & RANK_7) && !(allPieces & (start_bit >> 8)) && !(allPieces & end_bit)) {
                moves.push_back({start, static_cast<Square>(end_bit)});
            }
                
            // Captures
            uint64_t capture1 = (start_bit >> 7) & ~file_masks[0];
            if ((whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing) & capture1) {
                if (!isBlackPromoting) {
                    moves.push_back({start, static_cast<Square>(capture1)});
                } else {
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::QUEEN});
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::KNIGHT});
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::ROOK});
                    moves.push_back({start, static_cast<Square>(capture1), PieceType::BISHOP});
                }
            }
            uint64_t capture2 = (start_bit >> 9) & ~file_masks[7];
            if ((whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing) & capture2) {
                if (!isBlackPromoting) {
                    moves.push_back({start, static_cast<Square>(capture1)});
                } else {
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::QUEEN});
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::KNIGHT});
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::ROOK});
                    moves.push_back({start, static_cast<Square>(capture2), PieceType::BISHOP});
                }
            }
                
            // En Passant
            if (enPassent && (capture1 == enPassent || capture2 == enPassent)) {
                moves.push_back({start, static_cast<Square>(enPassent)});
            }
        }
        pawns &= ~start_bit;
    }
    return moves;
}

std::vector<Move> Board::generateKnightMoves() {
    std::vector<Move> moves;
    uint64_t knights = (sideToMove == Color::WHITE) ? whiteKnights : blackKnights;
    uint64_t friendlyPieces = (sideToMove == Color::WHITE) ? (whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing) : (blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing);
    while (knights) {
        uint64_t start_bit = knights & -knights;
        uint64_t attacks = getKnightAttacks(start_bit);
        uint64_t valid_moves = attacks & ~friendlyPieces;
        while(valid_moves) {
            uint64_t end_bit = valid_moves & -valid_moves;
            moves.push_back({static_cast<Square>(start_bit), static_cast<Square>(end_bit)});
            valid_moves &= valid_moves - 1;
        }
        knights &= knights - 1;
    }
    return moves;
}

// Helper function to convert a PieceType enum to a string for logging
std::string pieceTypeToString(PieceType piece) {
    switch (piece) {
        case PieceType::QUEEN: return "Queen";
        case PieceType::ROOK: return "Rook";
        case PieceType::KNIGHT: return "Knight";
        case PieceType::BISHOP: return "Bishop";
    }
    return ""; // Should not be reached
}

std::vector<Move> Board::generateBishopMoves() {
    std::vector<Move> moves;
    uint64_t bishops = (sideToMove == Color::WHITE) ? whiteBishops : blackBishops;
    uint64_t allPieces = whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing |
                         blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing;
    uint64_t friendlyPieces = (sideToMove == Color::WHITE) ? (whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing) : (blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing);
    
    while (bishops) {
        uint64_t start_bit = bishops & -bishops;
        uint64_t attacks = getSlidingAttacks(start_bit, allPieces, false);
        uint64_t valid_moves = attacks & ~friendlyPieces;
        while(valid_moves) {
            uint64_t end_bit = valid_moves & -valid_moves;
            moves.push_back({static_cast<Square>(start_bit), static_cast<Square>(end_bit)});
            valid_moves &= valid_moves - 1;
        }
        bishops &= bishops - 1;
    }
    return moves;
}

std::vector<Move> Board::generateRookMoves() {
    std::vector<Move> moves;
    uint64_t rooks = (sideToMove == Color::WHITE) ? whiteRooks : blackRooks;
    uint64_t allPieces = whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing |
                         blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing;
    uint64_t friendlyPieces = (sideToMove == Color::WHITE) ? (whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing) : (blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing);
    
    while (rooks) {
        uint64_t start_bit = rooks & -rooks;
        uint64_t attacks = getSlidingAttacks(start_bit, allPieces, true);
        uint64_t valid_moves = attacks & ~friendlyPieces;
        while(valid_moves) {
            uint64_t end_bit = valid_moves & -valid_moves;
            moves.push_back({static_cast<Square>(start_bit), static_cast<Square>(end_bit)});
            valid_moves &= valid_moves - 1;
        }
        rooks &= rooks - 1;
    }
    return moves;
}

std::vector<Move> Board::generateQueenMoves() {
    std::vector<Move> moves;
    uint64_t queens = (sideToMove == Color::WHITE) ? whiteQueens : blackQueens;
    uint64_t allPieces = whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing |
                         blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing;
    uint64_t friendlyPieces = (sideToMove == Color::WHITE) ? (whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing) : (blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing);
    
    while (queens) {
        uint64_t start_bit = queens & -queens;
        uint64_t attacks = getSlidingAttacks(start_bit, allPieces, true) | getSlidingAttacks(start_bit, allPieces, false);
        uint64_t valid_moves = attacks & ~friendlyPieces;
        while(valid_moves) {
            uint64_t end_bit = valid_moves & -valid_moves;
            moves.push_back({static_cast<Square>(start_bit), static_cast<Square>(end_bit)});
            valid_moves &= valid_moves - 1;
        }
        queens &= queens - 1;
    }
    return moves;
}

std::vector<Move> Board::generateKingMoves() {
    std::vector<Move> moves;
    uint64_t king = (sideToMove == Color::WHITE) ? whiteKing : blackKing;
    uint64_t friendlyPieces = (sideToMove == Color::WHITE) ? (whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing) : (blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing);
    
    uint64_t attacks = getKingAttacks(king);
    uint64_t valid_moves = attacks & ~friendlyPieces;
    while(valid_moves) {
        uint64_t end_bit = valid_moves & -valid_moves;
        moves.push_back({static_cast<Square>(king), static_cast<Square>(end_bit)});
        valid_moves &= valid_moves - 1;
    }
    // Castling
    if ((sideToMove == Color::WHITE) && (whiteKing & static_cast<uint64_t>(Square::E1))) {
        if (whiteCastleKingside && !((whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing) & WHITE_KINGSIDE_CASTLE_PATH))
            moves.push_back({Square::E1, Square::G1});
        if (whiteCastleQueenside && !((whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|blackPawns|blackKnights|blackBishops|blackRooks|blackQueens|blackKing) & WHITE_QUEENSIDE_CASTLE_PATH))
            moves.push_back({Square::E1, Square::C1});
    } else if (blackKing & static_cast<uint64_t>(Square::E8)){
        if (blackCastleKingside && !((whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing|blackPawns|blackKnights|blackBishops|blackRooks|blackQueens) & BLACK_KINGSIDE_CASTLE_PATH))
            moves.push_back({Square::E8, Square::G8});
        if (blackCastleQueenside && !((whitePawns|whiteKnights|whiteBishops|whiteRooks|whiteQueens|whiteKing|blackPawns|blackKnights|blackBishops|blackRooks|blackQueens) & BLACK_QUEENSIDE_CASTLE_PATH))
            moves.push_back({Square::E8, Square::C8});
    }
    return moves;
}

/**
 * @brief Generates all legal moves for the current side to move.
 * @return A vector of valid Move structs.
 */
std::vector<Move> Board::generateLegalMoves() {
    std::vector<Move> pseudoLegalMoves = generatePseudoLegalMoves();

    std::vector<Move> legalMoves;
    for (const Move& move : pseudoLegalMoves) {
        if (this->isMoveLegal(move)) {
            legalMoves.push_back(move);
        } else {
            // std::cout << "pseudo legal move illegal: " << toAlgebraicNotation(move.start) << " -> " << toAlgebraicNotation(move.end)<< std::endl;
        }
    }
    
    return legalMoves;
}

bool Board::isMoveLegal(Move move) {
     Board tempBoard = *this;

    if (!tempBoard.applyMove(move)) {
        return false;
    }

    if (tempBoard.isKingInCheck(this->sideToMove)) {
        // std::cout << "move leaves king in check" << std::endl;
        return false;
    }

    return true;
}

// BoardBuilder class implementations
BoardBuilder::BoardBuilder(Square blackKingSquare, Square whiteKingSquare, Color sideToMove) : board(std::make_unique<Board>()) {
    board->setBlackKing(static_cast<uint64_t>(blackKingSquare));
    board->setWhiteKing(static_cast<uint64_t>(whiteKingSquare));
    board->setSideToMove(sideToMove);
}

BoardBuilder::BoardBuilder(const std::string& boardString, Color sideToMove) : board(std::make_unique<Board>()) {
    if (boardString.length() != 64) {
        // Handle invalid string length.
        return;
    }

    uint64_t whitePawns = 0ULL;
    uint64_t whiteKnights = 0ULL;
    uint64_t whiteBishops = 0ULL;
    uint64_t whiteRooks = 0ULL;
    uint64_t whiteQueens = 0ULL;
    uint64_t whiteKing = 0ULL;

    uint64_t blackPawns = 0ULL;
    uint64_t blackKnights = 0ULL;
    uint64_t blackBishops = 0ULL;
    uint64_t blackRooks = 0ULL;
    uint64_t blackQueens = 0ULL;
    uint64_t blackKing = 0ULL;

    for (int i = 0; i < 64; ++i) {
        // Convert string index to square index
        int rank = i / 8;
        int file = i % 8;
        int square_index = (7 - rank) * 8 + file;
        
        switch (boardString[i]) {
            case 'P': whitePawns |= (1ULL << square_index); break;
            case 'N': whiteKnights |= (1ULL << square_index); break;
            case 'B': whiteBishops |= (1ULL << square_index); break;
            case 'R': whiteRooks |= (1ULL << square_index); break;
            case 'Q': whiteQueens |= (1ULL << square_index); break;
            case 'K': whiteKing |= (1ULL << square_index); break;
            case 'p': blackPawns |= (1ULL << square_index); break;
            case 'n': blackKnights |= (1ULL << square_index); break;
            case 'b': blackBishops |= (1ULL << square_index); break;
            case 'r': blackRooks |= (1ULL << square_index); break;
            case 'q': blackQueens |= (1ULL << square_index); break;
            case 'k': blackKing |= (1ULL << square_index); break;
            case '.': break; // Empty square, do nothing
            default: return; // Invalid character
        }
    }

    board->setWhitePawns(whitePawns);
    board->setWhiteKnights(whiteKnights);
    board->setWhiteBishops(whiteBishops);
    board->setWhiteRooks(whiteRooks);
    board->setWhiteQueens(whiteQueens);
    board->setWhiteKing(whiteKing);
    board->setBlackPawns(blackPawns);
    board->setBlackKnights(blackKnights);
    board->setBlackBishops(blackBishops);
    board->setBlackRooks(blackRooks);
    board->setBlackQueens(blackQueens);
    board->setBlackKing(blackKing);
    board->setSideToMove(sideToMove);
}

BoardBuilder& BoardBuilder::setBlackBishops(uint64_t squares) { board->setBlackBishops(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackKnights(uint64_t squares) { board->setBlackKnights(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackPawns(uint64_t squares) { board->setBlackPawns(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackQueens(uint64_t squares) { board->setBlackQueens(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackRooks(uint64_t squares) { board->setBlackRooks(squares); return *this;}
BoardBuilder& BoardBuilder::setBlackKing(uint64_t square) { board->setBlackKing(square); return *this;}
BoardBuilder& BoardBuilder::setWhiteBishops(uint64_t squares) { board->setWhiteBishops(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteKnights(uint64_t squares) { board->setWhiteKnights(squares); return *this;}
BoardBuilder& BoardBuilder::setWhitePawns(uint64_t squares) { board->setWhitePawns(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteQueens(uint64_t squares) { board->setWhiteQueens(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteRooks(uint64_t squares) { board->setWhiteRooks(squares); return *this;}
BoardBuilder& BoardBuilder::setWhiteKing(uint64_t square) { board->setWhiteKing(square); return *this;}
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