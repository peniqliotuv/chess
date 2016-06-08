//makemove.h
#ifndef MAKE_MOVE
#define MAKE_MOVE

#include "enums.h"
#include "board.h"

void clearPiece(const int sq, board& b);
void addPiece(const int sq, board& b, int piece);
void movePiece(const int from, const int to, board& b);
//Returns boolean value if move is valid or not
bool makeMove(board& b, int move);
void takeMove(board& b);

#endif
