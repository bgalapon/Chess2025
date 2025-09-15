#include "player.h"
#include "board.h"
#include <iostream>

Square stringToSquare(const std::string& s) {
    std::cout << "Input: " << s << std::endl;
    if (s.length() != 2) return static_cast<Square>(-1);
    int file = std::tolower(s[0]) - 'a';
    int rank = s[1] - '1';
    if (file < 0 || file > 7 || rank < 0 || rank > 7) return static_cast<Square>(-1);
    return static_cast<Square>(1ULL << (rank * 8 + file));
}

// A helper function to convert a character to a PieceType for promotions.
PieceType charToPieceType(char c) {
    switch (c) {
        case 'Q': return PieceType::QUEEN;
        case 'R': return PieceType::ROOK;
        case 'B': return PieceType::BISHOP;
        case 'N': return PieceType::KNIGHT;
        default: return PieceType::QUEEN; // Default to Queen if invalid input
    }
}

bool HumanPlayer::makeMove(Board& board) {
    std::string move_input;
    Move move;
    
    // Loop until a valid move is entered and executed.
    while (true) {
        std::cout << "Enter your move (e.g., 'E2E4' or 'H7H8Q' for promotion): ";
        std::cin >> move_input;

        if (move_input.length() < 4) {
            std::cout << "Invalid input format. Please try again." << std::endl;
            continue;
        }

        Square start = stringToSquare(move_input.substr(0, 2));
        Square end = stringToSquare(move_input.substr(2, 2));

        std::cout << "Start square: " << toAlgebraicNotation(start) << std::endl;;
        std::cout << "End square: " << toAlgebraicNotation(end) << std::endl;;

        if (start == static_cast<Square>(-1) || end == static_cast<Square>(-1)) {
            std::cout << "Invalid square notation. Please try again." << std::endl;
            continue;
        }

        // Handle promotions
        if (move_input.length() == 5) {
            PieceType promotion = charToPieceType(move_input[4]);
            move = {start, end, promotion};
        } else {
            // Default to queen for non-promotion moves, as it's not used by board.move()
            move = {start, end, PieceType::QUEEN};
        }

        // Placeholder for move validation.
        // Once Board::generateLegalMoves is fully implemented, you can check
        // if the move is in the list of legal moves here.
        // For now, we will simply try to make the move on the board.
        if (board.makeMove(move)) {
            return true;
        } else {
            std::cout << "Illegal move. Please try again." << std::endl;
        }
    }
    return false; // Should not be reached in this loop
}