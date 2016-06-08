//test.cpp
#include "test.h"
#include "enums.h"
#include "movelist.h"
#include "board.h"
#include "io.h"
#include "makemove.h"
#include "movegenerator.h"

#include <iostream>

long leafNodes;

void test(int depth, board& b){
  if (checkBoard(b) != 1) std::cout << "test failed " << std::endl;
  if (depth == 0){
    leafNodes++;
    return;
  }
  moveList* list = new moveList;
  generateAllMoves(b, list);
  int totalMoves = list->getCount();

  for (int i=0; i<totalMoves; ++i){
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
  std::cout << "Testing to depth of: " << depth << std::endl;
  leafNodes = 0;
  moveList* list = new moveList;

  generateAllMoves(b, list);

  int move;
  std::cout << "total moves: " << std::dec << list->getCount() << std::endl;

  for (int i=0; i<list->getCount(); ++i){
    move = list->ml_getMove(i);
    //std::cout << printMove(move) << std::endl;
    //std::cout << makeMove(b, move) << std::endl;
    if (!makeMove(b, list->ml_getMove(i))){
      continue;
    }
    long totalNodes = leafNodes;
    test(depth-1, b);
    takeMove(b);
    long oldNodes = leafNodes - totalNodes;
    std::cout << "Move: " << (i+1) << " : " << printMove(move) << " : " << oldNodes << std::endl;
  }
  std::cout << "Testing complete. " << leafNodes << " total nodes visited." << std::endl;
  delete list;
}
