#ifndef BOARD
#define BOARD

#include "enums.h"
#include "undo.h"

struct board{
  int pieces[BOARD_SIZE];
  U64 pawns[3];
  int KingSquare[2];
  int side;
  int enPassent;
  int fiftyMoves;
  int ply;
  int plyHistory;

  U64 posKey;


  int numPieces[13]; //How many exist at this index in the board
  int numMajorPieces[3];
  int numMinorPieces[3];
  int numBigPieces[3];

  int castlePermission;

  undo history[MAX_GAME_MOVES];
};

#endif
