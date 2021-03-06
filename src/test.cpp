//test.cpp
#include "test.h"
#include "enums.h"
#include "movelist.h"
#include "board.h"
#include "io.h"
#include "makemove.h"
#include "movegenerator.h"

#include <iostream>
#include <ctime>

long leafNodes;

void test(int depth, board& b){
  if (checkBoard(b) != 1) std::cout << "test failed " << std::endl;
  if (depth == 0){
    leafNodes++;
    return;
  }
  moveList* list = new moveList;
  generateAllMoves(b, list);

  for (int i=0; i<list->getCount(); ++i){
    if (!makeMove(b, list->ml_getMove(i))){
      continue;
    }
    test(depth-1, b);
    takeMove(b);
  }
  delete list;
}

void perfTest(int depth, board& b){
  if (checkBoard(b) != 1) std::cout << "test failed " << std::endl;
  printBoard(b);
  std::cout << "*****PERFT TESTING*****" << std::endl;
  std::cout << "Testing to depth of: " << depth << std::endl;
  leafNodes = 0;
  moveList* list = new moveList;

  generateAllMoves(b, list);

  clock_t start;
  double duration;
  start = clock();

  int move;
  std::cout << "Total Moves: " << std::dec << list->getCount() << std::endl;

  for (int i=0; i<list->getCount(); ++i){
    move = list->ml_getMove(i);
    if (!makeMove(b, move)){
      continue;
    }
    long totalNodes = leafNodes;
    test(depth-1, b);
    takeMove(b);
    long oldNodes = leafNodes - totalNodes;
    std::cout << "Move: " << (i+1) << " : " << printMove(move) << " : " << oldNodes << std::endl;
  }
  duration = 1000 * ((clock() - start) / (double) CLOCKS_PER_SEC);
  std::cout << "Testing complete. " << std::dec << leafNodes <<
  " total nodes visited in: " << duration << "ms" << std::endl;
  delete list;
}
