//movegenerator.cpp
#include <iostream>
#include "movegenerator.h"
/***** MACROS *****/
#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (fileArray[(sq)]==OFFBOARD)

void addQuietMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);
  list->ml_setScore(list->getCount(), 0);
  list->incrementCount();
}

void addCaptureMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);
  list->ml_setScore(list->getCount(), 0);
  list->incrementCount();
}

void addWPCaptureMove(const board& b, const int from, const int to, const int cap, moveList* list){
  if (rowArray[from] == ROW_7){ //Can Promote
    addCaptureMove(b, MOVE(from, to, cap, wQ, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, wR, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, wB, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, wN, 0), list);
  }
  else{
    addCaptureMove(b, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

void addWPMove(const board& b, const int from, const int to, moveList* list){
  if (rowArray[from] == ROW_7){ //Can Promote
    addQuietMove(b, MOVE(from, to, EMPTY, wQ, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, wR, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, wB, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, wN, 0), list);
  }
  else{
    addQuietMove(b, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void addBPCaptureMove(const board& b, const int from, const int to, const int cap, moveList* list){
  if (rowArray[from] == ROW_2){ //Can Promote
    addCaptureMove(b, MOVE(from, to, cap, bQ, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, bR, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, bB, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, bN, 0), list);
  }
  else{
    addCaptureMove(b, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

void addBPMove(const board& b, const int from, const int to, moveList* list){
  if (rowArray[from] == ROW_2){ //Can Promote
    addQuietMove(b, MOVE(from, to, EMPTY, bQ, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, bR, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, bB, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, bN, 0), list);
  }
  else{
    addQuietMove(b, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void addEnPasMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);
  list->ml_setScore(list->getCount(), 0);
  list->incrementCount();
}

void generateAllMoves(const board& b, moveList* list){
  list->setCount(0);
  if (!checkBoard(b)){
    std::cout << "Error: board is incorrect" << std::endl;
  }

  int sq;

  if (b.side == WHITE){
    for (int i=0; i<b.numPieces[wP]; ++i){
      sq = b.pieceList[wP][i];
      if (!SqOnBoard(sq)){
        std::cout << "Error: Square is not on board" << std::endl;
      }
      if (b.pieces[sq+10] == EMPTY){
        addWPMove(b, sq, (sq+10), list);
        if (rowArray[sq] == ROW_2 && b.pieces[sq+20] == EMPTY){
          addQuietMove(b, MOVE(sq, (sq+20), EMPTY, EMPTY, PAWNFLAG), list);
        }
      }
      if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq+9]] == BLACK){
        addWPCaptureMove(b, sq, sq+9, b.pieces[sq+9], list);
      }
      if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq+11]] == BLACK){
        addWPCaptureMove(b, sq, sq+11, b.pieces[sq+11], list);
      }
      if ((sq+9) == b.enPassent){
        addCaptureMove(b, MOVE(sq, (sq+9), EMPTY, EMPTY, EPFLAG), list);
      }
      if ((sq+11) == b.enPassent){
        addCaptureMove(b, MOVE(sq, (sq+11), EMPTY, EMPTY, EPFLAG), list);
      }
    }
  }
  else if (b.side == BLACK){
    for (int i=0; i<b.numPieces[bP]; ++i){
      sq = b.pieceList[bP][i];
      if (!SqOnBoard(sq)){
        std::cout << "Error: Square is not on board" << std::endl;
      }
      if (b.pieces[sq-10] == EMPTY){
        addBPMove(b, sq, (sq-10), list);
        if (rowArray[sq] == ROW_7 && b.pieces[sq-20] == EMPTY){
          addQuietMove(b, MOVE(sq, (sq-20), EMPTY, EMPTY, PAWNFLAG), list);
        }
      }
      if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq-9]] == WHITE){
        addBPCaptureMove(b, sq, sq-9, b.pieces[sq-9], list);
      }
      if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq-11]] == WHITE){
        addBPCaptureMove(b, sq, sq-11, b.pieces[sq-11], list);
      }
      if ((sq-9) == b.enPassent){
        addCaptureMove(b, MOVE(sq, (sq-9), EMPTY, EMPTY, EPFLAG), list);
      }
      if ((sq-11) == b.enPassent){
        addCaptureMove(b, MOVE(sq, (sq-11), EMPTY, EMPTY, EPFLAG), list);
      }
    }
  }
}
