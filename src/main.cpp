//main.cpp
#include <iostream>
#include "enums.h"
#include "bitboard.h"
#include "board.h"
#include "undo.h"
#include "init.h"
#include "threats.h"

#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FEN_5 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "

using namespace std;
void PrintBin(int move) {

	int index = 0;
	printf("As binary:\n");
	for(index = 27; index >= 0; index--) {
		if( (1<<index) & move) printf("1");
		else printf("0");
		if(index!=28 && index%4==0) printf(" ");
	}
	printf("\n");
}

int main(){
  initialize();
  board* b = new board;
  parseFen(FEN_5, *b);
  printBoard(*b);

  int move = 0;
	int from = 6; int to = 12;
	int cap = wR; int prom = bR;

	move = ( ( from ) | ( to << 7 ) | ( cap << 14 ) | ( prom << 20) );

	printf("\ndec:%d hex:%X\n",move,move);
	PrintBin(move);
  cout << "From: " << FROMSQ(move);
  cout << endl << "To: " << TOSQ(move);
  cout << endl <<  "Captured: " << CAPTURED(move);
  cout << endl <<  "Promoted:" << PROMOTED(move);
  return 0;
}
