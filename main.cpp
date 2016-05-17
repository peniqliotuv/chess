#include <iostream>
#include "enums.h"
#include "bitboard.h"

using namespace std;

int main(){
  initialize();

  cout << "Printing Bit Board!" << endl;
  U64 bitboard = 0ULL;

  for (int i=0; i<64; i++){
    cout << "Index: " << i << endl;
    printBitBoard(setMask[i]);
    cout << endl;
  }

  return 0;
}
