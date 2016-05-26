//pieces.cpp
#include "enums.h"

int isBig[13] = { false, false, true, true, true, true, true, false, true, true, true, true, true };
int isMajor[13] = { false, false, false, false, true, true, true, false, false, false, true, true, true };
int isMinor[13] = { false, false, true, true, false, false, false, false, true, true, false, false, false };
int pieceValue[13]= { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000  };
int pieceColor[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
	BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };
bool pieceSlides[13] = {false, false, false, true, true, true, false, false, false, true, true, true, false};
