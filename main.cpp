#include <iostream>
#include "enums.h"
#include "bitboard.h"
#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

using namespace std;

int main(){
  initialize();
  board* b = new board;
  parseFen(FEN_4, *b);
  printBoard(*b);
  std::cout<<"WHITE PAWNS: " << std::endl;
  printBitBoard(b->pawns[WHITE]);
  std::cout<<"BLACK PAWNS: " << std::endl;
  printBitBoard(b->pawns[BLACK]);
  std::cout<<"BOTH PAWNS: " << std::endl;
  printBitBoard(b->pawns[BOTH]);
  checkBoard(*b);



/*  board* b = new board;
  parseFen(START_FEN, *b);
  printBoard(*b);
  parseFen(FEN_1, *b);
  printBoard(*b);
  parseFen(FEN_2, *b);
  printBoard(*b);
*/

  return 0;
}
