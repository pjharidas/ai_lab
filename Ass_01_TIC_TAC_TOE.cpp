#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int BOARD_SIZE = 3;
const char EMPTY = ' ';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

// Function to print the Tic Tac Toe board
void printBoard(vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j];
            if (j < BOARD_SIZE - 1) cout << " | ";
        }
        cout << endl;
        if (i < BOARD_SIZE - 1) {
            cout << "---------" << endl;
        }
    }
    cout << endl;
}

// Function to check if the game is over
bool isGameOver(vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

// Function to check if a player has won
int evaluate(vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == PLAYER_X) return 1;
            else if (board[i][0] == PLAYER_O) return -1;
        }
    }

    for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
            if (board[0][j] == PLAYER_X) return 1;
            else if (board[0][j] == PLAYER_O) return -1;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X) return 1;
        else if (board[0][0] == PLAYER_O) return -1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X) return 1;
        else if (board[0][2] == PLAYER_O) return -1;
    }

    return 0;  // No winner yet
}

// Minimax algorithm with alpha-beta pruning and variable depth
int minimax(vector<vector<char>>& board, int depth, int maxDepth, bool isMaximizing) {
    int score = evaluate(board);

    if (score == 1) return 1;
    if (score == -1) return -1;
    if (isGameOver(board) || depth == maxDepth) return 0;

    if (isMaximizing) {
        int maxEval = numeric_limits<int>::min();
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    int eval = minimax(board, depth + 1, maxDepth, false);
                    board[i][j] = EMPTY;
                    maxEval = max(maxEval, eval);
                }
            }
        }
        return maxEval;
    } else {
        int minEval = numeric_limits<int>::max();
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    int eval = minimax(board, depth + 1, maxDepth, true);
                    board[i][j] = EMPTY;
                    minEval = min(minEval, eval);
                }
            }
        }
        return minEval;
    }
}

// Find the best move for AI with variable depth
pair<int, int> findBestMove(vector<vector<char>>& board, int maxDepth) {
    int bestVal = numeric_limits<int>::min();
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_X;
                int moveVal = minimax(board, 0, maxDepth, false);
                board[i][j] = EMPTY;

                cout << "Heuristic value for (" << i << ", " << j << ") = " << moveVal << endl;

                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }

    cout << "AI chooses to play at (" << bestMove.first << ", " << bestMove.second << ")" << endl;

    return bestMove;
}

int main() {
    vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, EMPTY));
    int choice;
    char currentPlayer;
    int maxDepth = 1; // Default to 1-ply depth

    cout << "Choose your turn (1 for first, 2 for second): ";
    cin >> choice;

    if (choice == 1) {
        currentPlayer = PLAYER_O;
    } else if (choice == 2) {
        currentPlayer = PLAYER_X;
    } else {
        cout << "Invalid choice. Please choose 1 or 2." << endl;
        return 0;
    }

    cout << "Choose the depth for AI (1 for 1-ply, 2 for 2-ply): ";
    cin >> maxDepth;

    if (currentPlayer == PLAYER_X) {
        // AI plays its first move
        pair<int, int> bestMove = findBestMove(board, maxDepth);
        board[bestMove.first][bestMove.second] = PLAYER_X;
    }

    while (true) {
        printBoard(board); // Print the current board

        int row, col;

        if (currentPlayer == PLAYER_O) {
            cout << "Enter your move (row and column): ";
            cin >> row >> col;

            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != EMPTY) {
                cout << "Invalid move. Try again." << endl;
                continue;
            }

            board[row][col] = PLAYER_O;
        }

        int gameResult = evaluate(board);

        if (gameResult == 1) {
            printBoard(board);
            cout << "AI wins!" << endl;
            break;
        } else if (gameResult == -1) {
            printBoard(board);
            cout << "You win!" << endl;
            break;
        } else if (isGameOver(board)) {
            printBoard(board);
            cout << "It's a tie!" << endl;
            break;
        }

        // Switch players
        currentPlayer = (currentPlayer == PLAYER_O) ? PLAYER_X : PLAYER_O;

        if (currentPlayer == PLAYER_X) {
            // AI plays its move
            pair<int, int> bestMove = findBestMove(board, maxDepth);
            board[bestMove.first][bestMove.second] = PLAYER_X;
        }
    }

    return 0;
}
