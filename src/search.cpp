//search.cpp
#include "search.h"

bool isRepetition(const board& b){
  for (int i=(b.plyHistory - b.fiftyMoves); i<b.plyHistory-1; ++i){
    if (b.posKey == b.getPrevPosKey(i)){
      return true;
    }
  }
  return false;
}

void searchPosition(board& b){

}
