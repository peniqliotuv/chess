//hashkey.cpp
#include "hashkey.h"
#include <iostream>
#include <cstdlib>

U64 generatePosKey(const board& b){
  int piece = EMPTY;
  U64 finalKey = 0;

  for (int i=0; i<BOARD_SIZE; ++i){
    piece = b.pieces[i];
    if (piece != NO_SQUARE && piece != EMPTY && piece != OFFBOARD){
      if (piece < wP || piece > bK) std::cout << "err" << std::endl;
      finalKey ^= pieceKeys[piece][i];
    }
  }

  if (b.side == WHITE){
    finalKey ^= sideKey;
  }

  if (b.enPassent != NO_SQUARE){
    if (b.enPassent < 0 || b.enPassent > BOARD_SIZE) std::cout << "err" << std::endl;
    finalKey ^= pieceKeys[EMPTY][b.enPassent];
  }

  if (b.castlePermission < 0 || b.castlePermission > 15) std::cout << "err" << std::endl;
  finalKey ^= castleKeys[b.castlePermission];

  return finalKey;
}

void hashCastleKey(board& b){
  b.posKey ^= castleKeys[(b.castlePermission)];
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
