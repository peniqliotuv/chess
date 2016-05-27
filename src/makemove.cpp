//makemove.cpp
#include "makemove.h"
#include "hashkey.h"
#include <iostream>


//castle permission represented by 4 bits, 15 == 1 1 1 1
//will be bitwise AND-ing with the array

const int castlePermArray[120] = {
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

void clearPiece(const int sq, board& b){
  int piece = b.pieces[sq];
  int color = pieceColor[piece];
  int tempPieceNum = -1;

  hashPieceKey(b, piece, sq);

  b.pieces[sq] = EMPTY;
  b.materialValue[color] -= pieceValue[piece];

  if (isBig[piece]){
    b.numBigPieces[color]--;
    if (isMajor[piece]){
      b.numMajorPieces[color]--;
    }
    else if (isMinor[piece]){
      b.numMinorPieces[color]--;
    }
  }
  else {
    clearBit(b.pawns[color], SQ120[sq]);
    clearBit(b.pawns[BOTH], SQ120[sq]);
  }

  for (int i=0; i<b.numPieces[piece]; ++i){
    if (b.pieceList[piece][i] == sq){
      tempPieceNum = sq;
      break;
    }
  }
  if (tempPieceNum == -1){
    std::cout << "Error" << std::endl;
  }
  b.numPieces[piece]--;
  b.pieceList[piece][tempPieceNum] = b.pieceList[piece][b.numPieces[piece]];
}
