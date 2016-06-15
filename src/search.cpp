//search.cpp
#include "search.h"
#include "evaluate.h"
#include "makemove.h"
#include "pvt.h"
#include "io.h"
#include "board.h"
#include "movegenerator.h"
#include "threats.h"
#include "timer.h"
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
    if (search->stopped == true){
      break;
    }
    pvMoves = getPVLine(currDepth, b);
    bestMove = b.PVArray[0];

    if (search->postThinking == true){
      double timeDiff = getTime() - search->startTime;
      std::cout << std::dec << "score:" << bestScore << " depth:" <<
      currDepth << " nodes:" << search->nodes << " time:" <<
      (int) timeDiff << "(ms) ";
    }
    if(search->postThinking == true) {
			pvMoves = getPVLine(currDepth, b);
			std::cout << "pv";
			for(int i = 0; i < pvMoves; ++i) {
        std::cout << " " << printMove(b.PVArray[i]);
			}
			std::cout << std::endl;
		}
  }
    std::cout << std::endl << std::endl << "*** PENIQLIOTUV makes move ***"
    << std::endl << printMove(bestMove) << std::endl;
		makeMove(b, bestMove);
		printBoard(b);
}

void checkUp(searchInfo* search){
  if (search->timeSet == true && getTime() > search->stopTime){
    search->stopped = true;
  }
  readInput(search);
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

  search->startTime = getTime();
  search->stopped = false;
  search->nodes = 0;
  search->failHigh = 0;
  search->failHighFirst = 0;
}

int alphaBetaSearch(int alpha, int beta, int depth, board& b, searchInfo* search, bool nullMove){
  if (checkBoard(b) != 1) std::cout << "CHECKBOARD FAILED" << std::endl;
  if (depth == 0){
    return quiescenceSearch(alpha, beta, b, search);
  }

  if ((search->nodes & 2047) == 0){
    checkUp(search);
  }

  search->nodes++;

  if ((isRepetition(b) || b.fiftyMoves >= 100) && b.ply){
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
  int bestPVMove = probePVT(b);
  if (bestPVMove != NOMOVE){
    for (int i=0; i<list->getCount(); ++i){
      if (list->ml_getMove(i) == bestPVMove){
        list->ml_setScore(i, 2000000);
        break;
      }
    }
  }

  for (int i=0; i<list->getCount(); ++i){
    pickNextMove(i, list);
    int move = list->ml_getMove(i);
    if (!makeMove(b, move)){
      continue;
    }
    legal++;
    score = -alphaBetaSearch(-beta, -alpha, depth-1, b, search, true); //negamax
    takeMove(b);

    if (search->stopped == true){
      return 0;
    }

    if (score > alpha){
      if (score >= beta) {
        if (legal == 1){
          search->failHighFirst++;
        }
        search->failHigh++;
        if (!(move & ISCAPTURE)){ // If not capture move
          b.searchKillers[1][b.ply] = b.searchKillers[0][b.ply];
          b.searchKillers[0][b.ply] = move;
        }
        return beta; //Beta cutoff
      }
      alpha = score;
      bestMove = move;
      if (!(move & ISCAPTURE)){ //Alpha cutoff
        b.searchHistory[b.pieces[FROMSQ(bestMove)]][TOSQ(bestMove)] += depth; //prioritizes based on depth
      }
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
  if (checkBoard(b) != 1) std::cout << "checkboard failed" << std::endl;

  if ((search->nodes & 2047) == 0){
    checkUp(search);
  }

  search->nodes++;

  if (isRepetition(b) || b.fiftyMoves >= 100){
    return 0; //draw
  }

  if (b.ply > MAXDEPTH - 1){
    return evalPosition(b); //too deep
  }

  int score = evalPosition(b);

  if (score >= beta){
    return beta; //our position is already better than beta
  }

  if (score > alpha){
    alpha = score;
  }

  moveList* list = new moveList;
  generateAllCaptureMoves(b, list);

  int legal = 0;
  int oldAlpha = alpha;
  int bestMove = NOMOVE;
  score = -INFINITE;
  int bestPVMove = probePVT(b);
  if (bestPVMove != NOMOVE){
    for (int i=0; i<list->getCount(); ++i){
      if (list->ml_getMove(i) == bestPVMove){
        list->ml_setScore(i, 2000000);
        break;
      }
    }
  }

  for (int i=0; i<list->getCount(); ++i){
    pickNextMove(i, list);
    int move = list->ml_getMove(i);
    if (!makeMove(b, move)){
      continue;
    }
    legal++;
    score = -quiescenceSearch(-beta, -alpha, b, search); //negamax
    takeMove(b);
    if (search->stopped == true){
      return 0;
    }

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

  if (alpha != oldAlpha){
    storePVMove(b, bestMove);
  }

  delete list;
  return alpha;
}
