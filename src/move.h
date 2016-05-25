//move.h
#ifndef MOVE
#define MOVE

#include "enums.h"
class move{
private:
  int move;
  int score;
public:
  void setMove(int m);
  void setScore(int s);
};

/* Use bitwise operations
0000 0000 0000 0000 0000 0111 1111 -> From (7 bits) 0x7F
0000 0000 0000 0011 1111 1000 0000 -> To (7 bits) >> 7, 0x7F
0000 0000 0011 1100 0000 0000 0000 -> What piece captured? (4 bits) >>14, 0x7F
0000 0000 0100 0000 0000 0000 0000 -> enPassent? (1 bit) 0x40000
0000 0000 1000 0000 0000 0000 0000 -> Pawn Start (1 bit) 0x80000
0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece (4 bits) >> 20, 0x7F
0001 0000 0000 0000 0000 0000 0000 -> Castled? (1 bit) 0x1000000
*/

#endif
