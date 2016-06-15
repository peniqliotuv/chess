//uci.cpp
#include "uci.h"
#include "pvt.h"
#include "io.h"
#include "makemove.h"
#include <istream>
#include <iostream>

void parseGo(char* command, searchInfo* search, board& b){

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

void UCILoop(){
  char command[INPUTBUFFER];
  std::cout << "id name PENIQLIOTUV" << std::endl;
  std::cout << "id author JerryTsui" << std::endl;
  std::cout << "uciok" << std::endl;

  board* b = new board;
  initPVT(b->PVT);
  searchInfo* search = new searchInfo;

  while (true){
    std::cin.getline(command, INPUTBUFFER);
    if (command[0] == '\n') continue;
    if (!strncmp(command, "isready", 7)) {
        std::cout << "readok" << std::endl;
        continue;
    }
    else if (!strncmp(command, "position", 8)) {
        parsePosition(command, *b);
    }
    else if (!strncmp(command, "ucinewgame", 10)) {
        parsePosition("position startpos\n", *b);
    }
    else if (!strncmp(command, "go", 2)) {
        parseGo(command, search, *b);
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
  delete b;
  delete search;
  return;
}
