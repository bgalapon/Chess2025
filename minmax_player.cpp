#include "player.h"
#include <iostream>
#include <vector>
#include <algorithm>

MinMaxPlayer::MinMaxPlayer(int depth) : searchDepth(depth) {}

int MinMaxPlayer::evaluate(Board& board) {
    int score = 0;

    // Piece values
    const int PAWN_VALUE = 100;
    const int KNIGHT_VALUE = 320;
    const int BISHOP_VALUE = 330;
    const int ROOK_VALUE = 500;
    const int QUEEN_VALUE = 900;
    const int KING_VALUE = 20000; // High value to prioritize the king

    score += std::popcount(board.getWhitePawns()) * PAWN_VALUE;
    score += std::popcount(board.getWhiteKnights()) * KNIGHT_VALUE;
    score += std::popcount(board.getWhiteBishops()) * BISHOP_VALUE;
    score += std::popcount(board.getWhiteRooks()) * ROOK_VALUE;
    score += std::popcount(board.getWhiteQueens()) * QUEEN_VALUE;
    score += std::popcount(board.getWhiteKing()) * KING_VALUE;

    score -= std::popcount(board.getBlackPawns()) * PAWN_VALUE;
    score -= std::popcount(board.getBlackKnights()) * KNIGHT_VALUE;
    score -= std::popcount(board.getBlackBishops()) * BISHOP_VALUE;
    score -= std::popcount(board.getBlackRooks()) * ROOK_VALUE;
    score -= std::popcount(board.getBlackQueens()) * QUEEN_VALUE;
    score -= std::popcount(board.getBlackKing()) * KING_VALUE;

    return (board.getSideToMove() == Color::WHITE) ? score : -score;
}

int MinMaxPlayer::minimax(Board& board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return evaluate(board);
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

    if (board.getSideToMove() == Color::WHITE) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : legalMoves) {
            std::shared_ptr<Board> tempBoard = std::make_shared<Board>(board);
            tempBoard->makeMove(move);
            int eval = minimax(*tempBoard, depth - 1, alpha, beta);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : legalMoves) {
            std::shared_ptr<Board> tempBoard = std::make_shared<Board>(board);
            tempBoard->makeMove(move);
            int eval = minimax(*tempBoard, depth - 1, alpha, beta);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

bool MinMaxPlayer::makeMove(Board& board) {
    std::vector<Move> legalMoves = board.generateLegalMoves();
    if (legalMoves.empty()) {
        return false;
    }

    int bestScore = (board.getSideToMove() == Color::WHITE) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    Move bestMove = legalMoves[0];

    for (const auto& move : legalMoves) {
        std::shared_ptr<Board> tempBoard = std::make_shared<Board>(board);
        tempBoard->makeMove(move);
        int score = minimax(*tempBoard, searchDepth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

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