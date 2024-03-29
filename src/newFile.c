#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h" 

#define DEBUG(...) fprintf(stderr, "[          ] [ DEBUG ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, " -- %s()\n", __func__)



GameHistory game_history = {0};



//Methods for Clone and Freeing Game
void clone_game(GameState *dest, const GameState *src) {



    if (dest->board != NULL) {
        for (int i = 0; i < dest->rows; i++) {
            for (int j = 0; j < dest->cols; j++) {
                free(dest->board[i][j].top);
            }
            free(dest->board[i]);
        }
        free(dest->board);
    }



    dest->rows = src->rows;
    dest->cols = src->cols;

    dest->board = (Tile **)malloc(dest->rows * sizeof(Tile *));

    if (dest->board == NULL) {
        perror("Failed to allocate memory for board in dest");
        return;
    }


    for (int i = 0; i < dest->rows; i++) {
        dest->board[i] = (Tile *)malloc(dest->cols * sizeof(Tile));
        if (dest->board[i] == NULL) {

            printf("failed to allocate memory for board in dest");
            for (int k = 0; k < i; k++) {
                free(dest->board[k]);
            }
            free(dest->board);
            return;
        }


        for (int j = 0; j < dest->cols; j++) {
            dest->board[i][j].height = src->board[i][j].height;
            dest->board[i][j].top = (char *)malloc(5 * sizeof(char)); 

            if (dest->board[i][j].top == NULL) {
                perror("Failed to allocate memory for top in dest");
                for (int k = 0; k <= j; k++) {
                    free(dest->board[i][k].top);
                }
                free(dest->board[i]);
                free(dest->board);
                return;
            }
            if (dest->board[i][j].height > 0) {
                dest->board[i][j].top[dest->board[i][j].height-1] = src->board[i][j].top[src->board[i][j].height-1]; 
            } else {
                dest->board[i][j].top[0] = '.';
            }
        }   
    }
}





// //Game History
void initGameHistory(GameHistory *history, int initialCapacity) {
    history->states = (GameState **)malloc(initialCapacity * sizeof(GameState *));
    history->top = -1; 
    history->capacity = initialCapacity;
}
GameState* peekTopGameState(const GameHistory *history) {
    if (history->top == -1) {
        printf("Game history is empty.\n");
        return NULL;
    }

    return history->states[history->top];
}


void pushGameState(GameHistory *history, GameState *state) {
    if (history->top+1 == history->capacity) {
        int newCapacity = history->capacity * 2;
        GameState **newStates = (GameState **)realloc(history->states, newCapacity * sizeof(GameState *));
        if (newStates != NULL) {
            history->states = newStates;
            history->capacity = newCapacity;
        } else {
            perror("Failed to resize game history");
            return;
        }
    }

    history->top++;
    history->states[history->top] = state;
    
}
GameState* popGameState(GameHistory *history) {
    if (history->top == -1) {
        printf("Game history is empty.\n");
        return NULL;
    }

    GameState *poppedState = history->states[history->top];
    history->top--;
    return poppedState;
}


void printGameHistory(const GameHistory *history) {
    if (history->top == -1) {
        printf("No game history available.\n");
        return;
    }

    printf("Game History:\n");

    for (int i = history->top; i >= 0; i--) {
        printf("Game State %d:\n", history->top - i + 1);
        printBoard(history->states[i]); 
        printf("____________________\n"); 
    }
}
void freeGameHistory(GameHistory *history) {
    (void) history;

    if (!history || !history->states) return;



    for (int i = 0; i <= history->top; i++) {
        if (history->states[i]) {
            free_game_state(history->states[i]);
        }
    }

    free(history->states);

}







// //Helper functions for debugging
void printBoard(GameState *state) {
    if (state == NULL || state->board == NULL) {
        printf("The game state or board is not initialized.\n");
        return;
    }
    
    for (int i = 0; i < state->rows; i++) {

        for (int j = 0; j < state->cols; j++) {
            if(state->board[i][j].height <= 0){
                printf(".");
            }
            else{
            printf("%c", * ( state->board[i][j].top + (state->board[i][j].height-1)));
            }
        }


        printf("\n");
    }

    for (int i = 0; i < state->rows; i++) {
        for (int j = 0; j < state->cols; j++) {
            printf("%d", (state->board[i][j].height));

        }
            printf("\n");
        }


    printf("\n");
}


void printTile(Tile* t){
    printf("%c", t->top[t->height - 1]);
}


void printRow(Tile **row, int n) {
    for (int i = 0; i < n; i++) {

        if (row[i]->height > 0) {
            printTile(row[i]);
        } else {
            printf("."); 
        }
    }
    printf("\n");
}



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
            state->board[i][j].height = 0;
            state->board[i][j].top[0] = '.';
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
                *(state->board[i][j].top + (state->board[i][j].height)) = (char)ch;

                if(ch != '.'){
                    state->board[i][j].height++;
                }

            }
        }

    fclose(file);


    initGameHistory(&game_history, 10);
    GameState *temp = (GameState *)malloc(sizeof(GameState));
    clone_game(temp, state);
    pushGameState(&game_history, temp);

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
    free(game);
}

void free_row(Tile **row, int n) {
    if (row == NULL) {
        return; 
    }
    
    for (int i = 0; i < n; i++) {
            free(row[i]); 
    }
    
    free(row); 
}


void save_game_state(GameState *game, const char *filename) {

    FILE *file = fopen(filename, "w");
    if(file == NULL){
        printf("Error opening the file");
        return;
    }

    
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if(game->board[i][j].height == 0){
                fprintf(file, "%c", (char)'.');
            }
            else{
                fprintf(file, "%c", *(game->board[i][j].top + (game->board[i][j].height-1)));
            }
        }
        fprintf(file, "\n");

    }


    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            fprintf(file, "%d", (game->board[i][j].height));

        }
            fprintf(file, "\n");
        }



    fclose(file);

    freeGameHistory(&game_history);

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
            printf("Failed to allocate memory for new board columns\n");
            for (int j = 0; j < i; j++) {
                free(new_board[j]);
            }
            free(new_board);
            return;
        }

        for (int j = 0; j < desired_width; j++) {
            if (i < game->rows && j < game->cols) {
                new_board[i][j] = game->board[i][j];
            } else {
                new_board[i][j].top = (char *)malloc(5 * sizeof(char)); 
                if (new_board[i][j].top == NULL) {
                    printf("Failed to allocate memory for tile top\n");
                    for (int k = 0; k < j; k++) {
                        free(new_board[i][k].top);
                    }
                    free(new_board[i]);
                    for (int k = 0; k < i; k++) {
                        free(new_board[k]);
                    }
                    free(new_board);
                    return;
                }
                
                *(new_board[i][j].top) = '.';
                new_board[i][j].height = 0;
            }
        }
    }

    // free old board
    for (int i = game->rows; i < desired_height; i++) {
        for (int j = game->cols; j < desired_width; j++) {
            free(new_board[i][j].top);
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

char* get_row(GameState *game, int row) {
    if (row < 0 || row >= game->rows) {
        return NULL;
    }

    char* rowString = malloc((game->cols + 1) * sizeof(char)); 
    if (rowString == NULL) {
        perror("Failed to allocate memory for rowString");
        return NULL;
    }

    for (int col = 0; col < game->cols; col++) {
        rowString[col] = game->board[row][col].height > 0 ? game->board[row][col].top[game->board[row][col].height - 1] : '.';
    }
    rowString[game->cols] = '\0'; 

    return rowString;
}

char* get_col(GameState *game, int col) {
    if (col < 0 || col >= game->cols) {
        return NULL; 
    }
    
    char *colString = malloc((game->rows + 1) * sizeof(char));
    if (colString == NULL) {
        perror("Failed to allocate memory for colString");
        return NULL;
    }

    for (int row = 0; row < game->rows; row++) {
        colString[row] = game->board[row][col].height > 0 ? game->board[row][col].top[game->board[row][col].height - 1] : '.';
    }
    colString[game->rows] = '\0';

    return colString;
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

int compare_ignore_spaces(const char *str1, const char *str2) {

    while (*str2 == ' ') { 
        str1++;
        str2++;
    }
    return strcmp(str1, str2);
    }



int check_words_in_arr(char *row, int n, const char* tiles, int tiles_len) {
    (void) tiles_len;

    char buffer[n+1]; 
    int wordCount = 0; 
    int bufferIndex = 0; 

    int valid = 1; // Assume words are valid until proven otherwise

    // Iterate through the characters in the row
    for (int index = 0; index <= n; index++) {
        if (row[index] == '\0' || row[index] == '.') { 
            // End of word encountered
            if (bufferIndex > 0) { 
                // Null-terminate the buffer to form a word
                buffer[bufferIndex] = '\0';
                
                // Check if the word is legal
                if (!is_word_legal(buffer)) {
                    printf("Tiles cannot be placed as it results in an invalid word %d: %s\n", wordCount, buffer); 
                    valid = 0; 
                    break;
                }

                // Compare the word with tiles
                if (compare_ignore_spaces(buffer, tiles) == 0) {
                    valid = 0;
                    printf("Tiles Cannot be placed as it attempts to add a brand new word or completely replaces an existing word: %s\n", buffer);
                    break;
                }

                // Reset buffer for the next word
                bufferIndex = 0; 
                wordCount++;
            }
        } else {
            // Add character to the buffer
            buffer[bufferIndex++] = row[index]; 
        }
    }

    return valid; 
}


int check_rows_and_cols(GameState *game, const char* tiles, int tiles_len) {

    (void) game;
    (void) tiles;
    (void) tiles_len;

    for (int row = 0; row < game->rows; row++) {
        char* rowTiles = get_row(game, row);
        if (rowTiles == NULL) {
            break;
        }


        int isValid = check_words_in_arr(rowTiles, game->cols, tiles, tiles_len);


        if (!isValid) {
            printf("Check Rows Failed at row %d: ", row);
            printf("%s\n", rowTiles);
            free(rowTiles);
            return 0;
        }

        free(rowTiles);

    }

   

    for (int col = 0; col < game->cols; col++) {
        char* colTiles = get_col(game, col);

        if (colTiles == NULL) {
            break;
        }


        int isValid = check_words_in_arr(colTiles, game->rows, tiles, tiles_len);


        if (!isValid) {
            printf("Check Cols Failed at col %d: ", col);
            printf("%s\n", colTiles);
            free(colTiles);
            return 0;
        }

        free(colTiles);

        }


    return 1;
}

// int is_new(Tile *row, int n, const char* tiles){
   
//     (void) tiles;

//     char buffer[n + 1];
//     int wordCount = 0;
//     char *words[n + 1];
//     int bufferIndex = 0;

//     memset(buffer, 0, sizeof(buffer));

//     for (int index = 0; index <= n; index++) {
//         if (index == n || row[index].height == 0) {
//             if (bufferIndex > 0) {
//                 buffer[bufferIndex] = '\0';
//                 words[wordCount] = strdup(buffer);
//                 wordCount++;
//                 bufferIndex = 0;
//             }
//         } else {
//             if (row[index].height > 0) {
//                 buffer[bufferIndex++] = row[index].top[row[index].height - 1];
//             }
//         }
//     }

//     int valid = 1;
//     for (int i = 0; i < wordCount; i++) {
//         if (!is_word_legal(words[i])) {
//             printf("Invalid word %d: %s\n", i, words[i]);
//             valid = 0;
//             break;
//         }
//     }

//     // Print the words separated by any number of "."
//     printf("Words in row: ");
//     for (int i = 0; i < wordCount; i++) {
//         printf("%s", words[i]);
//         if (i < wordCount - 1) {
//             printf(".");
//         }
//     }
//     printf("\n");

//     for (int i = 0; i < wordCount; i++) {
//         free(words[i]);
//     }

//     return valid;
// }


// int fails_continuation(GameState *game, int row, int col, char direction, const char *tiles) {

//     (void) row;
//     (void) col; 
//     (void) direction;
//     (void) tiles;

//     if (row < 0 || row >= game->rows || col < 0 || col >= game->cols) {
//         printf("array out of bounds");
//     }


//     if (direction == 'h' || direction== 'H') {
//        Tile** rowTiles = get_row(game, row);
//         if (is_new(*rowTiles, game->cols, tiles)) {
//                 free(rowTiles);
//                 return 1;
//         }
//         free(rowTiles);
//     }


//      else if (direction == 'v' || direction== 'V') {
     

//        Tile** colTiles = get_col(game, col); 

//         printf("\n");
       
//         if(is_new(*colTiles, game->rows, tiles)){

//             free(colTiles);
//             return 1;
//         }
//         free(colTiles);


//     } 

 
//     return 0;
// }

int valid_placement(GameState *game, int row, int col, char direction, const char *tiles, int* num_tiles){
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
            if (game->board[row][col + i].height < 5 && tiles[i]!= ' ') {
                *(game->board[row][col + i].top + game->board[row][col + i].height) = tiles[i];
                game->board[row][col+i].height++;
                number_of_tiles_placed++;
            }
        }
    } else if (direction == 'V' || direction == 'v') {

        for (int i = 0; i < length && (row + i) < game->rows; i++) {
            if (game->board[row + i][col].height < 5 && tiles[i]!= ' ') {                          //where to change stacking doesnt increase count
                *(game->board[row + i][col].top + game->board[row + i][col].height) = tiles[i];   
                game->board[row + i][col].height++;
                number_of_tiles_placed++;

            }
        }
    }


   
    if (!check_rows_and_cols(game, tiles, length)){
        return 0;
    }   

    // if (direction == 'H' || direction == 'h') {

    //     char* rowTiles = get_row(game,row);
    //      if(fails_cont(rowTiles, game->cols, tiles, length)){
    //             free(rowTiles);
    //             return 0;
    //     };
    //     free(rowTiles);


    //   } else if (direction == 'V' || direction == 'v') {
    //     char* colTiles = get_col(game,col);
    //     if(fails_cont(colTiles, game->rows, tiles, length)){
    //             free(colTiles);
    //             return 0;
    //     };
    //     free(colTiles);
      
    //   }

   

    *num_tiles = number_of_tiles_placed;


    return 1;

}
int validate_place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed){
    (void) num_tiles_placed;

    int length = strlen(tiles);

    if(length == 0){
            printf("No tiles given\n");
            return 0;
     }

    if (!(direction == 'H' || direction == 'h' || direction == 'V' || direction == 'v')){
        printf("Invalid Direction of movement %c", direction);
        return 0;
    }

    //ensure that row,col is within bounds
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
    
    
    if(!valid_placement(game, row, col, direction, tiles, num_tiles_placed)) return 0;





    return 1;
}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {
    *num_tiles_placed = 0;

    
    if(!validate_place_tiles(game, row, col, direction, tiles, num_tiles_placed)){
        clone_game(game, peekTopGameState(&game_history));
        return game;
    }

    printf("Tiles Sucessfully Placed\n");

    GameState *temp = (GameState *)malloc(sizeof(GameState));
    clone_game(temp, game);
    
    pushGameState(&game_history, temp);
    return game;
}




GameState* undo_place_tiles(GameState *game) {

    // (void) game;

    if (game_history.top <= 0) { 
        printf("No more moves to undo or initial state reached.\n");
        return game;
    }
    
    GameState *temp = popGameState(&game_history);
    free_game_state(temp);
    free_game_state(game);
    
    GameState *peek = (GameState *)malloc(sizeof(GameState));
    clone_game(peek, peekTopGameState(&game_history));
    return peek;
    
    }
