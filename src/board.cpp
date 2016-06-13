//board.cpp
#include <cstdlib>
#include <iostream>
#include "board.h"
#include "io.h"
#include "pvt.h"
#include "hashkey.h"
#include "undo.h"
#include "makemove.h"
#include "bitboard.h"
#include "movegenerator.h"

//Data Members:
char pieceChar[] = ".PNBRQKpnbrqk";
char sideChar[] = "wb-";
char rowChar[] = "12345678";
char fileChar[] = "abcdefgh";

//Constructor and Destructor:
board::board(){
  PVT = new PVTable;
}

board::~board(){
  delete PVT;
}

//Member Functions:
void board::setUndoPosKey(){
  this->history[this->plyHistory].setPosKey(this->posKey);
}

void board::setUndoMove(int move){
this->history[this->plyHistory].setMove(move);
}

void board::setUndoFiftyMove(){
  this->history[this->plyHistory].setFiftyMove(this->fiftyMoves);
}

void board::setUndoEnPassent(){
  this->history[this->plyHistory].setEnPassent(this->enPassent);
}

void board::setUndoCastlePerm(){
  this->history[this->plyHistory].setCastlePerm(this->castlePermission);
}

int board::getPrevMove(){
  //std::cout << printMove(this->history[this->plyHistory].move) << std::endl;
  return this->history[this->plyHistory].move;
}

int board::getPrevCastlePerm(){
  return this->history[this->plyHistory].castlePermission;
}

int board::getPrevFiftyMove(){
  return this->history[this->plyHistory].fiftyMoves;
}

int board::getPrevEnPassent(){
  return this->history[this->plyHistory].enPassent;
}

U64 board::getPrevPosKey(int index) const{
  return this->history[index].posKey;
}

//Resets the board to empty
void resetBoard(board &b){
  for (int i=0; i<BOARD_SIZE; ++i){
    b.pieces[i] = OFFBOARD;
  }
  for (int i=0; i<64; ++i){
    b.pieces[SQ64[i]] = EMPTY;
  }
  for (int i=0; i<2; ++i){
    b.numMajorPieces[i] = 0;
    b.numMinorPieces[i] = 0;
    b.numBigPieces[i] = 0;
    b.materialValue[i] = 0;
  }
  for (int i=0; i<3; ++i){
    b.pawns[i] = 0ULL;
  }
  for (int i=0; i<13; ++i){
    b.numPieces[i] = 0;
  }

  //initPVT(b.PVT);

  b.kingSquare[0] = b.kingSquare[1] = NO_SQUARE;
  b.side = BOTH;
  b.enPassent = NO_SQUARE;
  b.castlePermission = 0;
  b.ply = 0;
  b.plyHistory = 0;
  b.posKey = 0ULL;
  b.fiftyMoves = 0;
}

void updateMateriaList(board& b){
  int piece, sq, color;
  for (int i=0; i<BOARD_SIZE; ++i){
    sq = i;
    piece = b.pieces[i];
    if (piece != EMPTY && piece != OFFBOARD){
      color = pieceColor[piece];
      if (isBig[piece]) b.numBigPieces[color]++;
      if (isMinor[piece]) b.numMinorPieces[color]++;
      if (isMajor[piece]) b.numMajorPieces[color]++;

      b.materialValue[color] += pieceValue[piece];
      b.pieceList[piece][b.numPieces[piece]] = sq;
      b.numPieces[piece]++;

      if (piece == wK) b.kingSquare[color] = sq;
      else if (piece == bK) b.kingSquare[color] = sq;

      if (piece == wP){
        setBit(b.pawns[WHITE], SQ120[sq]);
        setBit(b.pawns[BOTH], SQ120[sq]);
      }
      else if (piece == bP){
        setBit(b.pawns[BLACK], SQ120[sq]);
        setBit(b.pawns[BOTH], SQ120[sq]);
      }
    }
  }
}

int checkBoard(const board& b){
  int temp_numPieces[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
  int temp_numMinorPieces[2] = {0,0};
  int temp_numMajorPieces[2] = {0,0};
  int temp_numBigPieces[2] = {0,0};
  int temp_materialValue[2] = {0,0};
  U64 temp_pawns[3] = {0ULL, 0ULL, 0ULL};
  temp_pawns[WHITE] = b.pawns[WHITE];
  temp_pawns[BLACK] = b.pawns[BLACK];
  temp_pawns[BOTH] = b.pawns[BOTH];

  int sq120, color, pcount, tempPiece;

  for (int i = wP; i <= bK; ++i){
    for (int j = 0; j < b.numPieces[i]; ++j){
      sq120 = b.pieceList[i][j];
      if (b.pieces[sq120] != i) throw pieceListException();
    }
  }

  for (int i = 0; i < 64; ++i){
    sq120 = SQ64[i];
    tempPiece = b.pieces[sq120];
    temp_numPieces[tempPiece]++;
    color = pieceColor[tempPiece];
    if (isBig[tempPiece]) temp_numBigPieces[color]++;
    if (isMinor[tempPiece]) {
      temp_numMinorPieces[color]++;
    }
    if (isMajor[tempPiece]) temp_numMajorPieces[color]++;
    temp_materialValue[color] += pieceValue[tempPiece];
  }

  for (int i = wP; i<= bK; ++i){
    if (temp_numPieces[i] != b.numPieces[i]){
      throw pieceListException();
    }
  }

  pcount = countBits(temp_pawns[WHITE]);
  if (pcount != b.numPieces[wP]) throw pieceListException();
  pcount = countBits(temp_pawns[BLACK]);
  if (pcount != b.numPieces[bP]) throw pieceListException();
  pcount = countBits(temp_pawns[BOTH]);
  if (pcount != (b.numPieces[wP] + b.numPieces[bP])) {
    std::cout << "Piece count: " << pcount << " White: " << b.numPieces[wP] << " Black: " << b.numPieces[bP] <<
    " Total: " << (b.numPieces[wP] + b.numPieces[bP]) << std::endl;
    throw pieceListException();
  }

  while (temp_pawns[WHITE]){
    int sq = popBit(&temp_pawns[WHITE]);
    if (b.pieces[SQ64[sq]] != wP) throw pieceListException();
  }
  while (temp_pawns[BLACK]){
    int sq = popBit(&temp_pawns[BLACK]);
    if (b.pieces[SQ64[sq]] != bP) throw pieceListException();
  }
  while (temp_pawns[BOTH]){
    int sq = popBit(&temp_pawns[BOTH]);
    if (b.pieces[SQ64[sq]] != bP && b.pieces[SQ64[sq]] != wP){
      throw pieceListException();
    }

  }

  if (temp_materialValue[WHITE] != b.materialValue[WHITE] || temp_materialValue[BLACK] != b.materialValue[BLACK]){
    std::cout << "TmpWhite: " << std::dec <<temp_materialValue[WHITE] << " White: " << std::dec <<b.materialValue[WHITE] << std::endl;
    std::cout << "Tmpblack: " << std::dec << temp_materialValue[BLACK] << " black: " <<std::dec << b.materialValue[BLACK] << std::endl;
    throw pieceListException();
  }
  if (temp_numMajorPieces[WHITE] != b.numMajorPieces[WHITE] || temp_numMajorPieces[BLACK] != b.numMajorPieces[BLACK]){
    throw pieceListException();
  }
  if (temp_numMinorPieces[WHITE] != b.numMinorPieces[WHITE] || temp_numMinorPieces[BLACK] != b.numMinorPieces[BLACK]){
    std::cout << "TmpWhite: " << std::dec << temp_numMinorPieces[WHITE] << " White: " << std::dec <<b.numMinorPieces[WHITE] << std::endl;
    std::cout << "Tmpblack: " << std::dec << temp_numMinorPieces[BLACK] << " black: " <<std::dec << b.numMinorPieces[BLACK] << std::endl;
    throw pieceListException();
  }
  if (temp_numBigPieces[WHITE] != b.numBigPieces[WHITE] && temp_numBigPieces[BLACK] != b.numBigPieces[BLACK]){
    throw pieceListException();
  }
  if (generatePosKey(b) != b.posKey){
    throw pieceListException();
  }
  return 1;
}

int parseFen(char* fen, board& b){
  int row = ROW_8;
  int file = FILE_A;
  int piece = 0; // number of pieces
  int count = 0; // number of empty squares
  int sq64 = 0;
  int sq120 = 0;
  resetBoard(b);

  while (row >= ROW_1){
    count = 1;
    switch (*fen) {
        case 'p': piece = bP; break;
        case 'r': piece = bR; break;
        case 'n': piece = bN; break;
        case 'b': piece = bB; break;
        case 'k': piece = bK; break;
        case 'q': piece = bQ; break;
        case 'P': piece = wP; break;
        case 'R': piece = wR; break;
        case 'N': piece = wN; break;
        case 'B': piece = wB; break;
        case 'K': piece = wK; break;
        case 'Q': piece = wQ; break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            piece = EMPTY;
            count = *fen - '0';
            break;

        case '/':
        case ' ':
            row--;
            file = FILE_A;
            fen++;
            continue;

        default:
            std::cout << "FEN ERROR" << std::endl;
            return -1;
      }

    for (int i=0; i<count; ++i){
        sq64 = row*8 + file;
        sq120 = SQ64[sq64];
        if (piece != EMPTY){
          b.pieces[sq120] = piece;
        }
        file++;
    }
    fen++;
  }

  if (*fen == 'w'){
    b.side = WHITE;
  }
  else if (*fen == 'b'){
    b.side = BLACK;
  }
  fen += 2;
  for (int i=0; i<4; ++i){
    if (*fen == ' '){
      break;
    }
    switch (*fen){
      case 'K': b.castlePermission |= whiteKingCastle; break;
      case 'Q': b.castlePermission |= whiteQueenCastle; break;
      case 'k': b.castlePermission |= blackKingCastle; break;
      case 'q': b.castlePermission |= blackQueenCastle; break;
      default: break;
    }
    fen++;
  }
  fen++;

  if (b.castlePermission < 0 || b.castlePermission > 15){
    throw castlePermBounds();
  }

  if (*fen != '-'){
    file = fen[0] - 'a';
    row = fen[1] - '1';
    b.enPassent = toSquareNumber(file, row);
  }
  b.posKey = generatePosKey(b);
  updateMateriaList(b);
  return 0;
}

void printBoard(const board& b){
  int sq, file, row, piece;
  std::cout << std::endl << "*******GAME BOARD*******" << std::endl;
  for (row = ROW_8; row >= ROW_1; row--){
    std::cout << row+1 << "   ";
    for (file = FILE_A; file <= FILE_H; ++file){
      sq = toSquareNumber(file, row);
      piece = b.pieces[sq];
      std::cout << pieceChar[piece] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl << "    ";
  for (file = FILE_A; file <= FILE_H; ++file){
    char f = 'a' + file;
    std::cout << f << " ";
  }
  std::cout << std::endl;
  std::cout << "Side: " << sideChar[b.side] << std::endl;
  std::cout << "En Passent: " << std::dec << b.enPassent << std::endl; // In base-10
  std::cout << "Castle Permissions: ";
  b.castlePermission & whiteKingCastle ? (std::cout << 'K') : (std::cout << '-');
  b.castlePermission & whiteQueenCastle ? (std::cout << 'Q') : (std::cout << '-');
  b.castlePermission & blackKingCastle ? (std::cout << 'k') : (std::cout << '-');
  b.castlePermission & blackQueenCastle ? (std::cout << 'q') : (std::cout << '-');
  std::cout << std::endl << "Position Key: " << std::hex<< b.posKey << std::endl << std::endl;
}

void storePVMove(const board& b, const int move){
  //Where you want to store
  //NUMENTRIES NOT DEFINED
  int index = b.posKey % ((&b)->PVT->numEntries);
  if (index < 0 || index > ((&b)->PVT->numEntries - 1)) std::cout << "PVT index is invalid" << std::endl;
  (&b)->PVT->PVTSetMove(move, index);
  (&b)->PVT->PVTSetPosKey(b.posKey, index);
}

int probePVT(const board& b){
  //Where you want to store
  int index = b.posKey % ((&b)->PVT->numEntries);
  if (index < 0 || index > ((&b)->PVT->numEntries - 1)) std::cout << "PVT index is invalid" << std::endl;
  if ((&b)->PVT->PVTGetPosKey(index) == b.posKey){
    return ((&b)->PVT->PVTGetMove(index));
  }
  else return NOMOVE;
}

int getPVLine(const int depth, board& b){
  if (depth > MAXDEPTH) std::cout << "Error, depth > MAXDEPTH" << std::endl;

  int move = probePVT(b);
  int count = 0;

  while (count < depth && move != NOMOVE){
    if (count > MAXDEPTH) std::cout << "Error, count > MAXDEPTH" << std::endl;
    if (moveExists(b, move)){
      makeMove(b, move);
      b.PVArray[count++] = move;
    }
    else break;
    move = probePVT(b);
  }

  while (b.ply > 0){
    takeMove(b);
  }

  return count;
}
