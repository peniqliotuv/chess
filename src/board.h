//board.h
#ifndef BOARD
#define BOARD

#include "enums.h"
#include "pvt.h"
#include "undo.h"

struct board{
  /***** Data Members*****/
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

  PVTable* PVT;

  /***** Methods *****/
  //Mutators for Undo Struct
  void setUndoPosKey();
  void setUndoMove(int move);
  void setUndoFiftyMove();
  void setUndoEnPassent();
  void setUndoCastlePerm();
  //Accessors for Undo struct
  int getPrevMove();
  int getPrevCastlePerm();
  int getPrevFiftyMove();
  int getPrevEnPassent();
  U64 getPrevPosKey(int index) const;
};

void resetBoard(board &b);
void updateMateriaList(board &b);
//Checks if board is valid
int checkBoard(const board& b);
int parseFen(char* fen, board& b);
void printBoard(const board& b);
//PVT Functions
void storePVMove(const board& b, const int move);
int probePVT(const board& b);

#endif
