#include <iostream>
#include <memory>
#include <string>

enum class Color {
    WHITE,
    BLACK
};

// Bitboard masks for ranks for efficient checking
const uint64_t RANK_2 = 0x000000000000FF00ULL;
const uint64_t RANK_4 = 0x0000000000FF0000ULL;
const uint64_t RANK_5 = 0x0000FF0000000000ULL;
const uint64_t RANK_7 = 0x00FF000000000000ULL;

const uint64_t WHITE_KINGSIDE_CASTLE_PATH = 96ULL;
const uint64_t WHITE_QUEENSIDE_CASTLE_PATH = 14ULL;
const uint64_t BLACK_KINGSIDE_CASTLE_PATH = 6917529027641081856ULL;
const uint64_t BLACK_QUEENSIDE_CASTLE_PATH = 1008806316530991104ULL;

// Helper function to get the index (0-63) from a single-bit bitboard
inline uint8_t getSquareIndex(uint64_t bitboard) {
    if (bitboard == 0) return 64; // Sentinel value for invalid square
    return __builtin_ctzll(bitboard);
}

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

    bool isPathClear(uint64_t start_bit, uint64_t end_bit, uint64_t allPieces) {
        uint8_t start_index = getSquareIndex(start_bit);
        uint8_t end_index = getSquareIndex(end_bit);
        
        int start_rank = start_index >> 3;
        int start_file = start_index & 7;
        int end_rank = end_index >> 3;
        int end_file = end_index & 7;

        // Check for horizontal move
        if (start_rank == end_rank) {
            uint64_t pathMask;
            if (start_bit < end_bit) {
                pathMask = (end_bit - 1) & ~((start_bit << 1) - 1);
            } else {
                pathMask = (start_bit - 1) & ~((end_bit << 1) - 1);
            }
            return (pathMask & allPieces) == 0;
        }

        // Check for vertical move
        if (start_file == end_file) {
            uint64_t pathMask = 0;
            int step = (start_index < end_index) ? 8 : -8;
            for (int i = start_index + step; i != end_index; i += step) {
                pathMask |= (1ULL << i);
            }
            return (pathMask & allPieces) == 0;
        }

        // Check for diagonal move
        if (std::abs(start_rank - end_rank) == std::abs(start_file - end_file)) {
            int step_rank = (start_rank < end_rank) ? 8 : -8;
            int step_file = (start_file < end_file) ? 1 : -1;
            int step = step_rank + step_file;
            for (int i = start_index + step; i != end_index; i += step) {
                if ((allPieces & (1ULL << i)) != 0) {
                    return false;
                }
            }
            return true;
        }
        
        return false;
    }

    // move performs a move and returns a bool indicating if the move was successful
    bool move(Square start, Square end) {
        uint64_t start_bit = static_cast<uint64_t>(start);
        uint64_t end_bit = static_cast<uint64_t>(end);
        
        bool isPawnDoubleStep = false;
        bool isCastlingMove = false;

        // Get bitboards for friendly and enemy pieces for efficiency
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
            // White King-side castling
            if ((whiteKing & start_bit) && start == Square::E1 && end == Square::G1 && this->whiteCastleKingside) {
                // Check if squares between king and rook are empty
                if (!((friendlyPieces | enemyPieces) & WHITE_KINGSIDE_CASTLE_PATH)) {
                    // Move the king and rook
                    whiteKing = static_cast<uint64_t>(Square::G1);
                    whiteRooks &= ~static_cast<uint64_t>(Square::H1);
                    whiteRooks |= static_cast<uint64_t>(Square::F1);
                    isCastlingMove = true;
                }
            }
            // White Queen-side castling
            else if ((whiteKing & start_bit) && start == Square::E1 && end == Square::C1 && this->whiteCastleQueenside) {
                // Check if squares between king and rook are empty
                if (!((friendlyPieces | enemyPieces) & WHITE_QUEENSIDE_CASTLE_PATH)) {
                    // Move the king and rook
                    whiteKing = static_cast<uint64_t>(Square::C1);
                    whiteRooks &= ~static_cast<uint64_t>(Square::A1);
                    whiteRooks |= static_cast<uint64_t>(Square::D1);
                    isCastlingMove = true;
                }
            }
        } else { // sideToMove == Color::BLACK
            // Black King-side castling
            if ((blackKing & start_bit) && start == Square::E8 && end == Square::G8 && this->blackCastleKingside) {
                // Check if squares between king and rook are empty
                if (!((friendlyPieces | enemyPieces) & BLACK_KINGSIDE_CASTLE_PATH)) {
                    // Move the king and rook
                    blackKing = static_cast<uint64_t>(Square::G8);
                    blackRooks &= ~static_cast<uint64_t>(Square::H8);
                    blackRooks |= static_cast<uint64_t>(Square::F8);
                    isCastlingMove = true;
                }
            }
            // Black Queen-side castling
            else if ((blackKing & start_bit) && start == Square::E8 && end == Square::C8 && this->blackCastleQueenside) {
                // Check if squares between king and rook are empty
                if (!((friendlyPieces | enemyPieces) & BLACK_QUEENSIDE_CASTLE_PATH)) {
                    // Move the king and rook
                    blackKing = static_cast<uint64_t>(Square::C8);
                    blackRooks &= ~static_cast<uint64_t>(Square::A8);
                    blackRooks |= static_cast<uint64_t>(Square::D8);
                    isCastlingMove = true;
                }
            }
        }

        // Exit early if a castling move was successfully executed
        if (isCastlingMove) {
            this->whiteCastleKingside = false;
            this->whiteCastleQueenside = false;
            this->blackCastleKingside = false;
            this->blackCastleQueenside = false;
            this->enPassent = 0;
            if (this->sideToMove == Color::WHITE) {
                this->sideToMove = Color::BLACK;
            } else {
                this->sideToMove = Color::WHITE;
            }
            return true;
        }

        // --- Standard Move Logic (only if not a castling move) ---

        // Check if a piece of the current side exists at the start square
        if ((friendlyPieces & start_bit) == 0) {
            std::cerr << "Error: No piece of the current side at the start square." << std::endl;
            return false;
        }

        // Check if the destination square is occupied by a friendly piece
        if ((friendlyPieces & end_bit) != 0) {
            std::cerr << "Error: Cannot capture a friendly piece." << std::endl;
            return false;
        }

        // Get start and end square indices for validation
        uint8_t start_rank = getSquareIndex(start_bit) >> 3;
        uint8_t start_file = getSquareIndex(start_bit) & 7;
        uint8_t end_rank = getSquareIndex(end_bit) >> 3;
        uint8_t end_file = getSquareIndex(end_bit) & 7;
        
        // Piece-specific move validation
        if ((whitePawns & start_bit) || (blackPawns & start_bit)) {
            if (this->sideToMove == Color::WHITE) {
                if (end_bit == (start_bit << 8) && (allPieces & end_bit) == 0) {
                    // Valid single step forward
                } else if ((start_bit & RANK_2) && end_bit == (start_bit << 16) && (allPieces & (end_bit | (start_bit << 8))) == 0) {
                    isPawnDoubleStep = true;
                } else if ((end_bit == (start_bit << 7) || end_bit == (start_bit << 9)) && (enemyPieces & end_bit)) {
                    // Valid capture
                } else if (end_bit == this->enPassent) {
                    // Valid en passant
                } else {
                    std::cerr << "Error: Invalid white pawn move." << std::endl;
                    return false;
                }
            } else { // Black pawn
                if (end_bit == (start_bit >> 8) && (allPieces & end_bit) == 0) {
                    // Valid single step forward
                } else if ((start_bit & RANK_7) && end_bit == (start_bit >> 16) && (allPieces & (end_bit | (start_bit >> 8))) == 0) {
                    isPawnDoubleStep = true;
                } else if ((end_bit == (start_bit >> 7) || end_bit == (start_bit >> 9)) && (enemyPieces & end_bit)) {
                    // Valid capture
                } else if (end_bit == this->enPassent) {
                    // Valid en passant
                } else {
                    std::cerr << "Error: Invalid black pawn move." << std::endl;
                    return false;
                }
            }
        }
        else if ((whiteKnights & start_bit) || (blackKnights & start_bit)) {
            int rank_diff = std::abs(start_rank - end_rank);
            int file_diff = std::abs(start_file - end_file);
            if (!((rank_diff == 2 && file_diff == 1) || (rank_diff == 1 && file_diff == 2))) {
                std::cerr << "Error: Invalid knight move. Knights move in an L-shape." << std::endl;
                return false;
            }
        }
        else if ((whiteBishops & start_bit) || (blackBishops & start_bit)) {
            if (std::abs(start_rank - end_rank) != std::abs(start_file - end_file) || !isPathClear(start_bit, end_bit, allPieces)) {
                std::cerr << "Error: Invalid bishop move. Path is not clear or move is not diagonal." << std::endl;
                return false;
            }
        }
        else if ((whiteRooks & start_bit) || (blackRooks & start_bit)) {
            bool isStraight = (start_rank == end_rank) || (start_file == end_file);
            if (!isStraight || !isPathClear(start_bit, end_bit, allPieces)) {
                std::cerr << "Error: Invalid rook move. Path is not clear or move is not straight." << std::endl;
                return false;
            }
        }
        else if ((whiteQueens & start_bit) || (blackQueens & start_bit)) {
            bool isHorizontal = start_rank == end_rank;
            bool isVertical = start_file == end_file;
            bool isDiagonal = std::abs(start_rank - end_rank) == std::abs(start_file - end_file);
            if ((!isHorizontal && !isVertical && !isDiagonal) || !isPathClear(start_bit, end_bit, allPieces)) {
                std::cerr << "Error: Invalid queen move. Path is not clear or move is not straight/diagonal." << std::endl;
                return false;
            }
        }
        else if ((whiteKing & start_bit) || (blackKing & start_bit)) {
            int rank_diff = std::abs(start_rank - end_rank);
            int file_diff = std::abs(start_file - end_file);
            if (rank_diff > 1 || file_diff > 1) {
                std::cerr << "Error: Invalid king move. Kings move one square at a time." << std::endl;
                return false;
            }
        } else {
            // No valid piece found at the start square, though this should be caught by the friendlyPieces check
            std::cerr << "Error: No valid piece found at the start square." << std::endl;
            return false;
        }

        // --- Execute the Move and Capture ---
        
        // Handle captures by clearing the destination bit from all enemy pieces
        if (this->sideToMove == Color::WHITE) {
            // Special case for en passant capture
            if ((whitePawns & start_bit) && (end_bit == this->enPassent)) {
                uint64_t capturedPawnSquare = end_bit >> 8;
                blackPawns &= ~capturedPawnSquare;
            } else {
                 // Regular capture logic
                blackPawns &= ~end_bit;
                blackKnights &= ~end_bit;
                blackBishops &= ~end_bit;
                blackRooks &= ~end_bit;
                blackQueens &= ~end_bit;
                blackKing &= ~end_bit;
            }
        } else { // sideToMove == Color::BLACK
            // Special case for en passant capture
            if ((blackPawns & start_bit) && (end_bit == this->enPassent)) {
                uint64_t capturedPawnSquare = end_bit << 8;
                whitePawns &= ~capturedPawnSquare;
            } else {
                // Regular capture logic
                whitePawns &= ~end_bit;
                whiteKnights &= ~end_bit;
                whiteBishops &= ~end_bit;
                whiteRooks &= ~end_bit;
                whiteQueens &= ~end_bit;
                whiteKing &= ~end_bit;
            }
        }
        
        // Move the piece from the start square to the end square
        if (this->sideToMove == Color::WHITE) {
            if (whitePawns & start_bit) {
                whitePawns &= ~start_bit;
                whitePawns |= end_bit;
            } else if (whiteKnights & start_bit) {
                whiteKnights &= ~start_bit;
                whiteKnights |= end_bit;
            } else if (whiteBishops & start_bit) {
                whiteBishops &= ~start_bit;
                whiteBishops |= end_bit;
            } else if (whiteRooks & start_bit) {
                whiteRooks &= ~start_bit;
                whiteRooks |= end_bit;
                // Revoke castling rights if a rook moves from its starting square
                if (start == Square::A1) whiteCastleQueenside = false;
                if (start == Square::H1) whiteCastleKingside = false;
            } else if (whiteQueens & start_bit) {
                whiteQueens &= ~start_bit;
                whiteQueens |= end_bit;
            } else if (whiteKing & start_bit) {
                whiteKing &= ~start_bit;
                whiteKing |= end_bit;
                // Revoke all castling rights if the king moves
                whiteCastleKingside = false;
                whiteCastleQueenside = false;
            }
        } else { // sideToMove == Color::BLACK
            if (blackPawns & start_bit) {
                blackPawns &= ~start_bit;
                blackPawns |= end_bit;
            } else if (blackKnights & start_bit) {
                blackKnights &= ~start_bit;
                blackKnights |= end_bit;
            } else if (blackBishops & start_bit) {
                blackBishops &= ~start_bit;
                blackBishops |= end_bit;
            } else if (blackRooks & start_bit) {
                blackRooks &= ~start_bit;
                blackRooks |= end_bit;
                // Revoke castling rights if a rook moves from its starting square
                if (start == Square::A8) blackCastleQueenside = false;
                if (start == Square::H8) blackCastleKingside = false;
            } else if (blackQueens & start_bit) {
                blackQueens &= ~start_bit;
                blackQueens |= end_bit;
            } else if (blackKing & start_bit) {
                blackKing &= ~start_bit;
                blackKing |= end_bit;
                // Revoke all castling rights if the king moves
                blackCastleKingside = false;
                blackCastleQueenside = false;
            }
        }

        // Handle en passant flag reset
        if (!isPawnDoubleStep) {
            this->enPassent = 0;
        }

        // Toggle side to move
        if (this->sideToMove == Color::WHITE) {
            this->sideToMove = Color::BLACK;
        } else {
            this->sideToMove = Color::WHITE;
        }
        
        return true;
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