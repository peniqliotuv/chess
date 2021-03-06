//io.h
#ifndef IO
#define IO

#include <iostream>
#include <string>
#include "movelist.h"
#include "board.h"
#include "search.h"

//Prints square in string format
std::string printSquare(const int sq);
//Prints move in string format
//optional 5th character at the end of the string
//refers to what piece was captured (if any)
std::string printMove(const int move);
void printMoveList(moveList& list);
void printPiece(int index);
int parseMove(char* move, board& b);

//input interrupts
int inputWaiting();
void readInput(searchInfo* search);

#endif
