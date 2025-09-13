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
