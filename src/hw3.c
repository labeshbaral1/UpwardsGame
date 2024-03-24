#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h" 

#define DEBUG(...) fprintf(stderr, "[          ] [ DEBUG ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, " -- %s()\n", __func__)



GameHistory game_history = {0};



//Methods for Clone and Freeing Game
// void clone_game(GameState *dest, const GameState *src){
//     dest->rows = src->rows;
//     dest->cols = src->cols;
//     dest->board = (Tile **)malloc(dest->rows * sizeof(Tile *));
//     for (int i = 0; i < dest->rows; i++) {
//         dest->board[i] = (Tile *)malloc(dest->cols * sizeof(Tile));
//         for (int j = 0; j < dest->cols; j++) {
//             dest->board[i][j].top = (char *)malloc(5 * sizeof(char)); 
//             dest->board[i][j].height = src->board[i][j].height;  
//                 if (dest->board[i][j].height >= 0) {
//                     for(int k = 0; k <= dest->board[i][j].height;k++){
//                         *(dest->board[i][j].top + k) = *(src->board[i][j].top + k);
//                     }
//                 }
                           
//             }
//     }
// }

void free_game(GameState *game) {
    if (game == NULL) {
        return;
    }

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            free(game->board[i][j].top);
        }
        free(game->board[i]);
    }

    free(game->board);
    free(game);
}


// //Game History
// void initGameHistory(GameHistory *history, int initialCapacity) {
//     history->states = (GameState **)malloc(initialCapacity * sizeof(GameState *));
//     history->top = -1; 
//     history->size = 0;
//     history->capacity = initialCapacity;
// }
// GameState* peekTopGameState(const GameHistory *history) {
//     if (history->top == -1) {
//         printf("Game history is empty.\n");
//         return NULL;
//     }
//     return history->states[history->top];
// }
// void pushGameState(GameHistory *history, GameState *state) {
//     if (history->size == history->capacity) {
//         int newCapacity = history->capacity * 2;
//         GameState **newStates = (GameState **)realloc(history->states, newCapacity * sizeof(GameState *));
//         if (newStates != NULL) {
//             history->states = newStates;
//             history->capacity = newCapacity;
//         } else {
//             perror("Failed to resize game history");
//             return;
//         }
//     }
//     history->top++;
//     history->states[history->top] = state;
//     history->size++;
// }
// GameState* popGameState(GameHistory *history) {
//     if (history->top == -1) {
//         printf("Game history is empty.\n");
//         return NULL;
//     }
//     GameState *poppedState = history->states[history->top];
//     history->top--;
//     history->size--;
//     return poppedState;
// }
// void printGameHistory(const GameHistory *history) {
//     if (history->size == 0) {
//         printf("No game history available.\n");
//         return;
//     }

//     printf("Game History:\n");

//     for (int i = history->top; i >= 0; i--) {
//         printf("Game State %d:\n", history->top - i + 1);
//         printBoard(history->states[i]); 
//         printf("____________________\n"); 
//     }
// }
// void freeGameHistory(GameHistory *history) {
//     (void) history;
//     if (!history || !history->states) return;

//     for (int i = 0; i <= history->top; i++) {
//         if (history->states[i]) {
//             free_game(history->states[i]);
//         }
//     }

//     free(history->states);


//     history->states = NULL;
//     history->top = -1;
//     history->size = 0;
//     history->capacity = 0;
// }







// //Helper functions for debugging
void printBoard(GameState *state) {
    if (state == NULL || state->board == NULL) {
        printf("The game state or board is not initialized.\n");
        return;
    }
    
    for (int i = 0; i < state->rows; i++) {

        for (int j = 0; j < state->cols; j++) {
            if(state->board[i][j].height == -1){
                printf(".");
            }
            else{
            printf("%c", * ( (state->board[i][j].top) + state->board[i][j].height));
            }
        }


        printf("\n");
    }
    for (int i = 0; i < state->rows; i++) {
        for (int j = 0; j < state->cols; j++) {
            printf("%d", (state->board[i][j].height)+1);

        }
            printf("\n");
        }


    printf("\n");
}


// void printRow(Tile *row, int n){
//         for (int i = 0; i < n; i++) {
//                 if (row[i].height > -1) {
//                     printf("%c", row[i].top[row[i].height]); 
//                 } else {
//                     printf("."); 
//                 }
//             }
//             printf("\n"); 
// }



//Initializing the game state
int count_rows(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; 
    }

    int rows = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            rows++;
        }
    }

    fclose(file);
    return rows;
}
int count_columns(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; 
    }

    int columns = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF && ch != '\n') {
        columns++;
    }

    fclose(file);

    return columns;
}


GameState* initialize_game_state(const char *filename) {
    


    FILE *file = fopen(filename, "r");

    if(file == NULL){
        printf("Error opening file");
        return NULL;
    }

    GameState *state = (GameState *)malloc(sizeof(GameState));

    if (state == NULL) {
        perror("Failed to allocate memory for game state");
        fclose(file);
        return NULL;
    }


    state->rows = count_rows(filename);
    state->cols = count_columns(filename);

    state->board = (Tile **)malloc(state->rows * sizeof(Tile *));

    if (state->board == NULL) {
        perror("Failed to allocate memory for board rows");
        free(state);
        fclose(file);
        return NULL;
    }



    for (int i = 0; i < state->rows; i++) {
        state->board[i] = (Tile *)malloc(state->cols * sizeof(Tile));

        if (state->board[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(state->board[j]);
            }

            free(state->board);
            free(state);
            fclose(file);
            printf("Failed to allocate memory for board columns");
            return NULL;
        }

        for (int j = 0; j < state->cols; j++) {
            state->board[i][j].top = (char *)malloc(5 * sizeof(char)); 
            if (state->board[i][j].top == NULL) {
                printf("Failed to allocate memory for array of top");
                return NULL;
            }
            state->board[i][j].height = -1;
        }

    }

        int ch; 
        for (int i = 0; i < state->rows; i++) {
            for (int j = 0; j < state->cols; j++) {
                ch = fgetc(file);
                if(ch == '\n') { 
                    j--; 
                    continue;
                }
                *(state->board[i][j].top + (state->board[i][j].height+1)) = (char)ch;
                
                if(ch != '.'){
                    state->board[i][j].height++;
                }

            }
        }

    fclose(file);


    // initGameHistory(&game_history, 10);
    // GameState *temp = (GameState *)malloc(sizeof(GameState));
    // clone_game(temp, state);

    // pushGameState(&game_history, state);

    return state;
}


//Save game
void free_game_state(GameState *game) {
    if (game == NULL) {
        return;
    }

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            free(game->board[i][j].top);
        }
        free(game->board[i]);
    }

    free(game->board);
    // freeGameHistory(&game_history);
}


void save_game_state(GameState *game, const char *filename) {

    FILE *file = fopen(filename, "w");
    if(file == NULL){
        printf("Error opening the file");
        return;
    }

    
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if(game->board[i][j].height == -1){
                fprintf(file, "%c", (char)'.');
            }
            else{
                fprintf(file, "%c", *(game->board[i][j].top + (game->board[i][j].height)));
            }
        }
        fprintf(file, "\n");

    }


    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            fprintf(file, "%d", (game->board[i][j].height)+1);

        }
            fprintf(file, "\n");
        }



    fclose(file);

}


//Helper Functions for board
void resize_board(GameState *game, int desired_width, int desired_height) {

    Tile **new_board = (Tile **)malloc(desired_height * sizeof(Tile *));
    if (new_board == NULL) {
        printf("Failed to allocate memory for new board rows\n");
        return;
    }


    for (int i = 0; i < desired_height; i++) {
        new_board[i] = (Tile *)malloc(desired_width * sizeof(Tile));

        if (new_board[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(new_board[j]);
            }
            free(new_board);
            printf("Failed to allocate memory for new board columns\n");
            return;
        }

        for (int j = 0; j < desired_width; j++) {


            if (i < game->rows && j < game->cols) {
                new_board[i][j] = game->board[i][j];
            } else {
                new_board[i][j].top = (char *)malloc(5 * sizeof(char)); 
                if (new_board[i][j].top == NULL) {
                    printf("Failed to allocate memory for tile top\n");
                    return;
                }
                *(new_board[i][j].top) = '.';
                new_board[i][j].height = -1;
            }
        }
    }

    for (int i = 0; i < game->rows; i++) {
        free(game->board[i]);
    }
    free(game->board);

    game->board = new_board;
    game->rows = desired_height;
    game->cols = desired_width;
}

Tile** get_row(GameState *game, int row) {

    if (row < 0 || row >= game->rows) {
            return NULL; 
        }
        
        Tile **rowTiles = malloc(game->cols * sizeof(Tile*));
        if (rowTiles == NULL) {
            perror("Failed to allocate memory for rowTiles");
            return NULL;
        }

        for (int col = 0; col < game->cols; col++) {
            rowTiles[col] = &game->board[row][col]; 
        }

        return rowTiles;
}
Tile** get_col(GameState *game, int col) {
    if (col < 0 || col >= game->cols) {
            return NULL; 
        }
        
        Tile **colTiles = malloc(game->rows * sizeof(Tile*));
        if (colTiles == NULL) {
            perror("Failed to allocate memory for colTiles");
            return NULL;
        }

        for (int row = 0; row < game->rows; row++) {
            colTiles[row] = &game->board[row][col]; 
        }

        return colTiles;
}





//Validating the Board
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
int is_word_legal(const char *word) {
    FILE *file = fopen("./tests/words.txt", "r");
    if (!file) {
        perror("Failed to open the words file");
        return 0; 
    }

    char buffer[256];
    char singularForm[256];
    int wordLength = strlen(word);
    int isPlural = word[wordLength - 1] == 'S'; 

    if (isPlural && wordLength < 255) {
        strncpy(singularForm, word, wordLength - 1);
        singularForm[wordLength - 1] = '\0';
    } else {
        strncpy(singularForm, word, wordLength);
        singularForm[wordLength] = '\0';
    }


    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        to_uppercase(buffer); 

        if (strcmp(word, buffer) == 0 || (isPlural && strcmp(singularForm, buffer) == 0)) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0; 
}

int check_words_in_arr(Tile *row, int n) {


    char *words[n+1]; 
    int wordCount = 0;
    char buffer[n + 1];
    int bufferIndex = 0;

    for (int index = 0; index <= n; index++) {
        
        if (index == n || row[index].height == -1) { 
            if (bufferIndex > 0) { 
                buffer[bufferIndex] = '\0'; 
                words[wordCount] = strdup(buffer); 
                wordCount++;
                bufferIndex = 0; 
            }
        } else {
            if (row[index].height >= 0) {
                buffer[bufferIndex++] = row[index].top[row[index].height];
                buffer[bufferIndex] = '\0';
            }
        }
    }


 

    for (int i = 0; i < wordCount; i++) {
        if (!is_word_legal(words[i])) {
            for (int j = 0; j < wordCount; j++) {
                free(words[j]);
            }
            return 0; 
        }
    }

    for (int i = 0; i < wordCount; i++) {
        free(words[i]);
    }


    return 1; 
}




int check_rows_and_cols(GameState *game) {
    for (int row = 0; row < game->rows; row++) {
        Tile **rowTiles = get_row(game, row);
        if (rowTiles == NULL) {
            continue;
        }

        int isValid = check_words_in_arr(*rowTiles, game->cols);
        if (!isValid) {
             printf("Check Rows Failed at ");
            // printRow(*rowTiles, game->cols);
            printf("\n");
            free(rowTiles);
            return 0;
        }
        free(rowTiles);

    }


    for (int col = 0; col < game->cols; col++) {
            Tile **colTiles = get_col(game, col); 
            if (colTiles == NULL) {
                continue; // 
            }

            Tile tempCol[game->rows]; 
            for (int i = 0; i < game->rows; i++) {
                tempCol[i] = *colTiles[i];
            }

            

            int isValid = check_words_in_arr(tempCol, game->rows);
            if (!isValid) {
                printf("Check Cols Failed at ");
                // printRow(tempCol, game->rows);
                printf("\n");

                free(colTiles);
                return 0; 
            }
            free(colTiles);

        }
    return 1;
}



int valid_placement(GameState *game, int row, int col, char direction, const char *tiles){
    int number_of_tiles_placed = 0;

        int length = strlen(tiles);
        int desired_width = game->cols;
        int desired_height = game->rows;

        
        if (direction == 'H' || direction == 'h') {
            if (col + length > game->cols) {
                desired_width = col + length;
            }
        } else if (direction == 'V' || direction == 'v') {
            if (row + length > game->rows) {
                desired_height = row + length; 
            }
        }

        if (desired_width > game->cols || desired_height > game->rows) {
            resize_board(game, desired_width, desired_height);
        }



    //Simulate placing the tiles
    if (direction == 'H' || direction == 'h') {

        for (int i = 0; i < length && (col + i) < game->cols; i++) {
            if (game->board[row][col + i].height < 4 && tiles[i]!= ' ') {
                game->board[row][col + i].height++;
                *(game->board[row][col + i].top + game->board[row][col + i].height) = tiles[i];
                number_of_tiles_placed++;
            }
        }
    } else if (direction == 'V' || direction == 'v') {

        for (int i = 0; i < length && (row + i) < game->rows; i++) {
            if (game->board[row + i][col].height < 4 && tiles[i]!= ' ') {
                game->board[row + i][col].height++;
                *(game->board[row + i][col].top + game->board[row + i][col].height) = tiles[i];
                number_of_tiles_placed++;
            }
        }
    }

    if (!check_rows_and_cols(game)){
        return 0;
    }

    return 1;

}
int validate_place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed){
    (void) num_tiles_placed;

    int length = strlen(tiles);

    if(length == 0){
            return 0;
     }

    if (!(direction == 'H' || direction == 'h' || direction == 'V' || direction == 'v')){
        printf("Invalid Direction of movement %c", direction);
        return 0;
    }
    if (row < 0 || row >= game->rows || col < 0 || col >= game->cols) {
        printf("Position out of bounds: (%d, %d)\n", row, col);
        return 0;
    }
    
    //all tiles are uppercase
    for(int i = 0; tiles[i]!='\0'; i++){
            if (tiles[i] != ' ' && !isupper(tiles[i])){
                printf("All tile characters must be uppercase");
                return 0;
            }
    }
    
    if(!valid_placement(game, row, col, direction, tiles)) return 0;
    return 1;
}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {
    *num_tiles_placed = 0;

    if(!validate_place_tiles(game, row, col, direction, tiles, num_tiles_placed)){
        undo_place_tiles(game);
        return game;
    }

        int length = strlen(tiles);
        int desired_width = game->cols;
        int desired_height = game->rows;

        
        if (direction == 'H' || direction == 'h') {
            if (col + length > game->cols) {
                desired_width = col + length;
            }
        } else if (direction == 'V' || direction == 'v') {
            if (row + length > game->rows) {
                desired_height = row + length; 
            }
        }

        if (desired_width > game->cols || desired_height > game->rows) {
            resize_board(game, desired_width, desired_height);
        }


    if (direction == 'H' || direction == 'h') {

        for (int i = 0; i < length && (col + i) < game->cols; i++) {
            if (game->board[row][col + i].height < 4 && tiles[i]!= ' ') {
                game->board[row][col + i].height++;
                *(game->board[row][col + i].top + game->board[row][col + i].height) = tiles[i];
                (*num_tiles_placed)++;
            }
        }
    } else if (direction == 'V' || direction == 'v') {

        for (int i = 0; i < length && (row + i) < game->rows; i++) {
            if (game->board[row + i][col].height < 4 && tiles[i]!= ' ') {
                game->board[row + i][col].height++;
                *(game->board[row + i][col].top + game->board[row + i][col].height) = tiles[i];
                (*num_tiles_placed)++;
            }
        }
    }

    // GameState *temp = (GameState *)malloc(sizeof(GameState));
    // clone_game(temp, game);
    // pushGameState(&game_history, game);
    return game;
}




GameState* undo_place_tiles(GameState *game) {
        (void) game;

    // if (game_history.size <= 1) { 
    //     printf("No more moves to undo or initial state reached.\n");
    //     return game;
    // }
    
    // popGameState(&game_history);

    // return peekTopGameState(&game_history);

    return game;
    
    }
