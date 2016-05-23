//init.cpp
#include "init.h"

//Macros
#define RAND_64 (	(U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60    )

//Data Members:
int fileArray[BOARD_SIZE];
int rowArray[BOARD_SIZE];
int SQ120[BOARD_SIZE];
int SQ64[64];

U64 setMask[64];
U64 clearMask[64];
U64 sideKey;
U64 pieceKeys[13][120]; //indexed by pieces and by square
U64 castleKeys[16]; //0 0 0 0 -> 2^4 castle keys

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
      square120 = toSquareNumber(file, row);
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

void initHashKeys(){
  for (int i=0; i<13; i++){
    for (int j=0; j<120; j++){
      pieceKeys[i][j] = RAND_64;
    }
  }
  for (int i=0; i<16; i++){
    castleKeys[i] = RAND_64;
  }
  sideKey = RAND_64;
}

void initFileRow(){
	int file = FILE_A;
	int row = ROW_1;
	int sq = A1;
	//int sq64 = 0;
	for (int i=0; i<BOARD_SIZE; ++i){
		fileArray[i] = OFFBOARD;
		rowArray[i] = OFFBOARD;
	}
	for (row = ROW_1; row <= ROW_8; ++row){
		for (file = FILE_A; file <= FILE_H; ++file){
			sq = toSquareNumber(file, row);
			fileArray[sq] = file;
			rowArray[sq] = row;
		}
	}
}

void initialize(){
  initBoard();
  initBitMasks();
  initHashKeys();
	initFileRow();
}
