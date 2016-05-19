#include <cstdlib>
#include "enums.h"
#include <string>
#include <iostream>

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

  if (*fen != 'w' || *fen != 'b'){
    std::cout << "you have an error" << std::endl;
  }//error checking

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
    std::cout << "castleperm error" << std::endl;
  } // error checking

  if (*fen != '-'){
    file = fen[0] - 'a';
    row = fen[1] - '1';
    b.enPassent = toSquareNumber(file, row);
  }
  b.posKey = generatePosKey(b);
  return 0;
}