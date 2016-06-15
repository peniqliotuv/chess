//movegenerator.cpp
#include <iostream>
#include "threats.h"
#include "movelist.h"
#include "validate.h"
#include "movegenerator.h"
#include "move.h"
#include "makemove.h"

/***** MACROS *****/
#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (fileArray[(sq)]==OFFBOARD)

/***** GLOBALS *****/
const int slidingPieceArray[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
const int slidingPieceSide[2] = {0, 4}; //used to index the sliding piece array
const int nonSlidingPieceArray[6] = {wN, wK, 0, bN, bK, 0};
const int nonSlidingPieceSide[2] = {0, 3}; // used to index the non sliding piece array
const int pieceDir[13][8] = { //First index: piece | Second index: directions
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
const int numDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};
const int victimScore[13] = {0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};
int MVV_LVA_Scores[13][13]; // P x Q, P X R, P X B ..... N X Q, N X R

void addQuietMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);

	if (b.searchKillers[0][b.ply] == move){
		list->ml_setScore(list->getCount(), 900000); //Less than 1 million
	}
	else if (b.searchKillers[1][b.ply] == move){
		list->ml_setScore(list->getCount(), 800000);
	}
	else {
		list->ml_setScore(list->getCount(), b.searchHistory[b.pieces[FROMSQ(move)]][TOSQ(move)]);
	}

  list->incrementCount();
}

void addCaptureMove(const board& b, int move, moveList* list){
  list->ml_setMove(list->getCount(), move);
  list->ml_setScore(list->getCount(), MVV_LVA_Scores[CAPTURED(move)][b.pieces[FROMSQ(move)]] + 1000000);
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
  list->ml_setScore(list->getCount(), 1000105); // P x P = 105, + 1,000,000
  list->incrementCount();
}

void generateAllMoves(const board& b, moveList* list){
  list->setCount(0);
  //if (!checkBoard(b)){
  //  std::cout << "Error: board is incorrect" << std::endl;
  //}

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
			if (b.enPassent != NO_SQUARE){
	      if ((sq+9) == b.enPassent){
	        addEnPasMove(b, MOVE(sq, (sq+9), EMPTY, EMPTY, EPFLAG), list);
	      }
	      if ((sq+11) == b.enPassent){
	        addEnPasMove(b, MOVE(sq, (sq+11), EMPTY, EMPTY, EPFLAG), list);
	      }
			}
    }
    if (b.castlePermission & whiteKingCastle){
      if (b.pieces[G1] == EMPTY && b.pieces[F1] == EMPTY){
        if (!sqAttacked(F1, BLACK, b) && !sqAttacked(E1, BLACK, b)){
          addQuietMove(b, MOVE(E1, G1, EMPTY, EMPTY, CASTLEFLAG), list);
        }
      }
    }
    if (b.castlePermission & whiteQueenCastle){
      if (b.pieces[B1] == EMPTY && b.pieces[C1] == EMPTY && b.pieces[D1] == EMPTY){
        if (!sqAttacked(D1, BLACK, b) && !sqAttacked(E1, BLACK, b)){
          addQuietMove(b, MOVE(E1, C1, EMPTY, EMPTY, CASTLEFLAG), list);
        }
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
			if (b.enPassent != NO_SQUARE){
	      if ((sq-9) == b.enPassent){
	        addEnPasMove(b, MOVE(sq, (sq-9), EMPTY, EMPTY, EPFLAG), list);
	      }
	      if ((sq-11) == b.enPassent){
	        addEnPasMove(b, MOVE(sq, (sq-11), EMPTY, EMPTY, EPFLAG), list);
	      }
			}
    }
    if (b.castlePermission & blackKingCastle){
      if (b.pieces[G8] == EMPTY && b.pieces[F8] == EMPTY){
        if (!sqAttacked(E8, WHITE, b) && !sqAttacked(F8, WHITE, b)){
          addQuietMove(b, MOVE(E8, G8, EMPTY, EMPTY, CASTLEFLAG), list);
        }
      }
    }
    if (b.castlePermission & blackQueenCastle){
      if (b.pieces[B8] == EMPTY && b.pieces[C8] == EMPTY && b.pieces[D8] == EMPTY){
        if (!sqAttacked(D8, WHITE, b) && !sqAttacked(E8, WHITE, b)){
          addQuietMove(b, MOVE(E8, C8, EMPTY, EMPTY, CASTLEFLAG), list);
        }
      }
    }
  }
  //Sliding piece
  pieceIndex = slidingPieceSide[b.side];
  piece = slidingPieceArray[pieceIndex++];
  while (piece != 0){
    for (int i=0; i<b.numPieces[piece]; ++i){
      sq = b.pieceList[piece][i];
      if (!SqOnBoard(sq)){
        std::cout << "ERROR: SQUARE NOT ON BOARD" << std::endl;
      }
      for (int j=0; j<numDir[piece]; ++j){
        dir = pieceDir[piece][j];
        targetSq = sq + dir;
        while (!SQOFFBOARD(targetSq)){
        // BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK
          if (b.pieces[targetSq] != EMPTY){
            if (pieceColor[b.pieces[targetSq]] == (b.side ^ 1)){
              addCaptureMove(b, MOVE(sq, targetSq, b.pieces[targetSq], EMPTY, 0), list);
            }
            break;
          }
          addQuietMove(b, MOVE(sq, targetSq, EMPTY, EMPTY, 0), list);
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
    for (int i=0; i<b.numPieces[piece]; ++i){
      sq = b.pieceList[piece][i];
      if (!SqOnBoard(sq)){
        std::cout << "ERROR: SQUARE NOT ON BOARD" << std::endl;
      }
      for (int j=0; j<numDir[piece]; ++j){
        dir = pieceDir[piece][j];
        targetSq = sq + dir;
        if (SQOFFBOARD(targetSq)) {
          continue;
        }
        if (b.pieces[targetSq] != EMPTY){
          if (pieceColor[b.pieces[targetSq]] == (b.side ^ 1)){
            addCaptureMove(b, MOVE(sq, targetSq, b.pieces[targetSq], EMPTY, 0), list);
          }
          continue;
        }
        addQuietMove(b, MOVE(sq, targetSq, EMPTY, EMPTY, 0), list);
      }
    }
    piece = nonSlidingPieceArray[pieceIndex++];
  }
}

void generateAllCaptureMoves(const board& b, moveList* list){
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
			if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq+9]] == BLACK){
				addWPCaptureMove(b, sq, sq+9, b.pieces[sq+9], list);
			}
			if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq+11]] == BLACK){
				addWPCaptureMove(b, sq, sq+11, b.pieces[sq+11], list);
			}
			if (b.enPassent != NO_SQUARE){
				if ((sq+9) == b.enPassent){
					addEnPasMove(b, MOVE(sq, (sq+9), EMPTY, EMPTY, EPFLAG), list);
				}
				if ((sq+11) == b.enPassent){
					addEnPasMove(b, MOVE(sq, (sq+11), EMPTY, EMPTY, EPFLAG), list);
				}
			}
		}
	}
	else if (b.side == BLACK){
		for (int i=0; i<b.numPieces[bP]; ++i){
			sq = b.pieceList[bP][i];
			if (!SqOnBoard(sq)){
				std::cout << "Error: Square is not on board" << std::endl;
			}
			if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq-9]] == WHITE){
				addBPCaptureMove(b, sq, sq-9, b.pieces[sq-9], list);
			}
			if (!SQOFFBOARD(sq) && pieceColor[b.pieces[sq-11]] == WHITE){
				addBPCaptureMove(b, sq, sq-11, b.pieces[sq-11], list);
			}
			if (b.enPassent != NO_SQUARE){
				if ((sq-9) == b.enPassent){
					addEnPasMove(b, MOVE(sq, (sq-9), EMPTY, EMPTY, EPFLAG), list);
				}
				if ((sq-11) == b.enPassent){
					addEnPasMove(b, MOVE(sq, (sq-11), EMPTY, EMPTY, EPFLAG), list);
				}
			}
		}
	}
	//Sliding piece
	pieceIndex = slidingPieceSide[b.side];
	piece = slidingPieceArray[pieceIndex++];
	while (piece != 0){
		for (int i=0; i<b.numPieces[piece]; ++i){
			sq = b.pieceList[piece][i];
			if (!SqOnBoard(sq)){
				std::cout << "ERROR: SQUARE NOT ON BOARD" << std::endl;
			}
			for (int j=0; j<numDir[piece]; ++j){
				dir = pieceDir[piece][j];
				targetSq = sq + dir;
				while (!SQOFFBOARD(targetSq)){
				// BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK
					if (b.pieces[targetSq] != EMPTY){
						if (pieceColor[b.pieces[targetSq]] == (b.side ^ 1)){
							addCaptureMove(b, MOVE(sq, targetSq, b.pieces[targetSq], EMPTY, 0), list);
						}
						break;
					}
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
		for (int i=0; i<b.numPieces[piece]; ++i){
			sq = b.pieceList[piece][i];
			if (!SqOnBoard(sq)){
				std::cout << "ERROR: SQUARE NOT ON BOARD" << std::endl;
			}
			for (int j=0; j<numDir[piece]; ++j){
				dir = pieceDir[piece][j];
				targetSq = sq + dir;
				if (SQOFFBOARD(targetSq)) {
					continue;
				}
				if (b.pieces[targetSq] != EMPTY){
					if (pieceColor[b.pieces[targetSq]] == (b.side ^ 1)){
						addCaptureMove(b, MOVE(sq, targetSq, b.pieces[targetSq], EMPTY, 0), list);
					}
					continue;
				}
			}
		}
		piece = nonSlidingPieceArray[pieceIndex++];
	}
}

bool moveExists(board& b, const int move){
	moveList* list = new moveList;
	generateAllMoves(b, list);

	for (int i=0; i<list->getCount(); ++i){
		if (!makeMove(b, list->ml_getMove(i))){
			continue;
		}
		takeMove(b);
		if (list->ml_getMove(i) == move){
			return true;
		}
	}
	return false;
}

int initMVVLVA(){
	for (int i=wP; i<= bK; ++i){
		for (int j=wP; j<= bK; ++j){
			MVV_LVA_Scores[j][i] = victimScore[j] + 6 - (victimScore[i]/100);
		}
	}
}

void pickNextMove(int moveNum, moveList* list){
	move temp;
	int bestScore = 0;
	int bestNum = moveNum;

	for (int i=moveNum; i<list->getCount(); ++i){
		if (list->ml_getScore(i) > bestScore){
			bestScore = list->ml_getScore(i);
			bestNum = i;
		}
	}
		temp = list->moves[moveNum];
		list->moves[moveNum] = list->moves[bestNum];
		list->moves[bestNum] = temp;
}
