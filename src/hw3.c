#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h" 

#define DEBUG(...) fprintf(stderr, "[          ] [ DEBUG ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, " -- %s()\n", __func__)


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

    printf("rows: %d cols: %d", count_rows(filename), count_columns(filename));
    // GameState *state = (GameState *)malloc(sizeof(GameState));
    // if (state == NULL) {
    //     perror("Failed to allocate memory for game state");
    //     fclose(file);
    //     return NULL;
    // }



    fclose(file);

    return NULL;
}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {
    (void)game;
    (void)row;
    (void)col;
    (void)direction;
    (void)tiles;
    (void)num_tiles_placed;
    return NULL;
}

GameState* undo_place_tiles(GameState *game) {
    (void)game;
    return NULL;
}

void free_game_state(GameState *game) {
    (void)game;
}

void save_game_state(GameState *game, const char *filename) {
    (void)game;
    (void)filename;
}
