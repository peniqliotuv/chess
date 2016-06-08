//undo.h
#ifndef UNDO
#define UNDO

#include "enums.h"

struct undo{
  //Members
  int move;
  int castlePermission;
  int enPassent;
  int fiftyMoves;
  U64 posKey;
  //Methods
  void setPosKey(U64 key);
  void setMove(int mv);
  void setFiftyMove(int fifty);
  void setEnPassent(int ep);
  void setCastlePerm(int cp);
};

#endif
