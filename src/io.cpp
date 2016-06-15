//io.cpp
#include "io.h"
#include "enums.h"
#include "movelist.h"
#include "validate.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include "threats.h"
#include "movegenerator.h"

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

int parseMove(char* move, board& b){
  if (move[1] > '8' || move[1] < '1') return false;
  if (move[3] > '8' || move[3] < '1') return false;
  if (move[2] > 'h' || move[2] < 'a') return false;
  if (move[0] > 'h' || move[0] < 'a') return false;

  int from = toSquareNumber(move[0] - 'a', move[1] - '1');
  int to = toSquareNumber(move[2] - 'a', move[3] - '1');

  if (!(SqOnBoard(from) && SqOnBoard(to))) std::cout << "squares are not on board" << std::endl;

  moveList* list = new moveList;
  generateAllMoves(b, list);

  int mv;
  int promotion = EMPTY; //Was there a promoted piece?

  for (int i=0; i< list->getCount(); ++i){
    mv = list->ml_getMove(i);
    if (FROMSQ(mv) == from && TOSQ(mv) == to){
      promotion = PROMOTED(mv);
      if (promotion != EMPTY){
        if (isRookQueen(promotion) && !isBishopQueen(promotion) && move[4] == 'r'){
          delete list;
          return mv;
        }
        else if (isBishopQueen(promotion) && !isRookQueen(promotion) && move[4] =='b'){
          delete list;
          return mv;
        }
        else if (isRookQueen(promotion) && isBishopQueen(promotion) && move[4] == 'q'){
          delete list;
          return mv;
        }
        else if (isKnight(promotion) && move[4] == 'n'){
          delete list;
          return mv;
        }
        continue;
      }
      delete list;
      return mv;
    }
  }
  //If no move was found
  delete list;
  return NOMOVE;
}


/***** code found on http://home.arcor.de/dreamlike/chess/ *****/
int inputWaiting()
{
  fd_set readfds;
  struct timeval tv;
  FD_ZERO (&readfds);
  FD_SET (fileno(stdin), &readfds);
  tv.tv_sec=0; tv.tv_usec=0;
  select(16, &readfds, 0, 0, &tv);
  return (FD_ISSET(fileno(stdin), &readfds));
}

void readInput(searchInfo* search) {
  int bytes;
  char input[256] = "", *endc;

  if (inputWaiting()) {
	search->stopped = true;

	do {
	  bytes=read(fileno(stdin),input,256);
	} while (bytes<0);

	endc = strchr(input,'\n');

	if (endc) {
    *endc=0;
  }

	if (strlen(input) > 0) {
		if (!strncmp(input, "quit", 4))    {
		  search->quit = true;
		}
	}
	return;
  }
}
/***** code found on http://home.arcor.de/dreamlike/chess/ *****/
