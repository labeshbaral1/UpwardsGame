
# Upwords Game Implementation

## Responsibilities

This project implements a subset of the classic game Upwords. The core responsibilities of this project include:




### Language Used
C

### Key Concepts
- **Memory Management**: Perform efficient allocation and deallocation of dynamic memory using malloc and free for 2D arrays.
- **Struct Usage**: Working with structs to manage the game state and tile placements.
- **Game Mechanics**: Implementation of custom game logic to place tiles according to specified rules, grow the game board dynamically, and undo tile placements.
- **Data Structures**: Implementation of a linked list to maintain game history allowing users to undo moves.
- **File IO**: Read and write to files using the stdio.h C library
- **Pointers**: Leverage pointers to efficiently reference stored memory locations and create a small memory footprint 

## Result

This implementation enables the following functionalities within the constraints of the assigned subset of Upwords rules:

- **Tile Placement and Word Formation**: Players can place tiles to form new words or modify existing ones, with the ability to stack tiles up to five layers high.
- **Board Expansion**: The board automatically expands to accommodate new tiles as needed.
- **Undo Mechanism**: Players can undo their last move, reverting the board to its previous state.
- **No Scoring or Drawing**: Focuses purely on the placement and structural integrity of words without the complexity of scoring or tile drawing.

## Game Rules

# Upwords Game Implementation

## Responsibilities

This project implements a subset of the classic game Upwords. The core responsibilities of this project include:

### Language Used
C

### Key Concepts
- **Memory Management**: Perform efficient allocation and deallocation of dynamic memory using malloc and free for 2D arrays.
- **Struct Usage**: Working with structs to manage the game state and tile placements.
- **Game Mechanics**: Implementation of custom game logic to place tiles according to specified rules, grow the game board dynamically, and undo tile placements.
- **Data Structures**: Implementation of a linked list to maintain game history allowing users to undo moves.
- **File IO**: Read and write to files using the stdio.h C library
- **Pointers**: Leverage pointers to efficiently reference stored memory locations and create a small memory footprint 

## Result

This implementation enables the following functionalities within the constraints of the assigned subset of Upwords rules:

- **Tile Placement and Word Formation**: Players can place tiles to form new words or modify existing ones, with the ability to stack tiles up to five layers high.
- **Board Expansion**: The board automatically expands to accommodate new tiles as needed.
- **Undo Mechanism**: Players can undo their last move, reverting the board to its previous state.
- **No Scoring or Drawing**: Focuses purely on the placement and structural integrity of words without the complexity of scoring or tile drawing.

### Game Rules for the Upwords Implementation

In this implementation of Upwords, the game is simplified to focus on tile placement and word formation without the elements of scoring or drawing tiles. Here are the key rules and mechanics:

1. **Board and Tile Placement**:
   - The game board starts with an initial size, but it will expand as needed when players place tiles that exceed the current board dimensions.
   - Players can place tiles either horizontally (left to right) or vertically (top to bottom).
   - The first word placed on the board must be at least two letters long and can be placed anywhere.

2. **Word Formation**:
   - A turn consists of placing one or more tiles on the board, either by creating a new word or by modifying an existing word with additional tiles.
   - All newly formed or modified words must be legal. A word is considered legal if it exists in the provided `words.txt` file.

3. **Stacking Tiles**:
   - Players can stack tiles on top of existing tiles to form new words.
   - Tiles can be stacked up to a maximum of five layers high.
   - When stacking tiles, at least one letter of the original word on the board must remain uncovered in the new word formation.

4. **Expanding the Board**:
   - If a word placement requires more space, the board will automatically expand horizontally or vertically.
   - Only the minimum necessary rows or columns will be added to accommodate the new word.

5. **Using Existing Tiles**:
   - Players can use existing tiles on the board to form new words by placing additional tiles adjacent to or on top of them.
   - A space character in the tile placement input indicates the use of an existing tile on the board.

6. **Legal Words**:
   - The game uses a predefined list of words (from `words.txt`) to determine legality.
   - Modifications like adding an 'S' to pluralize a word are generally not allowed unless the resulting word is specifically listed in `words.txt`.

7. **Undo Functionality**:
   - Players can undo their last move, reverting the board to its state before the last tiles were placed.


### Screenshots of key features

Example of GameBoard Expansion

<img width="849" alt="image" src="https://github.com/labeshbaral1/UpwardsGame/assets/99603007/616b7275-aef3-4a8d-aa9c-abcd6e05fca3">

Example of undoing a move

<img width="573" alt="image" src="https://github.com/labeshbaral1/UpwardsGame/assets/99603007/d944940d-80b4-4385-a0d9-8c82145a78df">


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
