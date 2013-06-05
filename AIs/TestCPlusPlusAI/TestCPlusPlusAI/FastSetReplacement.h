#pragma once

#include "q.h"
#include "Constants.h"

//#include "FastSetLinked.h"

//As its a set, it will never contain duplicates (no matter what is done to it)

//Square** refers to where it is stored in its current storage location
//Square* refers to where it is on the board
//Square refers to what it is on the board

/*
struct FastSetPiece// : public std::set<Square *>
{
  Board* board;
  
  int squarePositions[cSquareIntPositions + 1];
  //int curDataHead;
};
*/

void ctor_FastSetPiece0(unsigned char* m, Board* containingBoard);
void ctor_FastSetPiece1(unsigned char* m, unsigned char* other);

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(unsigned char* m, Square* square);
  
int FastSetPiece_length(unsigned char* m);

Square* FastSetPiece_getAt(Board* board, unsigned char* m, int pos);

//Set is not needed by anyone outside... I think?
void FastSetPiece_setAt(unsigned char* m, int pos, Square* square);

Square* FastSetPiece_getAtOtherBoard(unsigned char* m, int pos, Board* board);


void FastSetPiece_clear(unsigned char* m);

int FastSetPiece_size(unsigned char* m);

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(Board* board, unsigned char* m, int pos);

void FastSetPiece_erase(unsigned char* m, Square* piece);

bool FastSetPiece_contains(unsigned char* m, Square* piece);