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

int moveList::ml_getMove(int count){
  return moves[count].getMove();
}

int moveList::ml_getScore(int count){
  return moves[count].getScore();
}

void moveList::incrementCount(){
  (this->count)++;
}

void moveList::setCount(int cnt){
  this->count = cnt;
}
