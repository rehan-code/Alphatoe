#include "ttt.h"
#include <stdlib.h>
#include<stdio.h>
/**
 *  File Name: count_nodes.c
 *  Author: Rehan Nagoor Mohideen
 *  Student ID: 1100592
 *  Purpose: main function that calculate the number of nodes initialized in the hash table and prints it to the screen and returns it
 */

int main( int argc, char* argv[] ) {
    //initialize the game. No need to calculate scores
    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);

    int i, total = 0;
    for (i = 0; i < HSIZE; i++){//for every element in the table
        if (htable[i].init == 1){//if it has been initialized
            total++;//then increment the total number of initialized
        }
    }
    printf("%d\n", total);//print the number of nodes that were initialized

    return total;
}