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

void printOptions() {
	printf("feature ping=1 setboard=1 colors=0 usermove=1");
	printf("feature done=1");
}

void XBoardLoop(board& b, searchInfo* search) {
	search->gameMode = XBOARDMODE;
	search->postThinking = true;
	setbuf(stdin, NULL);
  setbuf(stdout, NULL);
	printOptions(); // HACK

	int depth = -1, movesToGo[2] = {30,30 }, moveTime = -1;
	int t = -1, inc = 0;
	int engineSide = BOTH;
	int timeLeft;
	int sec;
	int mps;
	int move = NOMOVE;
	int i, score;
	char inBuf[80], command[80];

	engineSide = BLACK;
	parseFen(START_FEN, b);
	depth = -1;
	t = -1;

	while(true) {
		fflush(stdout);
		if(b.side == engineSide && checkResult(b) == false) {
			search->startTime = getTime();
			search->depth = depth;

			if(t != -1) {
				search->timeSet = true;
				t /= movesToGo[b.side];
				t -= 50;
				search->stopTime = search->startTime + t + inc;
			}

			if(depth == -1 || depth > MAXDEPTH) {
				search->depth = MAXDEPTH;
			}

			printf("time:%d start:%d stop:%d depth:%d timeset:%d movestogo:%d mps:%d\n",
				t,search->startTime,search->stopTime,search->depth,search->timeSet, movesToGo[b.side], mps);
				searchPosition(b, search);

			if(mps != 0) {
				movesToGo[b.side^1]--;
				if(movesToGo[b.side^1] < 1) {
					movesToGo[b.side^1] = mps;
				}
			}
		}
		fflush(stdout);

		memset(&inBuf[0], 0, sizeof(inBuf));
		fflush(stdout);
		if (!fgets(inBuf, 80, stdin))
		continue;

		sscanf(inBuf, "%s", command);

		printf("command seen:%s\n",inBuf);

		if(!strcmp(command, "quit")) {
			search->quit = true;
			break;
		}

		if(!strcmp(command, "force")) {
			engineSide = BOTH;
			continue;
		}

		if(!strcmp(command, "protover")){
			printOptions();
		    continue;
		}

		if(!strcmp(command, "sd")) {
			sscanf(inBuf, "sd %d", &depth);
		    printf("DEBUG depth:%d\n",depth);
			continue;
		}

		if(!strcmp(command, "st")) {
			sscanf(inBuf, "st %d", &moveTime);
		    printf("DEBUG movetime:%d\n",moveTime);
			continue;
		}

		if(!strcmp(command, "time")) {
			sscanf(inBuf, "time %d", &t);
			t *= 10;
		    printf("DEBUG time:%d\n",t);
			continue;
		}

		if(!strcmp(command, "level")) {
			sec = 0;
			moveTime = -1;
			if( sscanf(inBuf, "level %d %d %d", &mps, &timeLeft, &inc) != 3) {
			  sscanf(inBuf, "level %d %d:%d %d", &mps, &timeLeft, &sec, &inc);
		      printf("DEBUG level with :\n");
			}	else {
		      printf("DEBUG level without :\n");
			}
			timeLeft *= 60000;
			timeLeft += sec * 1000;
			movesToGo[0] = movesToGo[1] = 30;
			if(mps != 0) {
				movesToGo[0] = movesToGo[1] = mps;
			}
			t = -1;
		    printf("DEBUG level timeLeft:%d movesToGo:%d inc:%d mps%d\n",timeLeft,movesToGo[0],inc,mps);
			continue;
		}
		if(!strcmp(command, "ping")) {
			printf("pong%s\n", inBuf+4);
			continue;
		}
		if(!strcmp(command, "new")) {
			engineSide = BLACK;
			parseFen(START_FEN, b);
			depth = -1;
			t = -1;
			continue;
		}
		if(!strcmp(command, "setboard")){
			engineSide = BOTH;
			parseFen(inBuf+9, b);
			continue;
		}
		if(!strcmp(command, "go")) {
			engineSide = b.side;
			continue;
		}
		if(!strcmp(command, "usermove")){
			movesToGo[b.side]--;
			move = parseMove(inBuf+9, b);
			if(move == NOMOVE) continue;
			makeMove(b, move);
      b.ply=0;
		}
  }

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
		printf("\nPENIQLIOTUV > ");
		fflush(stdout);

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
