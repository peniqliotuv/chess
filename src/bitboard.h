//bitboard.h
#ifndef BITBOARD
#define BITBOARD

#include "enums.h"

//Repeatedly pops off the least significant bit, used for debugging
int popBit(U64 *bb);

//Counts the number of bits in the 64 bit unsigned
int countBits(U64 b);

//Prints the board to the screen, used for debugging purposes only
void printBitBoard();

#endif
