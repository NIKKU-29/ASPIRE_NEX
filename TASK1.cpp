#include <iostream>
#include <vector>
#include <climits>

const int BOARD_SIZE = 3;
const char EMPTY = ' ';
const char HUMAN = 'X';
const char AI = 'O';

enum class Winner {
    Human,
    AI,
    Tie,
    None
};

class TicTacToe {
public:
    TicTacToe() : board(BOARD_SIZE * BOARD_SIZE, EMPTY) {}

    void displayBoard() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            std::cout << board[i * BOARD_SIZE + j] ;
             if(j<2) std::cout<<" | ";
             
            std::cout<<"";
            
        }
        std::cout << std::endl;
    }
}

    bool makeMove(char player, int move) {
        if (move < 0 || move >= BOARD_SIZE * BOARD_SIZE || board[move] != EMPTY) {
            return false;
        }
        board[move] = player;
        return true;
    }

    Winner checkWinner() {
        // Check rows
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (board[i * BOARD_SIZE] == board[i * BOARD_SIZE + 1] &&
                board[i * BOARD_SIZE] == board[i * BOARD_SIZE + 2] &&
                board[i * BOARD_SIZE] != EMPTY) {
                return board[i * BOARD_SIZE] == HUMAN ? Winner::Human : Winner::AI;
            }
        }

        // Check columns
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[j] == board[j + BOARD_SIZE] && board[j] == board[j + 2 * BOARD_SIZE] &&
                board[j] != EMPTY) {
                return board[j] == HUMAN ? Winner::Human : Winner::AI;
            }
        }

        // Check diagonals
        if (board[0] == board[4] && board[0] == board[8] && board[0] != EMPTY) {
            return board[0] == HUMAN ? Winner::Human : Winner::AI;
        }
        if (board[2] == board[4] && board[2] == board[6] && board[2] != EMPTY) {
            return board[2] == HUMAN ? Winner::Human : Winner::AI;
        }

        // Check for tie
        bool isFull = true;
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
            if (board[i] == EMPTY) {
                isFull = false;
                break;
            }
        }
        return isFull ? Winner::Tie : Winner::None;
    }

    int minimax(int depth, bool isMaximizingPlayer) {
        Winner winner = checkWinner();
        if (winner == Winner::Human) {
            return -10;
        } else if (winner == Winner::AI) {
            return 10;
        } else if (winner == Winner::Tie) {
            return 0;
        }

        if (depth == 0) {
            return evaluateBoard();
        }

        int bestScore = isMaximizingPlayer ? INT_MIN : INT_MAX;
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
            if (board[i] == EMPTY) {
                board[i] = isMaximizingPlayer ? AI : HUMAN;
                int score = minimax(depth - 1, !isMaximizingPlayer);
                board[i] = EMPTY; // Backtrack

                if (isMaximizingPlayer) {
                    bestScore = std::max(bestScore, score);
                } else {
                    bestScore = std::min(bestScore, score);
                }
            }
        }

        return bestScore;
    }

    int evaluateBoard() {
        int score = 0;

        // Check rows and columns for potential threats
        for (int i = 0; i < BOARD_SIZE; ++i) {
            int humanCount = 0, aiCount = 0;
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i * BOARD_SIZE + j] == HUMAN) {
                    humanCount++;
                } else if (board[i * BOARD_SIZE + j] == AI) {
                    aiCount++;
                }
            }
            score += (2 * humanCount) - (3 * aiCount); // Prioritize blocking human wins
        }

        for (int j = 0; j < BOARD_SIZE; ++j) {
            int humanCount = 0, aiCount = 0;
            for (int i = 0; i < BOARD_SIZE; ++i) {
                if (board[i * BOARD_SIZE + j] == HUMAN) {
                    humanCount++;
                } else if (board[i * BOARD_SIZE + j] == AI) {
                    aiCount++;
                }
            }
            score += (2 * humanCount) - (3 * aiCount);
        }

        // Check diagonals for potential threats
        int humanCount = 0, aiCount = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (board[i * BOARD_SIZE + i] == HUMAN) {
                humanCount++;
            } else if (board[i * BOARD_SIZE + i] == AI) {
                aiCount++;
            }
        }
        score += (2 * humanCount) - (3 * aiCount);

        humanCount = 0;
        aiCount = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (board[i * BOARD_SIZE + (BOARD_SIZE - 1 - i)] == HUMAN) {
                humanCount++;
            } else if (board[i * BOARD_SIZE + (BOARD_SIZE - 1 - i)] == AI) {
                aiCount++;
            }
        }
        score += (2 * humanCount) - (3 * aiCount);

        return score;
    }

    int findBestMove() {
        int bestScore = INT_MIN;
        int bestMove = -1;
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
            if (board[i] == EMPTY) {
                board[i] = AI;
                int score = minimax(2, false); // Depth of 2 for AI (can be adjusted)
                board[i] = EMPTY; // Backtrack

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = i;
                }
            }
        }
        return bestMove;
    }

    void playGame() {
    displayBoard();
    Winner winner = Winner::None;
    
    while (winner == Winner::None) {
        // Human's move
        int humanMove;
        do {
            std::cout << "Enter your move (1-9): ";
            std::cin >> humanMove;
            humanMove--; // Convert from user input (1-9) to array index (0-8)
        } while (!makeMove(HUMAN, humanMove));

        displayBoard();
        winner = checkWinner();
        if (winner != Winner::None) break; // Exit if there's a winner

        // AI's move
        int bestMove = findBestMove();
        makeMove(AI, bestMove);
        std::cout << "AI move: " << bestMove + 1 << std::endl; // Convert back to user output (1-9)
        displayBoard();
        winner = checkWinner();
    }

    // Declare the winner
    if (winner == Winner::Human) {
        std::cout << "You Win!" << std::endl;
    } else if (winner == Winner::AI) {
        std::cout << "AI Wins!" << std::endl;
    } else {
        std::cout << "Tie!" << std::endl;
    }
}


private:
    std::vector<char> board;
};

int main() {
    TicTacToe game;
    game.playGame();
    return 0;
}