//main.cpp
#include <iostream>
#include "enums.h"
#include "bitboard.h"
#include "board.h"
#include "undo.h"
#include "test.h"
#include "movegenerator.h"
#include "makemove.h"
#include "init.h"
#include "search.h"
#include "io.h"
#include "threats.h"

//FOR TESTING PURPOSES
#define MATE_IN_THREE "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define TESTFEN "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

using namespace std;

int main(){
  initialize();
  board* b = new board;
  initPVT(b->PVT);
  searchInfo* search = new searchInfo;
	parseFen(TESTFEN, *b);
  char io[6];
  int mv = NOMOVE;
  while (1){
    printBoard(*b);
    cout << "Please enter a move. " << std::endl;
    cin.getline(io, 6);
    if (!cin.fail()){
      if (io[0] == 'q') break;
      else if (io[0] == 't') {
        takeMove(*b);
        continue;
      }
      else if (io[0] == 's'){
        search->depth = 4;
        searchPosition(*b, search);
      }
      else {
        mv = parseMove(io, *b);
        if (mv != NOMOVE){
          storePVMove(*b, mv);
          makeMove(*b, mv);
          if (isRepetition(*b)) cout << "REPETITION" << endl;
        }
        else {
          cout << "Failed to parse move" << endl;
        }
      }
    }
    else if (cin.fail()){
      cin.clear();
    }
  }

  //delete list;
  delete b;
  delete search;
  return 0;
}
