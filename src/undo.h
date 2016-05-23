//undo.h
#ifndef UNDO
#define UNDO

#include "enums.h"

struct undo{
  int move;
  int castlePermission;
  int enPassent;
  int fiftyMoves;
  U64 posKey;
};

#endif
