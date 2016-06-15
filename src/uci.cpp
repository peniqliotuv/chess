//uci.cpp
#include "uci.h"
#include "pvt.h"
#include "io.h"
#include "timer.h"
#include "makemove.h"
#include <istream>
#include <iostream>

void parseGo(char* command, searchInfo* search, board& b){
  char* ptr = NULL;
  int depth = -1;
  int movesToGo = 30;
  int moveTime = -1;
  int t = -1;
  int inc = 0;
  search->timeSet = false;

  if ((ptr = strstr(command, "infinite"))){
    ;
  }
  if ((ptr = strstr(command, "binc"))){ //black increment
    inc = std::atoi(ptr + 5);
  }
  if ((ptr = strstr(command, "winc"))){ //white increment
    inc = std::atoi(ptr + 5);
  }
  if ((ptr = strstr(command,"wtime")) && b.side == WHITE) {
		t = std::atoi(ptr + 6);
	}
	if ((ptr = strstr(command,"btime")) && b.side == BLACK) {
		t = std::atoi(ptr + 6);
	}
	if ((ptr = strstr(command,"movestogo"))) {
		movesToGo = std::atoi(ptr + 10);
	}
	if ((ptr = strstr(command,"movetime"))) {
		moveTime = std::atoi(ptr + 9);
	}
	if ((ptr = strstr(command,"depth"))) {
		depth = std::atoi(ptr + 6);
	}

  if (moveTime != -1){
    t = moveTime;
    movesToGo = 1;
  }
  search->startTime = getTime();
  search->depth = depth;

  if (t != -1){
    search->timeSet = true;
    t /= movesToGo;
    t -= 50; // to be safe, take 50ms off
    search->stopTime = search->startTime + inc + t;
  }

  if (depth == -1 ){
    search->depth = MAXDEPTH;
  }

  std::cout << "time: " << t << " start: " << search->startTime <<
  " stop: " << search->stopTime << " depth: " << search->depth <<
  " timeset: " << search->timeSet << std::endl;
  searchPosition(b, search);
}

void parsePosition(char* command, board& b){
  command += 9; // "position " has 9 characters, each string has it
  char* temp = command;
  if (strncmp(command, "startpos", 8) == 0){
    parseFen(START_FEN, b);
  }
  else {
    temp = strstr(command, "fen");
    if (temp == NULL){
      parseFen(START_FEN, b);
    }
    else {
      temp += 4;
      parseFen(temp, b);
    }
  }

  temp = strstr(command, "moves");
  int move;
  if (temp != NULL){
    temp += 6;
    while (*temp){ // while we're pointing to something
      move = parseMove(temp, b);
      if (move == NOMOVE) break;
      makeMove(b, move);
      b.ply = 0;
      while (*temp && *temp != ' '){
        temp++;
      }
      temp++;
    }
  }
  printBoard(b);
}

void UCILoop(board& b, searchInfo* search){
  search->gameMode = UCIMODE;
  char command[INPUTBUFFER];
  std::cout << "id name PENIQLIOTUV" << std::endl;
  std::cout << "id author JerryTsui" << std::endl;
  std::cout << "uciok" << std::endl;

  while (true){
    std::cin.getline(command, INPUTBUFFER);
    if (command[0] == '\n') continue;
    if (!strncmp(command, "isready", 7)) {
        std::cout << "readok" << std::endl;
        continue;
    }
    else if (!strncmp(command, "position", 8)) {
        parsePosition(command, b);
    }
    else if (!strncmp(command, "ucinewgame", 10)) {
        parsePosition("position startpos\n", b);
    }
    else if (!strncmp(command, "go", 2)) {
        parseGo(command, search, b);
    }
    else if (!strncmp(command, "quit", 4)) {
        search->quit = true;
        break;
    }
    else if (!strncmp(command, "uci", 3)) {
        std::cout << "id name PENIQLIOTUV" << std::endl;
        std::cout << "id author JerryTsui" << std::endl;
        std::cout << "uciok" << std::endl;
    }
    if(search->quit) break;
  }
  return;
}
