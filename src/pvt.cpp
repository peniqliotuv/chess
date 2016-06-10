//pvt.cpp
#include "pvt.h"
#include <iostream>

const int PVT_SIZE = 0x100000 * 2;

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
  std::cout << "PVT intialization complete with " << PVT->numEntries << " entries." << std::endl;
}
