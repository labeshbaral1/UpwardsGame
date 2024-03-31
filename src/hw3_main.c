#include <stdio.h>
#include <stdlib.h>

#include "hw3.h"


int main(void) {
    int num_tiles_placed = 0;


	GameState *game = initialize_game_state("./tests/boards/board04.txt"); 

    game = place_tiles(game, 6, 11, 'H', "SNEER", &num_tiles_placed);
    printf("PLACED %d tiles: expected 0 tiles\n", num_tiles_placed);


    // // game = place_tiles(game, 2, 5, 'V', "P TAL", &num_tiles_placed);
    // // printf("PLACED %d tiles: expected 4 tiles\n", num_tiles_placed);

    // // game = place_tiles(game, 6, 1, 'H', "SN I", &num_tiles_placed);
    // // printf("PLACED %d tiles: expected 3 tiles\n", num_tiles_placed);
   
   
    save_game_state(game, "./tests/actual_outputs/output.txt");
    free_game_state(game);

    return 0;
}
