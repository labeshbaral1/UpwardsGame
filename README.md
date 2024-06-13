
# Upwords Game Implementation

## Responsibilities

This project implements a subset of the classic game Upwords. The core responsibilities of this project include:

- **Memory Management**: Efficient allocation and deallocation of dynamic memory for the game state.
- **Struct Usage**: Working with simple structs to manage the game state and tile placements.
- **Game Mechanics**: Implementation of custom game logic to place tiles according to specified rules, grow the game board dynamically, and undo tile placements.

## Technologies / C Concepts Used

### Language Used
C

### Key Concepts
- **Dynamic Memory Allocation**: Managing game state in memory, ensuring efficient use and avoidance of memory leaks.
- **Structs and Pointers**: Using structs to encapsulate game state and pointers for dynamic data management.
- **File I/O**: Reading from and writing to files to initialize and save game states.

## Result

This implementation enables the following functionalities within the constraints of the assigned subset of Upwords rules:

- **Tile Placement and Word Formation**: Players can place tiles to form new words or modify existing ones, with the ability to stack tiles up to five layers high.
- **Board Expansion**: The board automatically expands to accommodate new tiles as needed.
- **Undo Mechanism**: Players can undo their last move, reverting the board to its previous state.
- **No Scoring or Drawing**: Focuses purely on the placement and structural integrity of words without the complexity of scoring or tile drawing.

### Screenshots and Diagrams
Example of a game board expansion

<img src="https://your-image-link-here.png" alt="Game Board Expansion Example" width="500">

## How to Use

This section outlines the steps necessary to compile and run the Upwords game implementation.

### Installation

Clone the repository to your local machine:

```bash
git clone https://github.com/yourusername/upwords-game.git
```

### Compilation

Navigate to the project directory. Use CMake to configure and build the project:

```bash
cmake -S . -B build
cmake --build build
```

### Running the Program

After building, you can run the game simulation or tests:

```bash
./build/run_game_simulation
./build/run_all_test
```

---
