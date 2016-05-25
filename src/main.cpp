//main.cpp
#include <iostream>
#include "enums.h"
#include "bitboard.h"
#include "board.h"
#include "undo.h"
#include "movegenerator.h"
#include "init.h"
#include "io.h"
#include "threats.h"

//FOR TESTING PURPOSES
#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FEN_5 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "
#define PAWN_TEST "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

using namespace std;

int main(){
  initialize();
  board* b = new board;
	parseFen(PAWN_TEST, *b);
	printBoard(*b);
	moveList* list = new moveList;
	generateAllMoves(*b, list);
	printMoveList(*list);
	/*
  int move = 0;
	int from = A2; int to = H7;
	int cap = wR; int prom = bQ;

	move = ( ( from ) | ( to << 7 ) | ( cap << 14 ) | ( prom << 20) );

	printf("\ndec:%d hex:%X\n",move,move);

  cout << "From: " << FROMSQ(move);
  cout << endl << "To: " << TOSQ(move);
  cout << endl <<  "Captured: " << CAPTURED(move);
  cout << endl <<  "Promoted:" << PROMOTED(move);
	cout << endl;
	cout << "from: " << printSquare(from) << endl;
	cout << "to: " << printSquare(to) << endl;
	cout << "move: " << printMove(move) << endl;*/

  return 0;
}
