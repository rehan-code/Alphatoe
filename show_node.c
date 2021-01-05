#include "ttt.h"
#include <stdlib.h>
/**
 *  File Name: show_nodes.c
 *  Author: Rehan Nagoor Mohideen
 *  Student ID: 1100592
 *  Purpose: main function that shows board info for each hash index
 */

int main( int argc, char* argv[] ) {
    if (argc >= 2){//if they supplied an argument then initialize the game
        init_boards();
        init_board(START_BOARD);
        join_graph(START_BOARD);
        compute_score();
    }
    
    int i;
    for (i = 1; i < argc; i++){//for every hash index (argument) that was supplied; print the board at that index
        print_node(htable[atoi(argv[i])]);//assuming valid arguments are supplied
    }

    return 1;
}