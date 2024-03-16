#include <stdio.h>
#include <stdlib.h>

#include "hw3.h"

int main(void) {
    char *actual_filename = "./tests/actual_outputs/test_output.txt";
    int num_tiles_placed = 0;
	GameState *game = initialize_game_state("./tests/boards/board01.txt"); 
    game = place_tiles(game, 3, 2, 'H', "C", &num_tiles_placed);
    save_game_state(game, actual_filename);
    free_game_state(game);

    return 0;
    
    }
