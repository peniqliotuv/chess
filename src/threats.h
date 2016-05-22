#ifndef THREATS
#define THREATS

#include "enums.h"

bool isKnight(int p);
bool isRook(int p);
bool isBishop(int p);
bool isKing(int p);

int sqAttacked(const int sq, const int side, const board& b);

#endif
