//search.h
#ifndef SEARCH
#define SEARCH

#include "board.h"
#include "enums.h"

class searchInfo{
public:
  int depth;
  double startTime;
  double stopTime;
  bool depthSet;
  bool timeSet;
  int movesToGo;
  bool infinite; //If infinite == true, don't stop searching

  long nodes; //count of all of the positions visited

  bool quit; //If quit == true, stop searching
  bool stopped; //If stopped = true, recursively back out of search

  float failHigh; //how good is our ordering
  float failHighFirst;

  int gameMode;
  bool postThinking;
};

bool isRepetition(const board& b);
void searchPosition(board& b, searchInfo* info);
void checkUp(searchInfo* info); //Checks if we should stop searching (timer up, etc).
void clearForSearch(board& b, searchInfo* info); //Clears heuristics
int alphaBetaSearch(int alpha, int beta, int depth, board& b, searchInfo* info, bool nullMove);
int quiescenceSearch(int alpha, int beta, board& b, searchInfo* info); //reduces horizon effect

#endif
