//makemove.cpp
#include "makemove.h"

#define HASH_CASTLEKEY (b.posKey ^= (castlePermArray[(b.castlePermission)]))
#define HASH_PIECEKEY (piece,sq) (b.posKey ^= (pieceKeys[(piece)][(sq)]))
#define HASH_SIDEKEY (b.posKey ^= (sideKey))
#define HASH_EPKEY (b.posKey ^= (pieceKeys[EMPTY][(b.enPassent)]))


//castle permission represented by 4 bits, 15 == 1 1 1 1
//will be bitwise AND-ing with the array

const int castlePermArray[120] = {
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};
