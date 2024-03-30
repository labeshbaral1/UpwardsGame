#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h" 

#define DEBUG(...) fprintf(stderr, "[          ] [ DEBUG ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, " -- %s()\n", __func__)



GameHistory game_history = {0};



//Helper
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
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
int compare_ignore_spaces(const char *str1, const char *str2) {

    while (*str2 == ' ') { 
        str1++;
        str2++;
    }
    return strcmp(str1, str2);
    }








GameState* initialize_game_state(const char *filename){

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

    return state;
}




//HELPER FOR PLACING TILES
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

int check_words_in_arr(char *row, int n) {

    char buffer[n+1]; 
    int wordCount = 0; 
    int bufferIndex = 0; 

    int valid = 1; 

    for (int index = 0; index <= n; index++) {
        if (row[index] == '\0' || row[index] == '.') { 
            if (bufferIndex > 0) { 
                buffer[bufferIndex] = '\0';
                
                if (!is_word_legal(buffer)) {
                    printf("Tiles cannot be placed as it results in an invalid word %d: %s\n", wordCount, buffer); 
                    valid = 0; 
                    break;
                }

            
                bufferIndex = 0; 
                wordCount++;
            }
        } else {
            buffer[bufferIndex++] = row[index]; 
        }
    }

    return valid; 
}

int check_rows_and_cols(GameState *game) {

    (void) game;


    for (int row = 0; row < game->rows; row++) {
        char* rowTiles = get_row(game, row);
        if (rowTiles == NULL) {
            break;
        }


        int isValid = check_words_in_arr(rowTiles, game->cols);


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


        int isValid = check_words_in_arr(colTiles, game->rows);


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


    if (!check_rows_and_cols(game)){
        return 0;
    }   


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



GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed){
    *num_tiles_placed = 0;
    
    (void) row;
    (void) col;
    (void) direction;
    (void) tiles;


    
    if(!validate_place_tiles(game, row, col, direction, tiles, num_tiles_placed)){
        return game;
    }

    printf("Tiles Sucessfully Placed\n");
    return game;    
}


GameState* undo_place_tiles(GameState *game){
    
    return game;
}


void free_game_state(GameState *game){
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




void save_game_state(GameState *game, const char *filename){
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
}



void test(GameState *game){

    printf("%d", check_rows_and_cols(game));
   
}