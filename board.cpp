#include <cstdlib>
#include "enums.h"

//Resets the board to empty
void resetBoard(board &b){
  for (int i=0; i<BOARD_SIZE; i++){
    b.pieces[i] = OFFBOARD;
  }
  for (int i=0; i<64; i++){
    b.pieces[SQ64[i]] = EMPTY;
  }
  for (int i=0; i<3; i++){
    b.numMajorPieces[i] = 0;
    b.numMinorPieces[i] = 0;
    b.numBigPieces[i] = 0;
    b.pawns[i] = 0ULL;
  }
  for (int i=0; i<13; i++){
    b.numPieces[i] = 0;
  }

  b.kingSquare[0] = b.kingSquare[1] = NO_SQUARE;
  b.side = BOTH;
  b.enPassent = NO_SQUARE;
  b.castlePermission = 0;
  b.ply = 0;
  b.plyHistory = 0;
  b.posKey = 0ULL;
  b.fiftyMoves = 0;
  
}
