// xboard.c
#include "xboard.h"
#include "threats.h"
#include "movelist.h"
#include "search.h"
#include "makemove.h"
#include "movegenerator.h"
#include "io.h"
#include "timer.h"
#include <string>
#include <iostream>

int threeFoldRep(const board& b) {
	int i = 0, r = 0;
	for (i = 0; i < b.plyHistory; ++i)	{
	    if (b.history[i].posKey == b.posKey) {
		    r++;
		}
	}
	return r;
}

bool drawMaterial(const board& b) {
  if (b.numPieces[wP] || b.numPieces[bP]) return false;
  if (b.numPieces[wQ] || b.numPieces[bQ] || b.numPieces[wR] || b.numPieces[bR]) return true;
  if (b.numPieces[wB] > 1 || b.numPieces[bB] > 1) {return false;}
  if (b.numPieces[wN] > 1 || b.numPieces[bN] > 1) {return false;}
  if (b.numPieces[wN] && b.numPieces[wB]) {return false;}
  if (b.numPieces[bN] && b.numPieces[bB]) {return false;}

  return true;
}

bool checkResult(board& b) {
  if (b.fiftyMoves > 100) {
   printf("1/2-1/2 {fifty move rule (claimed by PENIQLIOTUV)}");
	 return true;
  }
  if (threeFoldRep(b) >= 2) {
   printf("1/2-1/2 {3-fold repetition (claimed by PENIQLIOTUV)}");
	 return true;
  }
	if (drawMaterial(b) == true) {
   printf("1/2-1/2 {insufficient material (claimed by PENIQLIOTUV)}");
	 return true;
  }
	moveList* list = new moveList;
  generateAllMoves(b,list);

	int found = 0;
	for(int i = 0; i < list->count; ++i) {
    if (!makeMove(b,list->ml_getMove(i)))  {
      continue;
    }
    found++;
	takeMove(b);
	break;
  }

	if(found != 0) {
		return false;
	}
	bool inCheck = sqAttacked(b.kingSquare[b.side],b.side^1,b);
	if(inCheck == true)	{
    if(b.side == WHITE) {
      printf("0-1 {black mates (claimed by PENIQLIOTUV)}");
			return true;
    }
		else {
      printf("0-1 {white mates (claimed by PENIQLIOTUV)}");
			return true;
    }
  }
	else {
    printf("\n1/2-1/2 {stalemate (claimed by PENIQLIOTUV)}");
		return true;
  }
	delete list;
	return false;
}

void consoleLoop(board& b, searchInfo* search) {

	printf("Welcome to PENIQLIOTUV In Console Mode!\n");
	printf("Type help for commands\n\n");

	search->gameMode = CONSOLEMODE;
	search->postThinking = true;
	setbuf(stdin, NULL);
  setbuf(stdout, NULL);

	int depth = MAXDEPTH, moveTime = 3000;
	int engineSide = BOTH;
	int move = NOMOVE;
	char inBuf[80], command[80];

	engineSide = BLACK;
	parseFen(START_FEN, b);

	while(true) {
		fflush(stdout);
		if(b.side == engineSide && checkResult(b) == false) {
			search->startTime = getTime();
			search->depth = depth;

			if(moveTime != 0) {
				search->timeSet = true;
				search->stopTime = search->startTime + moveTime;
			}
			searchPosition(b, search);
		}
		std::cout << std::endl << "PENIQLIOTUV > ";

		memset(&inBuf[0], 0, sizeof(inBuf));
		fflush(stdout);
		if (!fgets(inBuf, 80, stdin))
		continue;

		sscanf(inBuf, "%s", command);

		if(!strcmp(command, "help")) {
			printf("Commands:\n");
			printf("quit - quit game\n");
			printf("force - computer will not think\n");
			printf("print - show board\n");
			printf("post - show thinking\n");
			printf("nopost - do not show thinking\n");
			printf("new - start new game\n");
			printf("go - set computer thinking\n");
			printf("depth x - set depth to x\n");
			printf("time x - set thinking time to x seconds (depth still applies if set)\n");
			printf("view - show current depth and movetime settings\n");
			printf("** note ** - to reset time and depth, set to 0\n");
			printf("enter moves using b7b8q notation\n\n\n");
			continue;
		}

		if(!strcmp(command, "quit")) {
			search->quit = true;
			break;
		}

		if(!strcmp(command, "post")) {
			search->postThinking = true;
			continue;
		}

		if(!strcmp(command, "print")) {
			printBoard(b);
			continue;
		}

		if(!strcmp(command, "nopost")) {
			search->postThinking = false;
			continue;
		}

		if(!strcmp(command, "force")) {
			engineSide = BOTH;
			continue;
		}

		if(!strcmp(command, "view")) {
			if(depth == MAXDEPTH) printf("depth not set ");
			else printf("depth %d",depth);

			if(moveTime != 0) printf(" movetime %ds\n",moveTime/1000);
			else printf(" movetime not set\n");

			continue;
		}

		if(!strcmp(command, "depth")) {
			sscanf(inBuf, "depth %d", &depth);
		  if(depth==0) {
				depth = MAXDEPTH;
			}
			continue;
		}

		if(!strcmp(command, "time")) {
			sscanf(inBuf, "time %d", &moveTime);
			moveTime *= 1000;
			continue;
		}

		if(!strcmp(command, "new")) {
			engineSide = BLACK;
			parseFen(START_FEN, b);
			continue;
		}

		if(!strcmp(command, "go")) {
			engineSide = b.side;
			continue;
		}

		move = parseMove(inBuf, b);
		if(move == NOMOVE) {
			printf("Command unknown:%s\n",inBuf);
			continue;
		}
		makeMove(b, move);
		b.ply=0;
    }
}
