//search.cpp
#include "search.h"
#include "evaluate.h"
#include "makemove.h"
#include "pvt.h"
#include "io.h"
#include "board.h"
#include "movegenerator.h"
#include "threats.h"
#include <ctime>
#include <iostream>

#define MATE 29000
#define INFINITE 30000

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
  int bestMove = NOMOVE;
  int bestScore = -INFINITE; // Negative infinity
  int currDepth = 0;
  int pvMoves = 0;

  clearForSearch(b, search);
  for (currDepth = 1; currDepth<= search->depth; ++currDepth){
                            //Alpha = - infinity //Beta = infinity
    bestScore = alphaBetaSearch(-INFINITE, INFINITE, currDepth, b, search, true);
    pvMoves = getPVLine(currDepth, b);
    bestMove = b.PVArray[0];
    std::cout << "Depth: " << std::dec << currDepth << " Score: " << std::dec << bestScore
    << " move: " << printMove(bestMove) << " Nodes: " << std::dec << search->nodes << std::endl;

    for (int i=0; i< pvMoves; ++i){
      std::cout << printMove(b.PVArray[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Ordering: " << (search->failHighFirst)/(search->failHigh) << std::endl;
  }
}

void checkUp(){

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
  search->failHigh = 0;
  search->failHighFirst = 0;
}

int alphaBetaSearch(int alpha, int beta, int depth, board& b, searchInfo* search, bool nullMove){
  if (checkBoard(b) != 1) std::cout << "CHECKBOARD FAILED" << std::endl;
  if (depth == 0){
    search->nodes++;
    return evalPosition(b);
  }
  search->nodes++;
  if (isRepetition(b) || b.fiftyMoves >= 100){
    return 0; //draw
  }
  if (b.ply > MAXDEPTH - 1){
    return evalPosition(b); //too deep
  }

  moveList* list = new moveList;
  generateAllMoves(b, list);
  int legal = 0;
  int oldAlpha = alpha;
  int bestMove = NOMOVE;
  int score = -INFINITE;

  for (int i=0; i<list->getCount(); ++i){
    pickNextMove(i, list);
    int move = list->ml_getMove(i);
    if (!makeMove(b, move)){
      continue;
    }
    legal++;
    score = -alphaBetaSearch(-beta, -alpha, depth-1, b, search, true); //negamax
    takeMove(b);

    if (score > alpha){
      if (score >= beta) {
        if (legal == 1){
          search->failHighFirst++;
        }
        search->failHigh++;
        return beta; //Beta cutoff
      }
      alpha = score;
      bestMove = move;
    }
  }
  if (legal == 0){
    if (sqAttacked(b.kingSquare[b.side], b.side^1, b)){
      return -MATE + b.ply;
    }
    else return 0;
  }
  if (alpha != oldAlpha){ //alpha improved
    storePVMove(b, bestMove);
  }
  delete list;
  return alpha;
}

int quiescenceSearch(int alpha, int beta, board& b, searchInfo* search){
  return 0;
  //placeholder
}
