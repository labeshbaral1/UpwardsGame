#include <stdio.h>
#include <stdlib.h>

#include "hw3.h"


int main(void) {
    int num_tiles_placed = 0;
    GameState *game = initialize_game_state("./tests/boards/board02.txt");

    game = place_tiles(game, 4, 5, 'H', "BRAMBLEBERRY", &num_tiles_placed);
    printf("Test 1: %d tiles placed, expected 12.\n", num_tiles_placed);

    game = undo_place_tiles(game);

    game = place_tiles(game, 4, 5, 'V', "BRAMBLEBERRY", &num_tiles_placed);
    printf("Test 2: %d tiles placed, expected 12.\n", num_tiles_placed);


    game = place_tiles(game, 5, 2, 'H', "BEA ABLE", &num_tiles_placed);
    printf("Test 3: %d tiles placed, expected 7.\n", num_tiles_placed);

    game = place_tiles(game, 5, 1, 'H', "CL", &num_tiles_placed);
    printf("Test 4: %d tiles placed, expected 2.\n", num_tiles_placed);


    game = place_tiles(game, 5, 6, 'H', "ANCE", &num_tiles_placed);
    printf("Test 5: %d tiles placed, expected 0.\n", num_tiles_placed);

    game = place_tiles(game, 5, 7, 'H', "NC", &num_tiles_placed);
    printf("Test 6: %d tiles placed, expected 2.\n", num_tiles_placed);

    game = place_tiles(game, 3, 8, 'V', "EX ELLENCE", &num_tiles_placed);
    printf("Test 7: %d tiles placed, expected 9.\n", num_tiles_placed);

    game = place_tiles(game, 10, 3, 'H', "AB RU CATOR", &num_tiles_placed);
    printf("Test 8: %d tiles placed, expected 9.\n", num_tiles_placed);

    game = place_tiles(game, 8, 3, 'H', "EM EL ISHMENT", &num_tiles_placed);
    printf("Test 9: %d tiles placed, expected 11.\n", num_tiles_placed);

    game = undo_place_tiles(game);

    game = place_tiles(game, 8, 3, 'H', "EM AL ONURIDAE", &num_tiles_placed);
    printf("Test 10: %d tiles placed, expected 12.\n", num_tiles_placed);

    game = place_tiles(game, 5, 1, 'H', "AB", &num_tiles_placed);
    printf("Test 11: %d tiles placed, expected 2.\n", num_tiles_placed);

    save_game_state(game, "./tests/actual_outputs/output.txt");
    // free_game_state(game);

    return 0;
}
