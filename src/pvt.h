//pvt.h
#ifndef PV_TABLE
#define PV_TABLE

#include "enums.h"

class PVEntry {
public:
  int move;
  U64 posKey;
};

class PVTable {
public:
  int numEntries;
  PVEntry* table;

  void PVTSetMove(int mv, int index);
  void PVTSetPosKey(U64 key, int index);
  U64 PVTGetPosKey(int index);
  int PVTGetMove(int index);
};

void initPVT(PVTable* PVT);
//Sets all of the PVE members to zero in the table
void clearPVT(PVTable* PVT);

#endif
