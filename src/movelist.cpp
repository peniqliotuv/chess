//movelist.cpp
#include "movelist.h"

int moveList::getCount(){
  return this->count;
}

void moveList::ml_setMove(int count, int move){
  moves[count].setMove(move);
}

void moveList::ml_setScore(int count, int score){
  moves[count].setScore(score);
}

void moveList::incrementCount(){
  (this->count)++;
}

void moveList::setCount(int cnt){
  this->count = cnt;
}
