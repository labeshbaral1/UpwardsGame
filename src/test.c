#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>



void assertFilesEqual(const char *file1Path, const char *file2Path) {
    FILE *file1 = fopen(file1Path, "r");
    FILE *file2 = fopen(file2Path, "r");
    int ch1, ch2;

    if (file1 == NULL || file2 == NULL) {
        if (file1) fclose(file1);
        if (file2) fclose(file2);
        printf("Error opening files.\n");
        exit(1); // Exit if either file couldn't be opened
    }

    while (1) {
        ch1 = fgetc(file1);
        ch2 = fgetc(file2);

        if (ch1 != ch2) {
            printf("Files are not equal.\n");
            break; // Files are not equal
        }

        if (ch1 == EOF && ch2 == EOF) {
            printf("Files are equal.\n");
            break; // Reached the end of both files and they are equal
        }
    }

    fclose(file1);
    fclose(file2);
}




typedef struct Tile{
    char* top;
    int height;
} Tile;


typedef struct GameState
{
    Tile **board;
    int rows;
    int cols;

} GameState;

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
    (void)filename;

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
            perror("Failed to allocate memory for board columns");
            return NULL;
        }

        for (int j = 0; j < state->cols; j++) {
            state->board[i][j].top = (char *)malloc(5 * sizeof(char)); // Allocate space for 5 chars + null terminator
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

    return state;
}
void printBoard(GameState *state) {
    if (state == NULL || state->board == NULL) {
        printf("The game state or board is not initialized.\n");
        return;
    }
    
    for (int i = 0; i < state->rows; i++) {
        for (int j = 0; j < state->cols; j++) {
            printf("%c", * ( (state->board[i][j].top) + state->board[i][j].height));
        }

        printf("\n");
    }
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


void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
int is_word_legal(const char *word) {
    FILE *file = fopen("words.txt", "r");
    if (!file) {
        perror("Failed to open the words file");
        return 0; 
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; 
        to_uppercase(buffer); 

        if (strcmp(word, buffer) == 0) {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; // Word not found
}



int validate_place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed){
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
    

    

    return 1;
}

}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {


    if(!validate_place_tiles(game, row, col, direction, tiles, num_tiles_placed)){
        return game;
    }

    //RESIZE BOARD if necessary
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


    //PLACE THE TILES
    if (direction == 'H' || direction == 'h') {

        for (int i = 0; i < length && (col + i) < game->cols; i++) {
            if (game->board[row][col + i].height < 4 && tiles[i]!= ' ') {
                game->board[row][col + i].height++;
                *(game->board[row][col + i].top + game->board[row][col + i].height) = tiles[i];

                printf("adding[%c]\n", tiles[i]);
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

    return game;

}


GameState* undo_place_tiles(GameState *game) {
    (void)game;
    return game;
}








void free_game_state(GameState *game) {

    if(game == NULL){
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




int main(void){

    // GameState* game = initialize_game_state("ex.txt");
    // int num_tiles_placed = 0;
    // save_game_state(game, "ex-output01.txt");
    // place_tiles(game, 2, 8, 'H', " ROID", &num_tiles_placed);
    // save_game_state(game, "ex-output02.txt");



    
    return 0;
}
