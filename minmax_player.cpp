#include "player.h"
#include <iostream>
#include <vector>
#include <algorithm>

MinMaxPlayer::MinMaxPlayer(int depth) : searchDepth(depth) {}

const int white_pawn_pst[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    -5, -5,  -10,-20,-20,-10, -5, -5,
    -5, -5,  -10,-20,-20,-10, -5, -5,
    0,  0,  0,  0,  0,  0,  0,  0
};

const int black_pawn_pst[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    -5, -5,  -10,-20,-20,-10, -5, -5,
    -5, -5,  -10,-20,-20,-10, -5, -5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5,  5, 10, 25, 25, 10,  5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0,  0,  0,  0,  0,  0,  0,  0
};

const int knight_pst[64] = {
    -30,-15,-10,-10,-10,-10,-15,-30,
    -15,-15,  0,  0,  0,  0,-15,-15,
    -10,  0, 10, 15, 15, 10,  0,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  0, 10, 15, 15, 10,  0,-10,
    -15,-15,  0,  0,  0,  0,-15,-15,
    -30,-15,-10,-10,-10,-10,-15,-30
};

const int white_bishop_pst[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int black_bishop_pst[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};

const int white_rook_pst[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,   0,  0,  5,  5,  0,  0,  0
};

const int black_rook_pst[64] = {
    0,   0,  0,  5,  5,  0,  0,  0,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    5, 10, 10, 10, 10, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0,
};

const int queen_pst[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
     0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

const int white_king_mid_pst[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20
};

const int black_king_mid_pst[64] = {
    20, 30, 10,  0,  0, 10, 30, 20,
    20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
};

int MinMaxPlayer::evaluate(Board& board) {
    int score = 0;

    // Piece values
    const int PAWN_VALUE = 1000;
    const int KNIGHT_VALUE = 3200;
    const int BISHOP_VALUE = 3300;
    const int ROOK_VALUE = 5000;
    const int QUEEN_VALUE = 9000;
    
    // --- Material Advantage ---
    int whiteMaterial = std::popcount(board.getWhitePawns()) * PAWN_VALUE +
                        std::popcount(board.getWhiteKnights()) * KNIGHT_VALUE +
                        std::popcount(board.getWhiteBishops()) * BISHOP_VALUE +
                        std::popcount(board.getWhiteRooks()) * ROOK_VALUE +
                        std::popcount(board.getWhiteQueens()) * QUEEN_VALUE;
                        
    int blackMaterial = std::popcount(board.getBlackPawns()) * PAWN_VALUE +
                        std::popcount(board.getBlackKnights()) * KNIGHT_VALUE +
                        std::popcount(board.getBlackBishops()) * BISHOP_VALUE +
                        std::popcount(board.getBlackRooks()) * ROOK_VALUE +
                        std::popcount(board.getBlackQueens()) * QUEEN_VALUE;

    score += whiteMaterial - blackMaterial;

    // --- Positional Advantage (PSTs) ---
    // White pieces
    uint64_t temp_pawns = board.getWhitePawns();
    while (temp_pawns) {
        int square_index = std::countr_zero(temp_pawns);
        score += white_pawn_pst[square_index];
        temp_pawns &= temp_pawns - 1;
    }
    uint64_t temp_knights = board.getWhiteKnights();
    while (temp_knights) {
        int square_index = std::countr_zero(temp_knights);
        score += knight_pst[square_index];
        temp_knights &= temp_knights - 1;
    }
    uint64_t temp_bishops = board.getWhiteBishops();
    while (temp_bishops) {
        int square_index = std::countr_zero(temp_bishops);
        score += white_bishop_pst[square_index];
        temp_bishops &= temp_bishops - 1;
    }
    uint64_t temp_rooks = board.getWhiteRooks();
    while (temp_rooks) {
        int square_index = std::countr_zero(temp_rooks);
        score += white_rook_pst[square_index];
        temp_rooks &= temp_rooks - 1;
    }
    uint64_t temp_queens = board.getWhiteQueens();
    while (temp_queens) {
        int square_index = std::countr_zero(temp_queens);
        score += queen_pst[square_index];
        temp_queens &= temp_queens - 1;
    }
    uint64_t temp_king = board.getWhiteKing();
    while (temp_king) {
        int square_index = std::countr_zero(temp_king);
        score += white_king_mid_pst[square_index]; // We use mid-game PST for now
        temp_king &= temp_king - 1;
    }
    
    // Black pieces
    temp_pawns = board.getBlackPawns();
    while (temp_pawns) {
        int square_index = std::countr_zero(temp_pawns);
        score -= black_pawn_pst[square_index];
        temp_pawns &= temp_pawns - 1;
    }
    temp_knights = board.getBlackKnights();
    while (temp_knights) {
        int square_index = std::countr_zero(temp_knights);
        score -= knight_pst[square_index];
        temp_knights &= temp_knights - 1;
    }
    temp_bishops = board.getBlackBishops();
    while (temp_bishops) {
        int square_index = std::countr_zero(temp_bishops);
        score -= black_bishop_pst[square_index];
        temp_bishops &= temp_bishops - 1;
    }
    temp_rooks = board.getBlackRooks();
    while (temp_rooks) {
        int square_index = std::countr_zero(temp_rooks);
        score -= black_rook_pst[square_index];
        temp_rooks &= temp_rooks - 1;
    }
    temp_queens = board.getBlackQueens();
    while (temp_queens) {
        int square_index = std::countr_zero(temp_queens);
        score -= queen_pst[square_index];
        temp_queens &= temp_queens - 1;
    }
    temp_king = board.getBlackKing();
    while (temp_king) {
        int square_index = std::countr_zero(temp_king);
        score -= black_king_mid_pst[square_index]; // We use mid-game PST for now
        temp_king &= temp_king - 1;
    }
    
    // --- Pawn Structure ---
    // for (int file = 0; file < 8; ++file) {
    //     int whitePawnsInFile = std::popcount(board.getWhitePawns() & file_masks[file]);
    //     int blackPawnsInFile = std::popcount(board.getBlackPawns() & file_masks[file]);
        
    //     if (whitePawnsInFile > 1) {
    //         score -= (whitePawnsInFile - 1) * 50; // Penalty for doubled pawns
    //     }
    //     if (blackPawnsInFile > 1) {
    //         score += (blackPawnsInFile - 1) * 50;
    //     }
    // }
    
    return score;
}

int MinMaxPlayer::minimax(Board& board, int depth, int alpha, int beta) {
    if (depth == 0) {
        int score = evaluate(board);
        // std::cout << "score of board at depth " << depth << " is " << score << std::endl; 
        // std::cout << board.toString() << std::endl;
        return score;
    }
    
    // Check for terminal nodes (checkmate or stalemate).
    std::vector<Move> legalMoves = board.generateLegalMoves();
    if (legalMoves.empty()) {
        if (board.isKingInCheck(board.getSideToMove())) {
            return (board.getSideToMove() == Color::WHITE) ? -std::numeric_limits<int>::max() : std::numeric_limits<int>::max();
        } else {
            return 0; // Stalemate
        }
    }

    std::vector<Move> captureMoves;
    std::vector<Move> nonCaptureMoves;
    for (const auto& move : legalMoves) {
        if (board.isCaptureMove(move)) {
            captureMoves.push_back(move);
        } else {
            nonCaptureMoves.push_back(move);
        }
    }
    std::vector<Move> sortedMoves;
    sortedMoves.insert(sortedMoves.end(), captureMoves.begin(), captureMoves.end());
    sortedMoves.insert(sortedMoves.end(), nonCaptureMoves.begin(), nonCaptureMoves.end());

    if (board.getSideToMove() == Color::WHITE) {
        int maxEval = -std::numeric_limits<int>::max();
        for (const auto& move : sortedMoves) {
            std::shared_ptr<Board> tempBoard = std::make_shared<Board>(board);
            // tempBoard->setVerbose(true);
            tempBoard->makeMove(move);
            int eval = minimax(*tempBoard, depth - 1, alpha, beta);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        if (depth >= 2) {
            // std::cout << "score of board at depth " << depth << " is " << maxEval << std::endl;
            // std::cout << board.toString() << std::endl;
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : sortedMoves) {
            std::shared_ptr<Board> tempBoard = std::make_shared<Board>(board);
            // tempBoard->setVerbose(true);
            tempBoard->makeMove(move);
            int eval = minimax(*tempBoard, depth - 1, alpha, beta);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        if (depth >= 2) {
            // std::cout << "score of board at depth " << depth << " is " << minEval << std::endl;
            // std::cout << board.toString() << std::endl;
        }
        return minEval;
    }
}

bool MinMaxPlayer::makeMove(Board& board) {
    std::vector<Move> legalMoves = board.generateLegalMoves();
    if (legalMoves.empty()) {
        return false;
    }

    std::vector<Move> captureMoves;
    std::vector<Move> nonCaptureMoves;
    for (const auto& move : legalMoves) {
        if (board.isCaptureMove(move)) {
            captureMoves.push_back(move);
        } else {
            nonCaptureMoves.push_back(move);
        }
    }
    std::vector<Move> sortedMoves;
    sortedMoves.insert(sortedMoves.end(), captureMoves.begin(), captureMoves.end());
    sortedMoves.insert(sortedMoves.end(), nonCaptureMoves.begin(), nonCaptureMoves.end());

    int bestScore = (board.getSideToMove() == Color::WHITE) ? -std::numeric_limits<int>::max() : std::numeric_limits<int>::max();
    Move bestMove = sortedMoves[0];

    // std::cout << "eval of current position is: " << evaluate(board) << std::endl;
    for (const auto& move : sortedMoves) {
        std::shared_ptr<Board> tempBoard = std::make_shared<Board>(board);
        tempBoard->makeMove(move);
        int score = minimax(*tempBoard, searchDepth-1, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
        // std::cout << "score of move: " << toAlgebraicNotation(move.start) << " -> " << toAlgebraicNotation(move.end) << " is " << score << std::endl;
        // std::cout << "  eval of resulting position is: " << evaluate(*tempBoard) << std::endl;

        if (board.getSideToMove() == Color::WHITE) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    }

    std::string colorToMove = (board.getSideToMove() == Color::WHITE) ? "White" : "Black";
    std::cout << colorToMove << " made move (" << toAlgebraicNotation(bestMove.start) << ", " << toAlgebraicNotation(bestMove.end) << ")" << std::endl;

    return board.makeMove(bestMove);
}