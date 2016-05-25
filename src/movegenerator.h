//movegenerator.h
#ifndef MOVE_GENERATOR
#define MOVE_GENERATOR

#include "enums.h"
#include "board.h"
#include "movelist.h"

void addQuietMove(const board& b, int move, moveList* list);
void addCaptureMove(const board& b, int move, moveList* list);
void addEnPasMove(const board& b, int move, moveList* list);
void generateAllMoves(const board& b, moveList* list);

#endif
