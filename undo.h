#ifndef UNDO_STRUCT
#define UNDO_STRUCT

#include "enums.h"

struct undo{
  int move;
  int castlePermission;
  int enPassent;
  int fiftyMoves;
  U64 posKey;
};

#endif
