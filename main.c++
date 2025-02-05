#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
const char PLAYER = 'X';
const char AI = 'O';
bool playerFirst;
int playerScore = 0, aiScore = 0;

void displayBoard() {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << " " << (board[i][j] == ' ' ? ' ' : board[i][j]) << " ";
            if (j < 2) cout << "|";
        }
        cout << "\n";
        if (i < 2) cout << "---+---+---\n";
    }
    cout << "\n";
}

bool isMovesLeft() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return true;
    return false;
}

int evaluate() {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2] && board[row][0] != ' ') {
            return (board[row][0] == AI) ? 10 : -10;
        }
    }
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col] && board[0][col] != ' ') {
            return (board[0][col] == AI) ? 10 : -10;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        return (board[0][0] == AI) ? 10 : -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        return (board[0][2] == AI) ? 10 : -10;
    }
    return 0;
}

int minimax(bool isMax) {
    int score = evaluate();
    if (score == 10 || score == -10)
        return score;
    if (!isMovesLeft())
        return 0;

    int best = isMax ? -1000 : 1000;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = isMax ? AI : PLAYER;
                best = isMax ? max(best, minimax(false)) : min(best, minimax(true));
                board[i][j] = ' ';
            }
        }
    }
    return best;
}

pair<int, int> findBestMove() {
    int bestVal = -1000;
    pair<int, int> bestMove = {-1, -1};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = AI;
                int moveVal = minimax(false);
                board[i][j] = ' ';
                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

void playerMove() {
    int move;
    while (true) {
        cout << "Enter your move (1-9): ";
        cin >> move;
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        if (board[row][col] == ' ') {
            board[row][col] = PLAYER;
            break;
        }
        cout << "Invalid move! Try again.\n";
    }
}

void playGame() {
    srand(static_cast<unsigned>(time(nullptr)));
    fill(&board[0][0], &board[0][0] + 9, ' ');
    cout << "Do you want to go first? (1 for Yes, 0 for No): ";
    cin >> playerFirst;
    if (!playerFirst) {
        vector<pair<int, int>> corners = {{0,0}, {0,2}, {2,0}, {2,2}};
        pair<int, int> move = corners[rand() % 4];
        board[move.first][move.second] = AI;
    }
    displayBoard();
    while (true) {
        if (playerFirst) {
            playerMove();
            displayBoard();
            if (evaluate() == -10) {
                cout << "You win!\n";
                playerScore++;
                break;
            }
        }
        if (!isMovesLeft()) { cout << "It's a draw!\n"; break; }
        pair<int, int> aiMove = findBestMove();
        board[aiMove.first][aiMove.second] = AI;
        displayBoard();
        if (evaluate() == 10) {
            cout << "AI wins!\n";
            aiScore++;
            break;
        }
        if (!isMovesLeft()) { cout << "It's a draw!\n"; break; }
    }
    cout << "Score - Player: " << playerScore << " | AI: " << aiScore << "\n";
    char choice;
    cout << "Play again? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') playGame();
}

int main() {
    playGame();
    return 0;
}

