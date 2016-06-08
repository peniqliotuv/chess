//makemove.cpp
#include "makemove.h"
#include "hashkey.h"
#include "threats.h"
#include "validate.h"
#include <iostream>

//castle permission represented by 4 bits, 15 == 1 1 1 1
//will be bitwise AND-ing with the array

const int castlePermArray[120] = {
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

void clearPiece(const int sq, board& b){
  int piece = b.pieces[sq];
  int color = pieceColor[piece];
  int tempPieceNum = -1;

  hashPieceKey(b, piece, sq);

  b.pieces[sq] = EMPTY;
  b.materialValue[color] -= pieceValue[piece];

  if (isBig[piece]){
    b.numBigPieces[color]--;
    if (isMajor[piece]){
      b.numMajorPieces[color]--;
    }
    else if (isMinor[piece]){
      b.numMinorPieces[color]--;
    }
  }
  else {
    clearBit(b.pawns[color], SQ120[sq]);
    clearBit(b.pawns[BOTH], SQ120[sq]);
  }

  for (int i=0; i<b.numPieces[piece]; ++i){
    if (b.pieceList[piece][i] == sq){
      tempPieceNum = sq;
      break;
    }
  }
  if (tempPieceNum == -1){
    std::cout << "Error" << std::endl;
  }
  b.numPieces[piece]--;
  b.pieceList[piece][tempPieceNum] = b.pieceList[piece][b.numPieces[piece]];
}

void addPiece(const int sq, board& b, int piece){
  int color = pieceColor[piece];

  hashPieceKey(b, piece, sq);
  b.pieces[sq] = piece;

  if (isBig[piece]){
    b.numBigPieces[piece]++;
    if (isMajor[piece]){
      b.numMajorPieces[piece]++;
    }
    else{
      b.numMinorPieces[piece]++;
    }
  }
  else{
    setBit(b.pawns[color], SQ120[sq]);
    setBit(b.pawns[BOTH], SQ120[sq]);
  }

  b.materialValue[color] -= pieceValue[piece];
  b.pieceList[piece][b.numPieces[piece]] = sq;
  b.numPieces[piece]++;
}

void movePiece(const int from, const int to, board& b){
  bool foundPiece = false; //for sanity checking

  int piece = b.pieces[from];
  //std::cout << "PIece:" << piece << std::endl;
  int color = pieceColor[piece];

  hashPieceKey(b, piece, from);
  b.pieces[from] = EMPTY;
  hashPieceKey(b, piece, to);
  b.pieces[to] = piece;

  if (!isBig[piece]){ //if it's a pawn
    clearBit(b.pawns[color], SQ120[from]);
    clearBit(b.pawns[BOTH], SQ120[from]);
    setBit(b.pawns[color], SQ120[to]);
    setBit(b.pawns[BOTH], SQ120[to]);
  }

  for (int i=0; i<b.numPieces[piece]; ++i){
    if (b.pieceList[piece][i] == from){
      b.pieceList[piece][i] = to;
      foundPiece = true;
      break;
    }
  }

  if (!foundPiece){
    std::cout << "error: piece not found. cannot move" << std::endl;
  }
}

void takeMove(board& b){
  if (checkBoard(b) != 1){
    std::cout << "error" << std::endl;
  }
  b.plyHistory--;
  b.ply--;

  int move = b.getPrevMove();
  int from = FROMSQ(move);
  int to = TOSQ(move);

  if (!SqOnBoard(from)){
    std::cout << "From square is not on board" << std::endl;
  }
  if (!SqOnBoard(to)){
    std::cout << "To square is not on board" << std::endl;
  }
  if (b.enPassent != NO_SQUARE){
    hashEnPasKey(b);
  }

  hashCastleKey(b);
  b.castlePermission = b.getPrevCastlePerm();
  b.fiftyMoves = b.getPrevFiftyMove();
  b.enPassent = b.getPrevEnPassent();

  if (b.enPassent != NO_SQUARE){
    hashEnPasKey(b);
  }

  hashCastleKey(b);
  b.side ^= 1;
  hashSideKey(b);

  if (move & EPFLAG){
    if (b.side == WHITE){
      addPiece(to-10, b, bP);
    }
    else {
      addPiece(to+10, b, wP);
    }
  }
  else if (move & CASTLEFLAG){
    if (to == C1){
      movePiece(D1, A1, b);
    }
    else if (to == C8){
      movePiece(D8, A8, b);
    }
    else if (to == G1){
      movePiece(F1, H1, b);
    }
    else if (to == G8){
      movePiece(F8, H8, b);
    }
    else{
      std::cout << "CASTLING ERROR" << std::endl;
    }
  }
  movePiece(to, from, b);
  if (isKing(b.pieces[to])){
    b.kingSquare[b.side] = to;
  }
  int capturedPiece = CAPTURED(move);
  if (capturedPiece != EMPTY){
    std::cout << "A piece was captured" << std::endl;
    addPiece(to, b, capturedPiece);
  }
  int promotedPiece = PROMOTED(move);
  if (promotedPiece != EMPTY){
    if (!PieceValid(promotedPiece)){
      std::cout << "err promoted piece" << std::endl;
    }
    clearPiece(from, b);
    addPiece(from, b, (pieceColor[promotedPiece] == WHITE ? wP : bP));
  }
  if (checkBoard(b) != 1){
    std::cout << "error" << std::endl;
  }
}

bool makeMove(board& b, int move){
  int from = FROMSQ(move);
  int to = TOSQ(move);

  //Error checking
  if (!checkBoard(b)){
    std::cout << "checkboard failed" << std::endl;
  }
  if (!SqOnBoard(from)){
    std::cout << "from square not on board" << std::endl;
  }
  if (!SqOnBoard(to)){
    std::cout << "to square not on board" << std::endl;
  }
  if (!SideValid(b.side)){
    std::cout << "side not valid" << std::endl;
  }
  if (!PieceValid(b.pieces[from])){
    std::cout << "piece not valid" << std::endl;
  }
  b.setUndoPosKey(b.plyHistory);

  //If move was enpassent
  if (move & EPFLAG){
    if (b.side == WHITE){
      clearPiece(to-10, b);
    }
    else {
      clearPiece(to+10, b);
    }
  }
  else if (move & CASTLEFLAG){
    if (to == C1){
      movePiece(A1, D1, b);
    }
    else if (to == C8){
      movePiece(A8, D8, b);
    }
    else if (to == G1){
      movePiece(H1, F1, b);
    }
    else if (to == G8){
      movePiece(H8, F8, b);
    }
    else{
      std::cout << "CASTLING ERROR" << std::endl;
    }
  }

  if (b.enPassent != NO_SQUARE){
    hashEnPasKey(b);
  }
  hashCastleKey(b);
  //Store new info in the updated history array
  b.setUndoMove(b.plyHistory, move);
  b.setUndoFiftyMove(b.plyHistory);
  b.setUndoEnPassent(b.plyHistory);
  b.setUndoCastlePerm(b.plyHistory);
  b.castlePermission &= castlePermArray[to];
  b.castlePermission &= castlePermArray[from];
  b.enPassent = NO_SQUARE;

  hashCastleKey(b);

  int capturedPiece = CAPTURED(move);
  b.fiftyMoves++;

  if (capturedPiece != EMPTY){
    std::cout << "A piece was captured" << std::endl;
    clearPiece(to, b);
    b.fiftyMoves = 0;
  }
  b.plyHistory++;
  b.ply++;

  if (isPawn(b.pieces[from])){
    b.fiftyMoves = 0;
    if (move & PAWNFLAG){
      if (b.side == WHITE){
        b.enPassent = from + 10;
        if (rowChar[b.enPassent] != ROW_3){
          std::cout << "EP error" << std::endl;
        }
      }
      else if (b.side == BLACK){
        b.enPassent = from - 10;
        if (rowChar[b.enPassent] != ROW_6){
          std::cout << "EP ERROR" << std::endl;
        }
      }
      hashEnPasKey(b);
    }
  }

  movePiece(from, to, b);

  int promotedPiece = PROMOTED(move);
  if (promotedPiece != EMPTY){
    if (isPawn(promotedPiece)){
      std::cout << "error: promoted piece should not be a pawn" << std::endl;
    }
    clearPiece(to, b);
    addPiece(to, b, promotedPiece);
  }
  if (isKing(b.pieces[to])){
    b.kingSquare[b.side] = to;
  }
  b.side ^= 1;
  hashSideKey(b);

  if (checkBoard(b) != 1){
    std::cout << "board error" << std::endl;
  }

  if (sqAttacked(b.kingSquare[b.side], b.side, b)){
    takeMove(b);
    return false;
  }
  return true;
}
