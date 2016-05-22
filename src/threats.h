#ifndef THREATS
#define THREATS

#include "enums.h"

bool isKnight(int p);
bool isRookQueen(int p);
bool isBishopQueen(int p);
bool isKing(int p);

bool sqAttacked(const int sq, const int side, const board& b);

#endif
