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
#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FEN_5 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "
#define PAWN_TEST "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWN_TEST2 "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1 "
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 w - - 0 1 "
#define CASTLE1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define CASTLE2 "3rk2r/8/8/8/8/8/6p1/R3K2R w KQk - 0 1"
#define CASTLE3 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define PERFTFEN2 "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"

using namespace std;

int main(){
  initialize();
  board* b = new board;

	parseFen(START_FEN, *b);
  char io[6];
  int mv = NOMOVE;
  int max = 0;
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
      else if (io[0] == 'p'){
        //perfTest(4, *b);
        max = getPVLine(4, *b);
        cout << "PV Line of " << max << " moves" << endl;
        for (int i=0; i< max; ++i){
          mv = b->PVArray[i];
          cout << printMove(mv) << endl;
        }
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
	/*printBoard(*b);

	moveList* list = new moveList;
	generateAllMoves(*b, list);
	printMoveList(*list);
  perfTest(4, *b);*/

  //delete list;
  delete b;
  return 0;
}
