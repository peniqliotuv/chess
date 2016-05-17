#ifndef ENUMERATIONS
#define ENUMERATIONS
#include <cstdlib>

typedef unsigned long long U64;

//MACROS
#define BOARD_SIZE 120
#define MAX_GAME_MOVES 2048
#define toSquareNumber(row, file) ( (21 + (file) ) + ( (row) * 10 ))
#define clearBit(bb, sq) ((bb) &= clearMask[(sq)])
#define setBit(bb, sq) ((bb) |= setMask[(sq)])

//GLOBAL VARIABLES
extern int SQ120[BOARD_SIZE];
extern int SQ64[64];
extern U64 clearMask[64];
extern U64 setMask[64];
extern U64 pieceKeys[13][120];
extern U64 sideKey;
extern U64 castleKeys[16];

//ENUMERATIONS
enum {EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bW}; // already enumerated incrementally
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8, ROW_NONE};
enum {WHITE, BLACK, BOTH};
enum {whiteKingCastle = 1, whiteQueenCastle = 2, blackKingCastle = 4, blackQueenCastle = 8};
// Castle permission is going to be represented by bits
// 1 1 1 1 --> All four castling possibilities are allowed
enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 31, B3, C3, D3, E3, F3, G3, H3,
  A4 = 41, B4, C4, D4, E4, F4, G4, H4,
  A5 = 51, B5, C5, D5, E5, F5, G5, H5,
  A6 = 61, B6, C6, D6, E6, F6, G6, H6,
  A7 = 71, B7, C7, D7, E7, F7, G7, H7,
  A8 = 81, B8, C8, D8, E8, F8, G8, H8, NO_SQUARE, OFFBOARD
};

struct undo{
  int move;
  int castlePermission;
  int enPassent;
  int fiftyMoves;
  U64 posKey;
};

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
  int numMajorPieces[3];
  int numMinorPieces[3];
  int numBigPieces[3];

  int castlePermission;

  undo history[MAX_GAME_MOVES];
  //indexed by ply

  int pieceList[13][10];
  //13 total pieces, 10 possible of each piece
};


//FUNCTIONS
extern void printBitBoard(U64 bitBoard);
extern void initialize();
extern U64 generatePosKey(const board& b);
extern void resetBoard();

#endif
