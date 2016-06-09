//makemove.cpp
#include "makemove.h"
#include "hashkey.h"
#include "threats.h"
#include "io.h"
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
  if (!SqOnBoard(sq)) std::cout << "sq not on board" << std::endl;

  int piece = b.pieces[sq];
  if (!PieceValid(piece)) std::cout << "piece not valid" << std::endl;

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
      tempPieceNum = i;
      break;
    }
  }
  if (tempPieceNum == -1){
    std::cout << "Error" << std::endl;
  }
  b.numPieces[piece]--;
  std::cout << "********" << std::endl;
  printPiece(piece);
  std::cout << "On: " << printSquare(b.pieceList[piece][tempPieceNum]) << std::endl;
  printPiece(b.pieces[b.pieceList[piece][tempPieceNum]]);
  printPiece(b.pieces[b.pieceList[piece][b.numPieces[piece]]]);
  b.pieceList[piece][tempPieceNum] = b.pieceList[piece][b.numPieces[piece]];
  printPiece(b.pieces[b.pieceList[piece][tempPieceNum]]);
}

void addPiece(const int sq, board& b, int piece){
  if (!SqOnBoard(sq)) std::cout << "sq not on board" << std::endl;
  if (!PieceValid(piece)) std::cout << "piece not valid" << std::endl;

  int color = pieceColor[piece];

  hashPieceKey(b, piece, sq);
  b.pieces[sq] = piece;

  if (isBig[piece]){
    b.numBigPieces[color]++;
    if (isMajor[piece]){
      b.numMajorPieces[color]++;
    }
    else{
      b.numMinorPieces[color]++;
    }
  }
  else{
    setBit(b.pawns[color], SQ120[sq]);
    setBit(b.pawns[BOTH], SQ120[sq]);
  }

  b.materialValue[color] += pieceValue[piece];
  b.pieceList[piece][b.numPieces[piece]++] = sq;
  //b.numPieces[piece]++;
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
  std::cout << "   TAKE MOVE:   " << std::endl;
  if (checkBoard(b) != 1){
    std::cout << "error" << std::endl;
  }
  b.plyHistory--;
  b.ply--;

  int move = b.getPrevMove();
  int from = FROMSQ(move);
  int to = TOSQ(move);
  std::cout << "PRINT MOVE: " << printMove(move) << std::endl;
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
    switch(to) {
        case C1: movePiece(D1, A1, b); break;
        case C8: movePiece(D8, A8, b); break;
        case G1: movePiece(F1, H1, b); break;
        case G8: movePiece(F8, H8, b); break;
        default: std::cout << "CASTLING ERROR" << std::endl; break;
    }
  }
  movePiece(to, from, b);

  if (isKing(b.pieces[from])){
    b.kingSquare[b.side] = from;
  }
  int capturedPiece = CAPTURED(move);
  printPiece(capturedPiece);
  if (capturedPiece != EMPTY){
    if (!PieceValid(capturedPiece)) std::cout << "captured piece not valid" << std::endl;
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
  std::cout << "        BEFORE: " << std::endl;
  printBoard(b);
  int from = FROMSQ(move);
  int to = TOSQ(move);
  int side = b.side;
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
  if (!SideValid(side)){
    std::cout << "side not valid" << std::endl;
  }
  if (!PieceValid(b.pieces[from])){
    std::cout << "piece not valid" << std::endl;
  }
  b.setUndoPosKey();

  //If move was enpassent
  if (move & EPFLAG){
    std::cout << "move was EP" << std::endl;
    if (side == WHITE){
      clearPiece(to-10, b);
    }
    else {
      clearPiece(to+10, b);
    }
  }
  else if (move & CASTLEFLAG){
    std::cout << "move was castle" << std::endl;
    switch(to) {
        case C1: movePiece(A1, D1, b); break;
        case C8: movePiece(A8, D8, b); break;
        case G1: movePiece(H1, F1, b); break;
        case G8: movePiece(H8, F8, b); break;
        default: std::cout << "CASTLING ERROR" << std::endl; break;
    }
  }

  if (b.enPassent != NO_SQUARE){
    hashEnPasKey(b);
  }
  hashCastleKey(b);

  //Store new info in the updated history array
  b.setUndoMove(move);
  b.setUndoFiftyMove();
  b.setUndoEnPassent();
  b.setUndoCastlePerm();

  b.castlePermission &= castlePermArray[to];
  b.castlePermission &= castlePermArray[from];

  b.enPassent = NO_SQUARE;

  hashCastleKey(b);

  int capturedPiece = CAPTURED(move);
  b.fiftyMoves++;

  if (capturedPiece != EMPTY){
    std::cout << capturedPiece << " was captured" << std::endl;
    clearPiece(to, b);
    b.fiftyMoves = 0;
  }
  b.plyHistory++;
  b.ply++;

  if (isPawn(b.pieces[from])){
    b.fiftyMoves = 0;
    if (move & PAWNFLAG){
      if (side == WHITE){
        b.enPassent = from + 10;
        if (rowArray[b.enPassent] != ROW_3) std::cout << "EP error " <<  rowChar[b.enPassent] << std::endl;
      }
      else if (side == BLACK){
        b.enPassent = from - 10;
        if (rowArray[b.enPassent] != ROW_6) std::cout << "EP ERROR " << rowChar[b.enPassent]<< std::endl;
      }
      hashEnPasKey(b);
    }
  }

  movePiece(from, to, b);

  int promotedPiece = PROMOTED(move);
  if (promotedPiece != EMPTY){
    if (isPawn(promotedPiece)) std::cout << "error: promoted piece should not be a pawn" << std::endl;
    if (!PieceValid(promotedPiece)) std::cout << "err" << std::endl;
    clearPiece(to, b);
    addPiece(to, b, promotedPiece);
  }
  if (isKing(b.pieces[to])){
    std::cout << "is king" << std::endl;
    b.kingSquare[b.side] = to;
  }
  b.side ^= 1;
  hashSideKey(b);

  //THIS IS A CAPTURE ERROR ERROR
  std::cout << "Move: " << printMove(move) << " to: " << printMove(to) << " from: " << printMove(from) << std::endl;
  if (checkBoard(b) != 1){
    std::cout << "board error" << std::endl;
  }

  if (sqAttacked(b.kingSquare[side], b.side, b)){
    takeMove(b);
    return false;
  }
  //FOR DEBUGGING BELOW
  std::cout << "        AFTER: " << std::endl;
  printBoard(b);
  return true;
}
