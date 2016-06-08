//undo.cpp
#include "undo.h"

void undo::setPosKey(U64 key){
  posKey = key;
}

void undo::setMove(int mv){
  move = mv;
}

void undo::setFiftyMove(int fifty){
  fiftyMoves = fifty;
}

void undo::setEnPassent(int ep){
  enPassent = ep;
}

void undo::setCastlePerm(int cp){
  castlePermission = cp;
}
