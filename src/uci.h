//uci.h
#ifndef UCI_PROTOCOL
#define UCI_PROTOCOL

#include "board.h"
#include "enums.h"
#include "search.h"

#define INPUTBUFFER 3000

void parseGo(char* command, searchInfo* search, board& b);
void parsePosition(char* command, board& b);
void UCILoop();

#endif
