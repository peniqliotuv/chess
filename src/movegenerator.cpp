//movegenerator.cpp
#include "movegenerator.h"

void addQuietMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);
  list->ml_setScore(list->getCount(), 0);
  list->incrementCount();
}

void addCaptureMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);
  list->ml_setScore(list->getCount(), 0);
  list->incrementCount();
}

void addEnPasMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);
  list->ml_setScore(list->getCount(), 0);
  list->incrementCount();
}

void generateAllMoves(const board& b, moveList* list){
  list->setCount(0);
}
