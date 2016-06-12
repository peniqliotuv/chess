//evaluate.cpp
#include "evaluate.h"
#include "validate.h"
#include <iostream>

/***** so the AI doesn't do anything stupid *****/
#define MIRROR64(sq) (Mirror64[(sq)])

const int pawnTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int knightTable[64] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int bishopTable[64] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int rookTable[64] = {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

const int Mirror64[64] = {
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

int evalPosition(const board& b){
  int piece, sq;
  int score = b.materialValue[WHITE] - b.materialValue[BLACK]; //White scores positive, black negative

  piece = wP;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score += pawnTable[SQ120[sq]];
  }

  piece = bP;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score -= pawnTable[MIRROR64(SQ120[sq])];
  }

  piece = wN;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score += knightTable[SQ120[sq]];
  }

  piece = bN;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score -= knightTable[MIRROR64(SQ120[sq])];
  }

  piece = wB;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score += bishopTable[SQ120[sq]];
  }

  piece = bB;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score -= bishopTable[MIRROR64(SQ120[sq])];
  }
  piece = wR;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score += rookTable[SQ120[sq]];
  }

  piece = bR;
  for (int i=0; i<b.numPieces[piece]; ++i){
    sq = b.pieceList[piece][i];
    if (!SqOnBoard(sq)) std::cout << "eval position not on board" << std::endl;
    score -= rookTable[MIRROR64(SQ120[sq])];
  }

  if (b.side == WHITE) return score;
  else return -score;
}
