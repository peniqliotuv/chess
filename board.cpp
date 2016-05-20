#include <cstdlib>
#include "enums.h"
#include <iostream>
#include <exception>

//Exceptions
class castlePermBounds : public std::exception{
  virtual const char* what() const throw(){
    return "Castle Permission is out of bounds";
  }
};

//DATA MEMBERS:
char pieceChar[] = ".PNBRQKpnbrqk";
char sideChar[] = "wb-";
char rowChar[] = "12345678";
char fileChar[] = "abcdefgh";

//Resets the board to empty
void resetBoard(board &b){
  for (int i=0; i<BOARD_SIZE; i++){
    b.pieces[i] = OFFBOARD;
  }
  for (int i=0; i<64; i++){
    b.pieces[SQ64[i]] = EMPTY;
  }
  for (int i=0; i<3; i++){
    b.numMajorPieces[i] = 0;
    b.numMinorPieces[i] = 0;
    b.numBigPieces[i] = 0;
    b.pawns[i] = 0ULL;
  }
  for (int i=0; i<13; i++){
    b.numPieces[i] = 0;
  }

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
  for (int i=0; i<BOARD_SIZE; i++){
    sq = i;
    piece = b.pieces[i];
    if (piece != EMPTY && piece != OFFBOARD){
      color = pieceColor[piece];
      if (isBig[piece]) b.numBigPieces[color]++;
      if (isMinor[piece]) b.numMinorPieces[color]++;
      if (isMajor[piece]) b.numMajorPieces[color]++;

      b.materialValue[color] += pieceValue[color];

      b.pieceList[piece][b.numPieces[piece]] = sq;
      b.numPieces[piece]++;
      if (piece == wK) b.kingSquare[color] = sq;
      else if (piece == bK) b.kingSquare[color] = sq;
    }
  }
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

    for (int i=0; i<count; i++){
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
  for (int i=0; i<4; i++){
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
  return 0;
}

void printBoard(const board& b){
  int sq, file, row, piece;

  std::cout << "*******GAME BOARD*******" << std::endl;
  for (row = ROW_8; row >= ROW_1; row--){
    std::cout << row+1 << "   ";
    for (file = FILE_A; file <= FILE_H; file++){
      sq = toSquareNumber(file, row);
      piece = b.pieces[sq];
      std::cout << pieceChar[piece] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "    ";
  for (file = FILE_A; file <= FILE_H; file++){
    char f = 'a' + file;
    std::cout << f << " ";
  }
  std::cout << std::endl;
  std::cout << "Side: " << sideChar[b.side] << std::endl;
  std::cout << "En Passent: " << std::dec << b.enPassent << std::endl; // In base-10
  std:: cout << "Castle Permissions: ";
  b.castlePermission & whiteKingCastle ? (std::cout << 'K') : (std::cout << '-');
  b.castlePermission & whiteQueenCastle ? (std::cout << 'Q') : (std::cout << '-');
  b.castlePermission & blackKingCastle ? (std::cout << 'k') : (std::cout << '-');
  b.castlePermission & blackQueenCastle ? (std::cout << 'q') : (std::cout << '-');
  std::cout << std::endl << "Position Key: " << std::hex<< b.posKey << std::endl;
}
