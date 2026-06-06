<div align="center">


<br/>

![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Status](https://img.shields.io/badge/Status-Complete-00b894?style=flat-square)

</div>

---

## Overview

**dsa-mini-chess** is a terminal-based 4×4 chess game built in **C++** that demonstrates three core data structures working together in a single interactive application.

It supports:

- Two-player turn-based gameplay (White vs Black)
- Legal move validation using a graph
- Unlimited undo using a stack
- Win detection when a King is captured

This project was submitted as an **Innovative Assignment** for the Data Structures and Algorithms course at Nirma University.

---

## DSA Concepts Applied

### Graph — Adjacency List

Each cell on the 4×4 board is a **node**. Every valid neighbour (up to 8 directions) is stored as an **edge** in an adjacency list using `std::map`. Move validation simply checks whether the destination exists in the source cell's neighbour list.

### Stack — Move History (Undo)

Every executed move is **pushed** onto a `std::stack<Move>`. When the player enters `-1`, the top move is **popped** and the board is restored to its previous state. This implements O(1) undo with no board scanning.

### 2D Array — Board Representation

The 4×4 board is stored as a `char board[4][4]`. Uppercase letters represent White pieces (`P`, `K`) and lowercase represent Black pieces (`p`, `k`), making ownership checks straightforward using `isupper` / `islower`.

---

## Piece Rules

| Piece | White | Black | Movement |
|-------|-------|-------|----------|
| Pawn  | `P`   | `p`   | One step forward only, no captures straight ahead |
| King  | `K`   | `k`   | One step in any direction (8 directions) |

Win condition: capture the opponent's King.

---

## Folder Structure

```
dsa-mini-chess/
├── mini_chess.cpp
└── README.md
```

---

## How to Run

1. **Compile the program:**

```
g++ mini_chess.cpp -o mini_chess
```

2. **Run the executable:**

```
./mini_chess
```

3. **Controls during the game:**

```
   <fromRow fromCol toRow toCol>  to move
   -1  to undo last move
   -2  to quit
```

---

## Sample Gameplay

```
  +=========================================+
  |       D S A   M I N I   C H E S S      |
  +=========================================+

         Col 0    Col 1    Col 2    Col 3
       +--------+--------+--------+--------+
 Row 0 | W.Pwn  |  ....  |  ....  | W.Kng  |
       +--------+--------+--------+--------+
 Row 1 |  ....  |  ....  |  ....  |  ....  |
       +--------+--------+--------+--------+
 Row 2 |  ....  |  ....  |  ....  |  ....  |
       +--------+--------+--------+--------+
 Row 3 | B.Kng  |  ....  |  ....  | B.Pwn  |
       +--------+--------+--------+--------+

  Pieces:  W.Pwn = White Pawn   W.Kng = White King
           B.Pwn = Black Pawn   B.Kng = Black King

  Controls:  <fromRow fromCol toRow toCol>  to move
             -1  to undo last move
             -2  to quit

>>> White's turn
    Move: fromRow fromCol toRow toCol   |   Undo: -1   |   Quit: -2
```

---

## Improvements Over Original

- **Player ownership check** — players can only move their own pieces
- **Loop-based retry** — replaced recursive retries (stack overflow risk) with a `while` loop
- **Win condition** — game ends and declares a winner when a King is captured
- **Quit support** — enter `0` at any time to exit cleanly
- **Input validation** — handles malformed input without crashing
- **Cleaner board display** — grid-style rendering with column/row labels

---

## About the Author

Rushit Tholiya


- [LinkedIn](https://linkedin.com/in/rushit-tholiya-605341311)
- [GitHub Profile](https://github.com/Rushit004)

---

<div align="center">
<img src="https://capsule-render.vercel.app/api?type=waving&color=0:16213e,100:1a1a2e&height=100&section=footer" />
</div>
