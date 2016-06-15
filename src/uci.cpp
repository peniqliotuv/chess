//uci.cpp
#include "uci.h"
#include "pvt.h"
#include <istream>
#include <iostream>

void parseGo(char* command, searchInfo* search, board& b){

}

void parsePosition(char* command, board& b){

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
}
