//io.cpp
#include "io.h"
#include "enums.h"
#include "threats.h"
#include <iostream>

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
