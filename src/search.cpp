//search.cpp
#include "search.h"
#include "pvt.h"
#include <ctime>
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

void searchPosition(board& b, searchInfo* search){

}

void checkUP(){
  ///
}

void clearForSearch(board& b, searchInfo* search){
  for (int i=0; i<13; ++i){
    for (int j=0; j<BOARD_SIZE; ++j){
      b.searchHistory[i][j] = 0;
    }
  }
  for (int i=0; i<2; ++i){
    for (int j=0; j<MAXDEPTH; ++j){
      b.searchKillers[i][j] = 0;
    }
  }
  clearPVT(b.PVT);
  b.ply = 0;

  clock_t start;
  start = clock();
  search->startTime = 1000 * ((clock() - start) / (double) CLOCKS_PER_SEC);
  search->stopped = 0;
  search->nodes = 0;
}

int alphaBetaSearch(int alpha, int beta, int depth, board& b, searchInfo* search, int nullMove){
  return 0;
  //placeholder
}

int quiescenceSearch(int alpha, int beta, board& b, searchInfo* search){
  return 0;
  //placeholder
}
