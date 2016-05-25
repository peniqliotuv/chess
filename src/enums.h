//enums.h
#ifndef ENUMERATIONS
#define ENUMERATIONS
#include <string>
#include <cstdlib>
#include <exception>

/***** TYPE DEFINITIONS *****/
typedef unsigned long long U64; //for bitboards

/***** EXCEPTIONS *****/
class castlePermBounds : public std::exception{
  virtual const char* what() const throw(){
    return "Castle Permission is out of bounds";
  }
};

class pieceListException : public std::exception{
  virtual const char* what() const throw(){
    return "Piece lists do not match the board.";
  }
};

/***** MACROS *****/
#define BOARD_SIZE 120
#define MAX_GAME_MOVES 2048
#define toSquareNumber(file, row) ( (21 + (file) ) + ( (row) * 10 ))
#define clearBit(bb, sq) ((bb) &= clearMask[(sq)]) //for bitboards
#define setBit(bb, sq) ((bb) |= setMask[(sq)])
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define FROMSQ(m) ( (m) & 0x7F);
#define TOSQ(m) ( ( (m) >> 7) & 0x7F);
#define CAPTURED(m) ( ( (m) >> 14) & 0xF); //what piece was captured
#define PROMOTED(m) ( ( (m) >> 20) & 0xF);

#define EPFLAG 0x40000
#define PAWNFLAG 0x80000
#define CASTLEFLAG 0x1000000

#define ISCAPTURE 0x7C000 //is it a capturing move?
#define ISPROMOTION 0xF00000 //is it a promoting move?

/***** GLOBALS *****/
//init.cpp
extern int SQ120[BOARD_SIZE];
extern int SQ64[64];
extern U64 clearMask[64];
extern U64 setMask[64];
extern U64 pieceKeys[13][120];
extern U64 sideKey;
extern U64 castleKeys[16];
extern int fileArray[BOARD_SIZE];
extern int rowArray[BOARD_SIZE];
//board.cpp
extern char pieceChar[];
extern char sideChar[];
extern char rowChar[];
extern char fileChar[];
//pieces.cpp
extern int isBig[13];
extern int isMajor[13];
extern int isMinor[13];
extern int pieceValue[13];
extern int pieceColor[13];
//threats.cpp
extern const int knightMove[8];
extern const int rookMove[4];
extern const int bishopMove[4];
extern const int kingMove[8];

/***** ENUMERATIONS *****/
enum {EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};
enum FILE_ENUM {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum ROW_ENUM {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8, ROW_NONE};
enum {WHITE, BLACK, BOTH};
enum {whiteKingCastle = 1, whiteQueenCastle = 2, blackKingCastle = 4, blackQueenCastle = 8};
// Castle permission is going to be represented by bits
// 1 1 1 1 --> All four castling possibilities are allowed
enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQUARE, OFFBOARD
};

/***** FUNCTIONS *****/
//extern void printBitBoard(U64 bitBoard);
//extern void initialize();
//extern U64 generatePosKey(const board& b);
//extern void resetBoard();
//extern void printBoard(const board& b);
//extern void updateMateriaList(board& b);
//extern int checkBoard(const board& b);
//extern int countBits(U64 b);
//extern int popBit(U64 *bb);

#endif
