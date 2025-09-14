#include "player.h"
#include "board.h"
#include <random>
#include <iostream>

bool RandomPlayer::makeMove(Board& board) {
    // 1. Generate all legal moves for the current side.
    std::vector<Move> legalMoves = board.generateLegalMoves();
    
    // 2. If no legal moves, return false.
    if (legalMoves.empty()) {
        return false;
    }
    
    // 3. Select a random move from the list.
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<size_t> distribution(0, legalMoves.size() - 1);
    size_t random_index = distribution(generator);
    Move chosenMove = legalMoves[random_index];

    std::string colorToMove = (board.getSideToMove() == Color::WHITE) ? "White" : "Black";

    std::cout << colorToMove << " made move (" << toAlgebraicNotation(chosenMove.start) << ", " << toAlgebraicNotation(chosenMove.end) << ")" << std::endl;

    // 4. Apply the chosen move to the board.
    return board.makeMove({chosenMove.start, chosenMove.end});    
}