#ifndef _DEF_
#define _DEF_

#define BOARD_SIZE 120


typedef unsigned long long U64;

enum {EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bW}; // already enumerated incrementally
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {ROW_A, ROW_B, ROW_C, ROW_D, ROW_E, ROW_F, ROW_G, ROW_H, ROW_NONE};

enum {WHITE, BLACK, BOTH};

enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 31, B3, C3, D3, E3, F3, G3, H3,
  A4 = 41, B4, C4, D4, E4, F4, G4, H4,
  A5 = 51, B5, C5, D5, E5, F5, G5, H5,
  A6 = 61, B6, C6, D6, E6, F6, G6, H6,
  A7 = 71, B7, C7, D7, E7, F7, G7, H7,
  A8 = 81, B8, C8, D8, E8, F8, G8, H8, NO_SQUARE
}


#endif
