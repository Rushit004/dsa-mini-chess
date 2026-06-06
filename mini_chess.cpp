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
    bool undoTurn;  // BUG FIX #6: track if this move should un-switch the turn
};

char board[SIZE][SIZE] = {
    {'P', ' ', ' ', 'K'},
    {' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' '},
    {'k', ' ', ' ', 'p'}
};

stack<Move> moveHistory;
map<pair<int,int>, vector<pair<int,int>>> graph;

void printBoard() {
    cout << "\n";
    cout << "  +=========================================+\n";
    cout << "  |       D S A   M I N I   C H E S S      |\n";
    cout << "  +=========================================+\n\n";
    cout << "         Col 0    Col 1    Col 2    Col 3\n";
    cout << "       +--------+--------+--------+--------+\n";
    for (int i = 0; i < SIZE; ++i) {
        cout << " Row " << i << " |";
        for (int j = 0; j < SIZE; ++j) {
            char c = board[i][j];
            // BUG FIX: Visual clarity — label pieces with full names
            if      (c == 'P') cout << " W.Pwn  |";
            else if (c == 'K') cout << " W.Kng  |";
            else if (c == 'p') cout << " B.Pwn  |";
            else if (c == 'k') cout << " B.Kng  |";
            else               cout << "  ....  |";
        }
        cout << "\n       +--------+--------+--------+--------+\n";
    }
    cout << "\n  Pieces:  W.Pwn = White Pawn   W.Kng = White King\n";
    cout << "           B.Pwn = Black Pawn   B.Kng = Black King\n";
    cout << "\n  Controls:  <fromRow fromCol toRow toCol>  to move\n";
    cout << "             -1  to undo last move\n";
    cout << "             -2  to quit\n\n";
}

void initializeGraph() {
    for (int x = 0; x < SIZE; ++x)
        for (int y = 0; y < SIZE; ++y) {
            vector<pair<int,int>> neighbors;
            for (int dx = -1; dx <= 1; ++dx)
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE)
                        neighbors.push_back({nx, ny});
                }
            graph[{x, y}] = neighbors;
        }
}

bool belongsToPlayer(char piece, char player) {
    if (player == 'W') return isupper(piece) && piece != ' ';
    return islower(piece);
}

bool isOpponent(char piece, char player) {
    if (piece == ' ') return false;
    return !belongsToPlayer(piece, player);
}

// BUG FIX #3: Return whether the move was an undo so we can keep same player's turn
bool movePiece(int fromX, int fromY, int toX, int toY, char player) {
    if (fromX < 0 || fromX >= SIZE || fromY < 0 || fromY >= SIZE ||
        toX   < 0 || toX   >= SIZE || toY   < 0 || toY   >= SIZE) {
        cout << "  [!] Out of bounds — valid range is 0 to 3.\n";
        return false;
    }

    char piece = board[fromX][fromY];

    if (piece == ' ') {
        cout << "  [!] No piece at (" << fromX << "," << fromY << ").\n";
        return false;
    }
    if (!belongsToPlayer(piece, player)) {
        cout << "  [!] That piece doesn't belong to you!\n";
        return false;
    }

    // Check adjacency via graph
    bool isAdjacent = false;
    for (auto& n : graph[{fromX, fromY}])
        if (n.first == toX && n.second == toY) { isAdjacent = true; break; }
    if (!isAdjacent) {
        cout << "  [!] Target must be an adjacent square.\n";
        return false;
    }

    char target = board[toX][toY];

    if (target != ' ' && belongsToPlayer(target, player)) {
        cout << "  [!] Can't capture your own piece!\n";
        return false;
    }

    char upperPiece = (char)toupper(piece);

    if (upperPiece == 'P') {
        // BUG FIX #4 + #1: Pawn movement — forward only; diagonal only for capture
        // White moves DOWN (row increases), Black moves UP (row decreases)
        int direction = (player == 'W') ? 1 : -1;
        int dr = toX - fromX;
        int dc = toY - fromY;

        if (dc == 0) {
            // Straight forward — only valid if square is empty
            if (dr != direction) {
                cout << "  [!] Pawn must move one step forward.\n";
                return false;
            }
            if (target != ' ') {
                cout << "  [!] Pawn can't capture straight ahead!\n";
                return false;
            }
        } else if (abs(dc) == 1 && dr == direction) {
            // BUG FIX #5: Diagonal capture — only valid if enemy is there
            if (target == ' ') {
                cout << "  [!] Pawn can only move diagonally to capture an enemy piece.\n";
                return false;
            }
            if (!isOpponent(target, player)) {
                cout << "  [!] Pawn can only capture enemy pieces diagonally.\n";
                return false;
            }
        } else {
            cout << "  [!] Invalid pawn move. Move forward or capture diagonally.\n";
            return false;
        }
    }
    // Kings: any adjacent square, capture allowed (no extra restriction needed)

    moveHistory.push({fromX, fromY, toX, toY, target, false});
    board[toX][toY] = piece;
    board[fromX][fromY] = ' ';
    return true;
}

// BUG FIX #6: Undo pushes a sentinel so playerTurn knows not to switch player
void undoMove(stack<Move>& history) {
    if (history.empty()) {
        cout << "  [!] Nothing to undo!\n";
        return;
    }
    Move last = history.top();
    history.pop();
    board[last.fromX][last.fromY] = board[last.toX][last.toY];
    board[last.toX][last.toY] = last.capturedPiece;
    cout << "  [OK] Last move undone.\n";
}

bool kingAlive(char player) {
    char king = (player == 'W') ? 'K' : 'k';
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] == king) return true;
    return false;
}

char switchPlayer(char current) { return (current == 'W') ? 'B' : 'W'; }

// BUG FIX #6: Returns true if a real move was made (turn should advance)
//             Returns false if undo happened or invalid (stay on same player)
bool playerTurn(char player) {
    string playerName = (player == 'W') ? "White" : "Black";
    cout << ">>> " << playerName << "'s turn\n";
    cout << "    Move: fromRow fromCol toRow toCol   |   Undo: -1   |   Quit: -2\n>>> ";

    while (true) {
        int fx;
        if (!(cin >> fx)) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "  [!] Invalid input. Try again: ";
            continue;
        }
        if (fx == -2) {
            cout << "\n  Thanks for playing DSA Mini Chess!\n";
            exit(0);
        }
        if (fx == -1) {
            undoMove(moveHistory);
            return false;  // BUG FIX #6: don't advance turn after undo
        }
        int fy, tx, ty;
        if (!(cin >> fy >> tx >> ty)) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "  [!] Invalid input. Try again: ";
            continue;
        }
        if (movePiece(fx, fy, tx, ty, player)) return true;
        cout << "  Try again: ";
    }
}

int main() {
    initializeGraph();

    cout << "\n╔══════════════════════════════════╗\n";
    cout << "║      DSA MINI CHESS  (4x4)       ║\n";
    cout << "╠══════════════════════════════════╣\n";
    cout << "║   Graph  ·  Stack  ·  2D Array   ║\n";
    cout << "╚══════════════════════════════════╝\n\n";
    cout << "  Data Structures Used:\n";
    cout << "    Graph  → adjacency list for valid move detection\n";
    cout << "    Stack  → move history for undo functionality\n";
    cout << "    Array  → 4x4 board representation\n\n";

    char currentPlayer = 'W';

    while (true) {
        printBoard();
        // BUG FIX #6: Only switch player if a real move was made
        bool moveMade = playerTurn(currentPlayer);

        char opponent = switchPlayer(currentPlayer);
        if (!kingAlive(opponent)) {
            printBoard();
            string winner = (currentPlayer == 'W') ? "White" : "Black";
            cout << "\n  ★★★ " << winner << " wins! The opponent's King has been captured! ★★★\n\n";
            break;
        }
        if (moveMade) currentPlayer = switchPlayer(currentPlayer);
    }
    return 0;
}
