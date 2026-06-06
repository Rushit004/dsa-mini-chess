#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <cctype>
using namespace std;

const int SIZE = 4;



struct Move {
    int fromX, fromY;
    int toX, toY;
    char capturedPiece;
};

// Board: Uppercase = White (P, K), Lowercase = Black (p, k)
char board[SIZE][SIZE] = {
    {'P', ' ', ' ', 'K'},
    {' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' '},
    {'k', ' ', ' ', 'p'}
};

stack<Move> moveHistory;
map<pair<int, int>, vector<pair<int, int>>> graph;

// ── Print Board ──────────────────────────
void printBoard() {
    cout << "\n     Col\n";
    cout << "      0  1  2  3\n";
    cout << "    +--+--+--+--+\n";
    for (int i = 0; i < SIZE; ++i) {
        cout << "Row " << i << " |";
        for (int j = 0; j < SIZE; ++j) {
            cout << " " << board[i][j] << " |";
        }
        cout << "\n    +--+--+--+--+\n";
    }
    cout << "\n  White: P (pawn)  K (king)\n";
    cout << "  Black: p (pawn)  k (king)\n";
}

// ── Build Graph (adjacency list of all 8 directions) ──
void initializeGraph() {
    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE; ++y) {
            vector<pair<int, int>> neighbors;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE)
                        neighbors.push_back({nx, ny});
                }
            }
            graph[{x, y}] = neighbors;
        }
    }
}

// ── Ownership Check ──────────────────────
bool belongsToPlayer(char piece, char player) {
    if (player == 'W') return isupper(piece) && piece != ' ';
    return islower(piece);
}

// ── Move Validation & Execution ──────────
bool movePiece(int fromX, int fromY, int toX, int toY, char player) {
    char piece = board[fromX][fromY];

    if (piece == ' ') {
        cout << "  No piece at (" << fromX << "," << fromY << ")!\n";
        return false;
    }

    if (!belongsToPlayer(piece, player)) {
        cout << "  That's not your piece!\n";
        return false;
    }

    // Graph-based adjacency check
    bool isAdjacent = false;
    for (auto& n : graph[{fromX, fromY}]) {
        if (n.first == toX && n.second == toY) {
            isAdjacent = true;
            break;
        }
    }
    if (!isAdjacent) {
        cout << "  Invalid move — target must be adjacent!\n";
        return false;
    }

    char target = board[toX][toY];

    // Can't capture your own piece
    if (target != ' ' && belongsToPlayer(target, player)) {
        cout << "  Can't capture your own piece!\n";
        return false;
    }

    char upperPiece = (char)toupper(piece);

    // Pawn rules: move straight forward only, no captures
    if (upperPiece == 'P') {
        int direction = (player == 'W') ? 1 : -1;
        if (toX != fromX + direction || toY != fromY) {
            cout << "  Invalid pawn move! Pawns move straight forward one step.\n";
            return false;
        }
        if (target != ' ') {
            cout << "  Pawns can't capture straight ahead!\n";
            return false;
        }
    }

    // King: any adjacent cell (already validated by graph)

    // Push to stack before executing
    moveHistory.push({fromX, fromY, toX, toY, target});

    // Execute move
    board[toX][toY] = piece;
    board[fromX][fromY] = ' ';
    return true;
}

// ── Undo via Stack ───────────────────────
void undoMove() {
    if (moveHistory.empty()) {
        cout << "  Nothing to undo!\n";
        return;
    }
    Move last = moveHistory.top();
    moveHistory.pop();
    board[last.fromX][last.fromY] = board[last.toX][last.toY];
    board[last.toX][last.toY] = last.capturedPiece;
    cout << "  Last move undone.\n";
}

// ── Win Condition ────────────────────────
bool kingAlive(char player) {
    char king = (player == 'W') ? 'K' : 'k';
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] == king) return true;
    return false;
}

// ── Switch Player ────────────────────────
char switchPlayer(char current) {
    return (current == 'W') ? 'B' : 'W';
}

// ── Player Turn (loop-based, no recursion) ──
void playerTurn(char player) {
    string playerName = (player == 'W') ? "White" : "Black";
    cout << "\n>>> Player " << playerName << "'s turn\n";
    cout << "    Enter: fromRow fromCol toRow toCol\n";
    cout << "    Enter: -1 to undo | 0 to quit\n>>> ";

    while (true) {
        int fx;
        if (!(cin >> fx)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  Invalid input. Try again: ";
            continue;
        }
        if (fx == 0) {
            cout << "\n  Thanks for playing DSA Mini Chess!\n";
            exit(0);
        }
        if (fx == -1) {
            undoMove();
            return;
        }
        int fy, tx, ty;
        if (!(cin >> fy >> tx >> ty)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  Invalid input. Try again: ";
            continue;
        }
        if (movePiece(fx, fy, tx, ty, player)) return;
        cout << "  Try again: ";
    }
}

// ── Main ─────────────────────────────────
int main() {
    initializeGraph();

    cout << "╔══════════════════════════════╗\n";
    cout << "║     DSA Mini Chess  (4x4)    ║\n";
    cout << "╠══════════════════════════════╣\n";
    cout << "║  Graph  |  Stack  |  Array   ║\n";
    cout << "╚══════════════════════════════╝\n";
    cout << "\nControls:\n";
    cout << "  Move  : fromRow fromCol toRow toCol\n";
    cout << "  Undo  : -1\n";
    cout << "  Quit  : 0\n";

    char currentPlayer = 'W';

    while (true) {
        printBoard();
        playerTurn(currentPlayer);

        char opponent = switchPlayer(currentPlayer);
        if (!kingAlive(opponent)) {
            printBoard();
            string winner = (currentPlayer == 'W') ? "White" : "Black";
            cout << "\n  *** " << winner << " wins! The opponent's King has been captured! ***\n";
            break;
        }

        currentPlayer = switchPlayer(currentPlayer);
    }
    return 0;
}