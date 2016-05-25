//io.h
#ifndef IO
#define IO

#include <string>

//Prints square in string format
std::string printSquare(const int sq);
//Prints move in string format
//optional 5th character at the end of the string
//refers to what piece was captured (if any)
std::string printMove(const int move);

#endif
