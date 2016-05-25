#ifndef VALIDATE
#define VALIDATE
#include "enums.h"

bool SqOnBoard(const int sq);
bool SideValid(const int side);
bool FileRankValid(const int fr);
bool PieceValidEmpty(const int pce);
bool PieceValid(const int pce);

#endif
