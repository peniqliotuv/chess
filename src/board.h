//board.h
#ifndef BOARD
#define BOARD

#include "enums.h"
#include "undo.h"

struct board{
  int pieces[BOARD_SIZE];
  U64 pawns[3];
  int kingSquare[2];
  int side;
  int enPassent;
  int fiftyMoves;
  int ply;
  int plyHistory;
  U64 posKey;
  int numPieces[13]; //How many exist at this index in the board
  int numMajorPieces[2];
  int numMinorPieces[2];
  int numBigPieces[2];
  int materialValue[2]; //Total material value of each player
  int castlePermission; //WKCA, WQCA, BKCA, BQCA

  undo history[MAX_GAME_MOVES]; //indexed by ply
  int pieceList[13][10]; //13 total pieces, 10 possible of each piece
};

void resetBoard(board &b);
void updateMateriaList(board &b);
int checkBoard(const board& b);
int parseFen(char* fen, board& b);
void printBoard(const board& b);

#endif