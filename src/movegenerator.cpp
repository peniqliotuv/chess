//movegenerator.cpp
#include <iostream>
#include "movegenerator.h"

/***** MACROS *****/
#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (fileArray[(sq)]==OFFBOARD)

/***** GLOBALS *****/
int slidingPieceArray[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
int slidingPieceSide[2] = {0, 4}; //used to index the sliding piece array
int nonSlidingPieceArray[6] = {wN, wK, 0, bN, bK, 0};
int nonSlidingPieceSide[2] = {0, 3}; // used to index the non sliding piece array
int pieceDir[13][8] = { //First index: piece | Second index: directions
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }
};
//How far to loop through for each piece in pieceDir
int numDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

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

  int sq, targetSq;
  int dir = 0;
  int pieceIndex = 0;
  int piece = EMPTY;

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

  //Sliding piece
  pieceIndex = slidingPieceSide[b.side];
  piece = slidingPieceArray[pieceIndex++];
  while (piece != 0){
    std::cout << "sliding index: " << pieceIndex << " piece: " << piece << std::endl;
    for (int i=0; i<b.numPieces[piece]; ++i){
      sq = b.pieceList[piece][i];
      if (!SqOnBoard(sq)){
        std::cout << "ERROR: SQUARE NOT ON BOARD" << std::endl;
      }
      std::cout << "Piece: " << pieceChar[piece] << " on square: " << printSquare(sq) << std::endl;
      for (int j=0; j<numDir[piece]; ++j){
        dir = pieceDir[piece][j];
        targetSq = sq + dir;
        while (!SQOFFBOARD(targetSq)){
        // BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK
          if (b.pieces[targetSq] != EMPTY){
            if (pieceColor[b.pieces[targetSq]] == (b.side ^ 1)){
              //Capture move
              std::cout << "\tCapture on square: " << printSquare(targetSq) << std::endl;
            }
            break;
          }
          std::cout << "\tNon-capture on square: " << printSquare(targetSq) << std::endl;
          targetSq += dir;
        }
      }
    }
    piece = slidingPieceArray[pieceIndex++];
  }

  //Non-sliding piece
  pieceIndex = nonSlidingPieceSide[b.side];
  piece = nonSlidingPieceArray[pieceIndex++];
  while (piece != 0){
    std::cout << "non-sliding index: " << pieceIndex << " piece: " << piece << std::endl;
    for (int i=0; i<b.numPieces[piece]; ++i){
      sq = b.pieceList[piece][i];
      if (!SqOnBoard(sq)){
        std::cout << "ERROR: SQUARE NOT ON BOARD" << std::endl;
      }
      std::cout << "Piece: " << pieceChar[piece] << " on square: " << printSquare(sq) << std::endl;
      for (int j=0; j<numDir[piece]; ++j){
        dir = pieceDir[piece][j];
        targetSq = sq + dir;
        if (SQOFFBOARD(targetSq)) continue;

        // BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK
        if (b.pieces[targetSq] != EMPTY){
          if (pieceColor[b.pieces[targetSq]] == (b.side ^ 1)){
            //Capture move
            std::cout << "\tCapture on square: " << printSquare(targetSq) << std::endl;
          }
          continue;
        }
        std::cout << "\tNon-capture on square: " << printSquare(targetSq) << std::endl;
      }
    }
    piece = nonSlidingPieceArray[pieceIndex++];
  }
}
