#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <cmath> // For std::abs
#include <vector> // For bitboard to index conversion
#include "board.h"
#include "player.h"

int main() {
    std::unique_ptr<Board> board = StandardBoard();

    MinMaxPlayer whitePlayer(6);
    HumanPlayer blackPlayer;

    std::cout << "Board:" << std::endl;
    std::cout << board->toString() << std::endl;
    int turns = 0;
    while (true) {
        std::cout << "--------------------" << std::endl;
        if (board->getSideToMove() == Color::WHITE) {
            std::cout << "White's Turn" << std::endl;
            if (!whitePlayer.makeMove(*board)) {
                if (board->isKingInCheck(Color::WHITE)) {
                    std::cout << "Checkmate! Black wins." << std::endl;
                } else {
                    std::cout << "Stalemate! Game is a draw." << std::endl;
                }
                break;
            }
        } else {
            std::cout << "Black's Turn" << std::endl;
            if (!blackPlayer.makeMove(*board)) {
                if (board->isKingInCheck(Color::BLACK)) {
                    std::cout << "Checkmate! White wins." << std::endl;
                } else {
                    std::cout << "Stalemate! Game is a draw." << std::endl;
                }
                break;
            }
        }

        turns++;
        // if (turns == 3) {
        //     break;
        // }

        std::cout << "Board:" << std::endl;
        std::cout << board->toString() << std::endl;

        if (board->isInsufficientMaterial()) {
            std::cout << "Insufficient Material! Game is a draw." << std::endl;
            break;
        }
    }

    return 0;
}