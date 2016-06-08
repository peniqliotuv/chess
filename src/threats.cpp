//threats.cpp
#include "enums.h"
#include "board.h"
#include "threats.h"
#include "validate.h"

#include <iostream>

const int knightMove[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int rookMove[4] = {-1, -10, 1, 10};
const int bishopMove[4] = {-9, -11, 11, 9};
const int kingMove[8] = {-1, -10, 1, 10, -9, -11, 11, 9};
//All of the directions that these pieces can be located
//relative to a given square in order to be considered
//"threatening" it

bool isPawn(int p){
  if (p == wP || p == bP) return true;
  else return false;
}
bool isKnight(int p){
  if (p == wN || p == bN) return true;
  else return false;
}
bool isRookQueen(int p){
  if (p == wR || p == bR || p == wQ || p == bQ) return true;
  else return false;
}
bool isBishopQueen(int p){
  if (p == wB || p == bB || p == wQ || p == bQ) return true;
  else return false;
}
bool isKing(int p){
  if (p == wK || p == bK) return true;
  else return false;
}

bool sqAttacked(const int sq, const int side, const board& b){
  int tempPiece, tempSq, dir;

  if (!SqOnBoard(sq)){
    std::cout << "Square is not on board!" << std::endl;
  }
  if (!SideValid(side)){
    std::cout << "Side is not valid!" << std::endl;
  }

  //Pawns
  if (side == WHITE){
    if (b.pieces[sq-11] == wP || b.pieces[sq-9] == wP) return true;
  }
  else {
    if (b.pieces[sq+11] == bP || b.pieces[sq+9] == bP) return true;
  }
  //Knights
  for (int i=0; i<8; ++i){
    tempPiece = b.pieces[sq + knightMove[i]];
    if (isKnight(tempPiece) && pieceColor[tempPiece] == side) return true;
  }
  //Rooks and Queens
  for (int i=0; i<4; ++i){
    dir = rookMove[i];
    tempSq = sq + dir;
    tempPiece = b.pieces[tempSq];
    while (tempPiece != OFFBOARD){
      if (tempPiece != EMPTY){
        if (isRookQueen(tempPiece) && pieceColor[tempPiece] == side) return true;
        break;
      }
      tempSq += dir;
      tempPiece += b.pieces[tempSq];
    }
  }
  //Bishops and Queens
  for (int i=0; i<4; ++i){
    dir = bishopMove[i];
    tempSq = sq + dir;
    tempPiece = b.pieces[tempSq];
    while (tempPiece != OFFBOARD){
      if (tempPiece != EMPTY){
        if (isBishopQueen(tempPiece) && pieceColor[tempPiece] == side) return true;
        break;
      }
      tempSq += dir;
      tempPiece += b.pieces[tempSq];
    }
  }
  //Kings
  for (int i=0; i<8; ++i){
    tempPiece = b.pieces[sq + kingMove[i]];
    if (isKing(tempPiece) && pieceColor[tempPiece] == side) return true;
  }

  return false;
}
