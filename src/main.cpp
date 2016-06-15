//main.cpp
//Jerry Tsui, 2016
#include <iostream>
#include <string>
#include "timer.h"
#include "enums.h"
#include "bitboard.h"
#include "board.h"
#include "undo.h"
#include "uci.h"
#include "test.h"
#include "movegenerator.h"
#include "makemove.h"
#include "init.h"
#include "search.h"
#include "io.h"
#include "threats.h"
#include "xboard.h"

using namespace std;

int main(){
  initialize();
  board* b = new board;
  searchInfo* search = new searchInfo;
  initPVT(b->PVT);

  cout << "Welcome to PENIQLIOTUV! Type 'console' for console mode..." << endl;

  char line[256];
	while (true) {
		memset(&line[0], 0, sizeof(line));

		fflush(stdout);
		if (!fgets(line, 256, stdin))
			continue;
		if (line[0] == '\n')
			continue;
		if (!strncmp(line, "uci",3)) {
			UCILoop(*b, search);
			if(search->quit == true)
      break;
			continue;
		}
    else if (!strncmp(line, "xboard",6))	{
			XBoardLoop(*b, search);
			if(search->quit == true){
        break;
      }
			continue;
		}
    else if (!strncmp(line, "console",6))	{
			consoleLoop(*b, search);
			if(search->quit == true){
        break;
      }
			continue;
		}
    else if(!strncmp(line, "quit",4))	{
			break;
		}
	}
  return 0;
}
