//movegenerator.h
#ifndef MOVE_GENERATOR
#define MOVE_GENERATOR

#include "enums.h"
#include "board.h"
#include "movelist.h"
#include "validate.h"
#include "io.h"

//No Captures
void addQuietMove(const board& b, int move, moveList* list);
//Captures
void addCaptureMove(const board& b, int move, moveList* list);
void addWPCaptureMove(const board& b, const int from, const int to, const int cap, moveList* list);
void addWPMove(const board& b, const int from, const int to, moveList* list);
void addBPCaptureMove(const board& b, const int from, const int to, const int cap, moveList* list);
void addBPMove(const board& b, const int from, const int to, moveList* list);
void addEnPasMove(const board& b, int move, moveList* list);
void generateAllMoves(const board& b, moveList* list);
void generateAllCaptureMoves(const board& b, moveList* list);
bool moveExists(board& b, const int move);
//Most Valuable Victim, Least Valuable Attacker
int initMVVLVA();

void pickNextMove(int moveNum, moveList* list);


#endif
