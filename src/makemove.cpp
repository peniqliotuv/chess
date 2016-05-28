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

void addPiece(const int sq, board& b, int piece){
  int color = pieceColor[piece];

  hashPieceKey(b, piece, sq);
  b.pieces[sq] = piece;

  if (isBig[piece]){
    b.numBigPieces[piece]++;
    if (isMajor[piece]){
      b.numMajorPieces[piece]++;
    }
    else{
      b.numMinorPieces[piece]++;
    }
  }
  else{
    setBit(b.pawns[color], SQ120[sq]);
    setBit(b.pawns[BOTH], SQ120[sq]);
  }

  b.materialValue[color] -= pieceValue[piece];
  b.pieceList[piece][b.numPieces[piece]] = sq;
  b.numPieces[piece]++;
}

void movePiece(const int from, const int to, board& b){
  bool foundPiece = false; //for sanity checking

  int piece = b.pieces[from];
  int color = pieceColor[piece];

  hashPieceKey(b, piece, from);
  b.pieces[from] = EMPTY;
  hashPieceKey(b, piece, to);
  b.pieces[from] = piece;

  if (!isBig[piece]){ //if it's a pawn
    clearBit(b.pawns[color], SQ120[from]);
    clearBit(b.pawns[BOTH], SQ120[from]);
    setBit(b.pawns[color], SQ120[to]);
    setBit(b.pawns[BOTH], SQ120[to]);
  }

  for (int i=0; i<b.numPieces[piece]; ++i){
    if (b.pieceList[piece][i] == from){
      b.pieceList[piece][i] = true;
      foundPiece = true;
      break;
    }
  }

  if (!foundPiece){
    std::cout << "error: piece not found. cannot move" << std::endl;
  }
}
