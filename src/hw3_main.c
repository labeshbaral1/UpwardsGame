#include <stdio.h>
#include <stdlib.h>

#include "hw3.h"


int main(void) {
    int num_tiles_placed = 0;



    	GameState *game = initialize_game_state("./tests/boards/board02.txt"); 
        
    game = place_tiles(game,0 , 0, 'V', "ARABLE", &num_tiles_placed);

    game = place_tiles(game,0 , 0, 'V', "DO", &num_tiles_placed);

    game = place_tiles(game,0 , 0, 'V', "EN", &num_tiles_placed);

    game = place_tiles(game,0 , 0, 'V', "LI", &num_tiles_placed);

    game = place_tiles(game,0 , 0, 'V', "ME", &num_tiles_placed);

    game = place_tiles(game,0 , 0, 'V', "ST", &num_tiles_placed);

   

    save_game_state(game, "./tests/actual_outputs/output.txt");
    free_game_state(game);

    return 0;
}
