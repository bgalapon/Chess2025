#include "board.h"

// Player interface (abstract class)
class Player {
public:
    virtual ~Player() {}
    virtual bool makeMove(Board& board) = 0;
};
