//hashkey.h
#ifndef HASHKEY
#define HASHKEY

#include "enums.h"
#include "board.h"

//Generates a unique position key for a board
//which is useful for backtracking positions
//and debugging
U64 generatePosKey(const board& b);

#endif
