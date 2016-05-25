//movelist.h
#ifndef MOVE_LIST
#define MOVE_LIST

#include "enums.h"
#include "move.h"

class moveList {
private:
  move moves[256];
  //Assuming a maximum of 256 moves for a given position
  int count;
  //How many moves are actually in the movelist
public:
  int getCount();
  void ml_setMove(int count, int move);
  void ml_setScore(int count, int score);
  void incrementCount();
  void setCount(int cnt);
};

#endif
