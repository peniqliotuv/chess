//io.cpp
#include "io.h"
#include "enums.h"
#include "threats.h"

std::string rowStrings[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
std::string fileStrings[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};

std::string printSquare(const int sq){
  int file = fileArray[sq];
  int row = rowArray[sq];
  return fileStrings[file] + rowStrings[row];
}

std::string printMove(const int move){
  std::string moveString = "";
  int fromSq = FROMSQ(move);
  int toSq = TOSQ(move);

  int fileFrom = fileArray[fromSq];
  int rowFrom = rowArray[fromSq];
  int fileTo = fileArray[toSq];
  int rowTo = rowArray[toSq];

  int promoted = PROMOTED(move);

  if (promoted){
    std::string piece = "q";
    if (isKnight(promoted)){
      piece = "n";
    }
    else if (isRookQueen(promoted) && !isBishopQueen(promoted)){
      piece = "r";
    }
    else if (isBishopQueen(promoted) && !isRookQueen(promoted)){
      piece = 'b';
    }
    moveString = fileStrings[fileFrom] + rowStrings[rowFrom]
     + fileStrings[fileTo] + rowStrings[rowTo] + piece;
  }
  else {
    moveString = fileStrings[fileFrom] + rowStrings[rowFrom]
     + fileStrings[fileTo] + rowStrings[rowTo];
  }

  return moveString;
}

void printMoveList(moveList& move){
  std::cout <<std::endl << "******** MOVELIST ******** " << std::endl;
  for (int i=0; i<move.getCount(); ++i){
    std::cout << "Move " << i+1 << " :" << printMove(move.ml_getMove(i)) <<
    " Score: " << std::dec << move.ml_getScore(i) << std::endl;
  }
  std::cout << "Total amount of moves: " << std::dec << move.getCount() << std::endl;
}

void printPiece(int index){
  switch (index){
    case 0: std::cout << "EMPTY " << std::endl; break;
    case 1: std::cout << "wP " << std::endl; break;
    case 2: std::cout << "wN " << std::endl; break;
    case 3: std::cout << "wB " << std::endl; break;
    case 4: std::cout << "wR " << std::endl; break;
    case 5: std::cout << "wQ " << std::endl; break;
    case 6: std::cout << "wK " << std::endl; break;
    case 7: std::cout << "bP " << std::endl; break;
    case 8: std::cout << "bN " << std::endl; break;
    case 9: std::cout << "bB " << std::endl; break;
    case 10: std::cout << "bR " << std::endl; break;
    case 11: std::cout << "bQ " << std::endl; break;
    case 12: std::cout << "bK " << std::endl; break;
    default: std::cout << "Piece doesn't exist." << std::endl;
  }
}
