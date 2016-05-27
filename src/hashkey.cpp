//hashkey.cpp
#include "hashkey.h"
#include <cstdlib>

U64 generatePosKey(const board& b){
  int piece = EMPTY;
  U64 finalKey = 0;
  for (int i=0; i<BOARD_SIZE; i++){
    piece = b.pieces[i];
    if (piece != NO_SQUARE && piece != EMPTY){
      finalKey ^= pieceKeys[piece][i];
    }
  }
  if (b.side == WHITE){
    finalKey ^= sideKey;
  }
  if (b.enPassent != NO_SQUARE){
    finalKey ^= pieceKeys[EMPTY][b.enPassent];
  }
  finalKey ^= castleKeys[b.castlePermission];

  return finalKey;
}

void hashCastleKey(board& b){
  b.posKey ^= castlePermArray[(b.castlePermission)];
}

void hashPieceKey(board& b, int piece, int sq){
  b.posKey ^= pieceKeys[(piece)][(sq)];
}

void hashSideKey(board& b){
  b.posKey ^= sideKey;
}

void hashEnPasKey(board& b){
  b.posKey ^= pieceKeys[EMPTY][(b.enPassent)];
}
