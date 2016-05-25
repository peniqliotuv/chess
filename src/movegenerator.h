//movegenerator.h
#ifndef MOVE_GENERATOR
#define MOVE_GENERATOR

#include "enums.h"
#include "board.h"
#include "movelist.h"
#include "validate.h"

void addQuietMove(const board& b, int move, moveList* list);
void addCaptureMove(const board& b, int move, moveList* list);
void addWPCaptureMove(const board& b, const int from, const int to, const int cap, moveList* list);
void addWPMove(const board& b, const int from, const int to, moveList* list);
void addEnPasMove(const board& b, int move, moveList* list);
void generateAllMoves(const board& b, moveList* list);

#endif
