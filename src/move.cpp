//move.cpp
#include "move.h"

void move::setMove(int m){
  this->move = m;
}

void move::setScore(int s){
  this->score = s;
}

int move::getScore(){
  return this->score;
}

int move::getMove(){
  return this->move;
}
