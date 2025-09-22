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

    score += std::popcount(board.getWhitePawns()) * PAWN_VALUE;
    score += std::popcount(board.getWhiteKnights()) * KNIGHT_VALUE;
    score += std::popcount(board.getWhiteBishops()) * BISHOP_VALUE;
    score += std::popcount(board.getWhiteRooks()) * ROOK_VALUE;
    score += std::popcount(board.getWhiteQueens()) * QUEEN_VALUE;

    score -= std::popcount(board.getBlackPawns()) * PAWN_VALUE;
    score -= std::popcount(board.getBlackKnights()) * KNIGHT_VALUE;
    score -= std::popcount(board.getBlackBishops()) * BISHOP_VALUE;
    score -= std::popcount(board.getBlackRooks()) * ROOK_VALUE;
    score -= std::popcount(board.getBlackQueens()) * QUEEN_VALUE;

    // return (board.getSideToMove() == Color::WHITE) ? score : -score;
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

    if (board.getSideToMove() == Color::WHITE) {
        int maxEval = -std::numeric_limits<int>::max();
        for (const auto& move : legalMoves) {
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
        for (const auto& move : legalMoves) {
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

    for (const auto& move : sortedMoves) {
        std::shared_ptr<Board> tempBoard = std::make_shared<Board>(board);
        tempBoard->makeMove(move);
        int score = minimax(*tempBoard, searchDepth-1, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
        // std::cout << "score of move: " << toAlgebraicNotation(move.start) << " -> " << toAlgebraicNotation(move.end) << " is " << score << std::endl;

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