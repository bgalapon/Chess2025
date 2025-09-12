#include "player.h"
#include <random>

// RandomPlayer class that implements the Player interface
class RandomPlayer : public Player {
public:
    bool makeMove(Board& board) override;
};

bool RandomPlayer::makeMove(Board& board) {
    // 1. Generate all legal moves for the current side.
    std::vector<Move> legal_moves = board.generateLegalMoves();
    
    // 2. If no legal moves, return false.
    if (legal_moves.empty()) {
        return false;
    }
    
    // 3. Select a random move from the list.
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<size_t> distribution(0, legal_moves.size() - 1);
    size_t random_index = distribution(generator);
    Move chosen_move = legal_moves[random_index];

    // 4. Apply the chosen move to the board.
    board.move(chosen_move.start, chosen_move.end);
    
    return true;
}