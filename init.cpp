#include "enums.h"

int SQ120[BOARD_SIZE];
int SQ64[64];

U64 setMask[64];
U64 clearMask[64];

void initBoard(){
  int file = FILE_A;
  int row = ROW_1;
  int square120 = A1;
  int square64 = 0;
  for (int i = 0; i<BOARD_SIZE; i++){
    SQ120[i] = 65;
    //because 65 should never be returned as a value
  }
  for (int i = 0; i < 64; i++){
    SQ64[i] = 120;
    // because 120 should never be returned as a value
  }
  for (row = ROW_1; row <= ROW_8; row++){
    for (file = FILE_A; file <= FILE_H; file++){
      square120 = toSquareNumber(row, file);
      SQ64[square64] = square120;
      SQ120[square120] = square64;
      square64++;
    }
  }
}

void initBitMasks(){
  for (int i= 0; i < 64; i++){
    setMask[i] = 0ULL;
    clearMask[i] = 0ULL;
  }

  for (int i = 0; i < 64; i++){
    setMask[i] |= (1ULL << i);
    clearMask[i] = ~setMask[i];
  }

}

void initialize(){
  initBoard();
  initBitMasks();
}
