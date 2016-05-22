#include "enums.h"

const int knightMove[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int rookMove[4] = {-1, -10, 1, 10};
const int bishopMove[4] = {-9, -11, 11, 9};
const int kingMove[8] = {-1, -10, 1, 10, -9, -11, 11, 9};
//All of the directions that these pieces can be located
//relative to a given square in order to be considered
//"threatening" it
bool isKnight(int p){
  if (p == wN || p == bN) return true;
  else return false;
}
bool isRook(int p){
  if (p == wR || p == bR) return true;
  else return false;
}
bool isBishop(int p){
  if (p == wB || p == bB) return true;
  else return false;
}
bool isKing(int p){
  if (p == wK || p == bK) return true;
  else return false;
}

int sqAttacked(const int sq, const int side, const board& b){
  int tempPiece, tempSq, dir;
  //Pawns
  if (side == WHITE){
    if (b.pieces[sq-11] == wP || b.pieces[sq-9] == wP) return true;
  }
  else {
    if (b.pieces[sq-11] == bP || b.pieces[sq-9] == bP) return true;
  }
}
