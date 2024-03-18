#include <stdio.h>
#include <stdlib.h>

#include "hw3.h"




int main(void) {
    int num_tiles_placed = 0;

    
	GameState *game = initialize_game_state("./tests/boards/board06.txt"); 
    game = place_tiles(game, 2, 2, 'V', "LI TLESSNESS", &num_tiles_placed);
    // save_game_state(game, actual_filename);
    // printBoard(game);

    printf("Placed Tiles: %d\n", num_tiles_placed);
    free_game_state(game);

    return 0;
    
    }
