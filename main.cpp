#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <cmath> // For std::abs
#include <vector> // For bitboard to index conversion
#include "board.h"

int main() {
    std::unique_ptr<Board> board = StandardBoard();
    std::cout << board->toString() << std::endl;

    board->move(Square::E2, Square::E4);
    std::cout << board->toString() << std::endl;

    board->move(Square::D7, Square::D5);
    std::cout << board->toString() << std::endl;

    board->move(Square::E4, Square::D5);
    std::cout << board->toString() << std::endl;

    board->move(Square::C7, Square::C5);
    std::cout << board->toString() << std::endl;

    board->move(Square::D5, Square::C6);
    std::cout << board->toString() << std::endl;

    return 0;
}