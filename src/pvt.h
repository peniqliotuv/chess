//pvt.h
#ifndef PVTABLE
#define PVTABLE

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
};

void initPVT(PVTable* PVT);
//Sets all of the PVE members to zero in the table
void clearPVT(PVTable* PVT);

#endif
