#include <stdio.h>
#include <stdlib.h>

#include "hw3.h"


int main(void) {
    int num_tiles_placed = 0;
   GameState *game = initialize_game_state("./tests/boards/board01.txt");

  

   

    game = place_tiles(game, 3, 2, 'V', " OUSE", &num_tiles_placed);

    printf("placed tiles expected 0: %d\n ", num_tiles_placed);


   
   
    save_game_state(game, "./tests/actual_outputs/output.txt");
    free_game_state(game);

    return 0;
}
