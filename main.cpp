#include <iostream>
#include "board.h"
#include "enums.h"
#include "undo.h"
#include "bitboard.h"


using namespace std;

int main(){
  initialize();

  cout << "Printing Bit Board!" << endl;
  U64 bitboard = 0ULL;

  bitboard |= (1ULL << SQ120[D5]);
  printBitBoard(bitboard);

  cout << "Number of Bits: " << countBits(bitboard) << endl;



  return 0;
}
