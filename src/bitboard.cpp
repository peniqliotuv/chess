//bitboard.cpp
#include <iostream>
#include "enums.h"
#include "bitboard.h"

/***** TAKEN OFF OF THE CHESS PROGRAMMING WIKI *****/
const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

int popBit(U64 *bb) {
  U64 b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}

int countBits(U64 b) {
  int r;
  for(r = 0; b; r++, b &= b - 1);
  return r;
}
/***** TAKEN OFF OF THE CHESS PROGRAMMING WIKI *****/

void printBitBoard(U64 bitBoard){
  U64 bit = 1ULL;

  int row = 0;
  int file = 0;
  int sq120 = 0;
  int sq64 = 0;

  std::cout << std::endl;
  for (row = ROW_8; row >= ROW_1; --row){
    for (file = FILE_A; file <= FILE_H; ++file){
      sq120 = toSquareNumber(file, row); //120-Based
      sq64 = SQ120[sq120]; //64-Based
      if ((bit << sq64) & bitBoard){
        //Something exists there, print it.
        std::cout << "X";
      }
      else {
        //Nothing there.
        std::cout << "-";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
