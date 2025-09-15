#include "board.h"

// Player interface (abstract class)
class Player {
public:
    virtual ~Player() {}
    virtual bool makeMove(Board& board) = 0;
};

// RandomPlayer class that implements the Player interface
class RandomPlayer : public Player {
public:
    bool makeMove(Board& board) override;
};

// HumanPlayer class that implements the Player interface
class HumanPlayer : public Player {
public:
    bool makeMove(Board& board) override;
};

// MinMaxPlayer class that implements the Player interface
class MinMaxPlayer : public Player {
public:
    MinMaxPlayer(int depth);
    bool makeMove(Board& board) override;
private:
    int searchDepth;
    int evaluate(Board& board);
    int minimax(Board& board, int depth, int alpha, int beta);
};