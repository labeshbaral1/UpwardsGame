#include <stdio.h>
#include <stdlib.h>

#include "hw3.h"




int main(void) {
    int num_tiles_placed = 0;

	GameState *game = initialize_game_state("./tests/boards/board01.txt"); 
    printBoard(game);

    game = place_tiles(game, 0, 2, 'V', "UNICED", &num_tiles_placed);


    printBoard(game);
    printf("TilesPlaced: %d\n", num_tiles_placed );

    free_game_state(game);

    return 0;
    
    }
