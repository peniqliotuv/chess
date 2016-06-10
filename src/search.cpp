//search.cpp
#include "search.h"
#include <iostream>

bool isRepetition(const board& b){
  for (int i=(b.plyHistory - b.fiftyMoves); i<b.plyHistory-1; ++i){
    if (i < 0 || i > MAX_GAME_MOVES) {
      std::cout << "Invalid index " << std::endl;
      return false;
    }
    if (b.posKey == b.getPrevPosKey(i)){
      return true;
    }
  }
  return false;
}

void searchPosition(board& b){

}
