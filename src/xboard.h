//xboard.h
#ifndef XBOARD_H
#define XBOARD_H

#include "board.h"
#include "enums.h"
#include "search.h"

int threeFoldRep(const board& b);
bool drawMaterial(const board& b);
bool checkResult(board& b);
void printOptions();
void consoleLoop(board& b, searchInfo* search);
void XBoardLoop(board& b, searchInfo* search);


#endif
