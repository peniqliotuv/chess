//pvt.cpp
#include "pvt.h"
#include "enums.h"
#include <iostream>

//Two megabytes
const int PVT_SIZE = 0x100000 * 2;

void PVTable::PVTSetMove(int mv, int index){
  this->table[index].move = mv;
}

void PVTable::PVTSetPosKey(U64 key, int index){
  this->table[index].posKey = key;
}

U64 PVTable::PVTGetPosKey(int index){
  return (this->table[index].posKey);
}

int PVTable::PVTGetMove(int index){
  return (this->table[index].move);
}

void clearPVT(PVTable* PVT){
  for (PVEntry* PVE = PVT->table; PVE < (PVT->numEntries + PVT->table); ++PVE){
    PVE->move = NOMOVE;
    PVE->posKey = 0ULL;
  }
}

void initPVT(PVTable* PVT){
  if (PVT == NULL){
    PVT = new PVTable;
  }
  PVT->numEntries = PVT_SIZE / sizeof(PVEntry);
  PVT->numEntries -= 2;
  if (PVT->table != NULL){
    //delete PVT->table;
  }
  PVT->table = new PVEntry [PVT->numEntries];
  clearPVT(PVT);
  //std::cout << "PVT intialization complete with " << PVT->numEntries << " entries." << std::endl;
}
