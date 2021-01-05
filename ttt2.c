#include "ttt.h"
#include <string.h>
/**
 *  File Name: ttt2.c
 *  Author: Rehan Nagoor Mohideen
 *  Student ID: 1100592
 *  Purpose: Contains functions for CIS2520 assignment 7
 */

void init_boards(){//Used to initialize all boards init values to 0
    int i;
    for (i = 0; i < HSIZE; i++){
        htable[i].init = 0;
    }
}

int depth( Board board ){//Calculates the depth of a board
    int i, depth = 0;
    for (i = 0; i < BSIZE-1; i++){
        if (board[i] == 'X' || board[i] == 'O'){//by summing the number of moves played
            depth++;
        }
    }
    return depth;
}


char winner( Board board ){//calculates the winner of the board
    int i;
    for (i = 0; i < 8; i++){//for each possible way of winning
        //check each field in for each possible way of winning if its taken by the respective player to see if they won
        if ( board[pos2idx[WINS[i][0]]]=='X' && board[pos2idx[WINS[i][1]]]=='X' && board[pos2idx[WINS[i][2]]]=='X' ){
            return 'X';
        } else if ( board[pos2idx[WINS[i][0]]]=='O' && board[pos2idx[WINS[i][1]]]=='O' && board[pos2idx[WINS[i][2]]]=='O' ){
            return 'O';
        }
    }
    if (depth(board) == 9){//if it has reached a depth of 9 and there is no winner means its a tie
        return '-';
    }
    return '?';//otherwise game is still in progress
}

char turn( Board board ){//Calculates whos turn it is based on the board state
    if (depth(board) == 9 || winner(board) == 'X' || winner(board) == 'O'){//if the game is over then no turn
        return '-';
    }
    if (depth(board)%2 == 0){//if the depth is even then it is X's turn
        return 'X';
    }
    return 'O';//otherwise it is )'s turn
}

void init_board( Board board ){//initialize the values in the htable for the board
    int index = board_hash(board);
    //indicate that the board has been initialized and set all values in the board index
    htable[index].init = 1;
    htable[index].turn = turn(board);
    htable[index].depth = depth(board);
    strcpy(htable[index].board, board);
    htable[index].winner = winner(board);
}

void join_graph( Board board ){//to ocnnect boards to each other useing the move table in htable
    int i;
    int index = board_hash(board);
    int newHash;
    Board newBoard;

    if (winner(board) == 'X' || winner(board) == 'O' || winner(board) == '-'){//if the game is over; assign all vales to -1
        htable[index].move[0] = -1;
        htable[index].move[1] = -1;
        htable[index].move[2] = -1;
        htable[index].move[3] = -1;
        htable[index].move[4] = -1;
        htable[index].move[5] = -1;
        htable[index].move[6] = -1;
        htable[index].move[7] = -1;
        htable[index].move[8] = -1;
            
    } else {//if the game isnt over

        for (i = 0; i < 9; i++){//for each spot in the board
            if (board[pos2idx[i]] == 'X' || board[pos2idx[i]] == 'O'){//if an X or O is already played there
                htable[index].move[i] = -1;//assign it as an illegal move

            }else{//otherwise
                strcpy(newBoard, board);//create a copy of the old board
                newBoard[pos2idx[i]] = turn(board);//and assign it the next move to be played
                newHash = board_hash(newBoard);//got the hash location for the new board
                htable[index].move[i] = newHash;//and add that hash to the move table of the old board

                if (htable[newHash].init == 0){//if the board wasnt initialized before
                    init_board(newBoard);//initialize it
                    join_graph(newBoard);//and recursively join_graph (calculate the possible moves for that board)
                }
            }
        }
    }
}

void compute_score(){//to compute and assign all the score values on the htable for each board
    int i, j, k, min, max;

    for (i = 0; i < HSIZE; i++){//assign corresponding values to each board where the game is over in the htable
        if (htable[i].init == 1){//when it is initalized
            if (htable[i].winner == 'X'){
                htable[i].score = 1;
            }

            if (htable[i].winner == 'O'){
                htable[i].score = -1;
            }
            
            if (htable[i].winner == '-'){
                htable[i].score = 0;
            }
        }
    }
    
    for (j = 8; j > -1; j--){//for each position on the board. going from depth of 8-0
        for (i = 0; i < HSIZE; i++){//for all the boards
            if (htable[i].init == 1){
                if (htable[i].depth == j){//in order to backwards from the depth to assign values based on the higher depths

                    if (htable[i].turn == 'X'){//for those in X's turn

                        max = -1;//assigning max to the lowest values to find the max in comparisons
                        for (k = 0; k < 9 && max != 1; k++){//for each position on the board
                            if (htable[i].move[k] != -1 && htable[htable[i].move[k]].score > max){//if it is valid and the new max
                                max = htable[htable[i].move[k]].score;
                            }
                        }
                        htable[i].score = max;//assigning the largest value to the score for when X turn
                    }

                    if (htable[i].turn == 'O'){//for those in O's turn
                        min = 1;//assigning min to the lowest values to find the min in comparisons
                        for (k = 0; k < 9 && min != -1; k++){//for each position on the board
                            if (htable[i].move[k] != -1 && htable[htable[i].move[k]].score < min){//if it is valid and the new min
                                min = htable[htable[i].move[k]].score;
                            }
                        }
                        htable[i].score = min;//assigning the smallest value to the score for when O turn      
                    }
                }
            }
        }
    }
        
}

int best_move( int board ){//to calculate what is the best move in the game
    int i, min = 1, max = -1, pos;

    if (htable[board].turn == 'X'){//if X's turn
        for (i = 0; i < 9; i++){//for each position
            if (htable[board].move[i] != -1 && htable[htable[board].move[i]].score >= max){//if its valid and the score is the maximum
                max = htable[htable[board].move[i]].score;//assign it to the new max
                pos = i;//and the position to pos
                if (htable[htable[board].move[i]].winner == 'X'){//if that move would result in X winning then return that position
                    return pos;
                }
            }
        }
        if (max == -1){//if all the scores are -1 (bad for X)
            for (i = 0; i < 9; i++){//try to find the position that is valid and will not immediately lose the game if played. In order to tie
                if (htable[board].move[i] != -1 && htable[ htable[ htable[board].move[i] ].move[ best_move( htable[board].move[i] )] ].winner == '?'){
                    return i;
                }
            }
            for (i = 0; i < 9; i++){//otherwise play a valid position
                if (htable[board].move[i] != -1){
                    return i;
                }
            }
        }

        return pos;//return the position
    }

    if (htable[board].turn == 'O'){//otherwise if O's turn
        for (i = 0; i < 9; i++){//for each position
            if (htable[board].move[i] != -1 && htable[htable[board].move[i]].score <= min){//if its valid and the score is the minimum
                min = htable[htable[board].move[i]].score;//assign it to the new min
                pos = i;//and the position to pos
                if (htable[htable[board].move[i]].winner == 'O'){//if that move would result in O winning then return that position
                    return pos;
                }
            }
        }
        if (min == 1){//if all the scores are 1 (bad for O)
            for (i = 0; i < 9; i++){//try to find the position that is valid and will not immediately lose the game if played. In order to tie
                if (htable[board].move[i] != -1 && htable[ htable[ htable[board].move[i] ].move[ best_move( htable[board].move[i] )] ].winner == '?'){
                    return i;
                }   
            }
            for (i = 0; i < 9; i++){//otherwise play a valid position
                if (htable[board].move[i] != -1){
                    return i;
                }
            }
        }
        return pos;//return the position
    }
    
    return -1;//will return 0 if the game is already over
}
