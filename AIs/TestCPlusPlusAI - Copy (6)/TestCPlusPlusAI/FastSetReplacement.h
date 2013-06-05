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

void ctor_FastSetPiece0(int* m, Board* containingBoard);
void ctor_FastSetPiece1(int* m, int* other);

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(int* m, Square* square);
  
int FastSetPiece_length(int* m);

Square* FastSetPiece_getAt(Board* board, int* m, int pos);

//Set is not needed by anyone outside... I think?
void FastSetPiece_setAt(int* m, int pos, Square* square);

Square* FastSetPiece_getAtOtherBoard(int* m, int pos, Board* board);


void FastSetPiece_clear(int* m);

int FastSetPiece_size(int* m);

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(Board* board, int* m, int pos);

void FastSetPiece_erase(int* m, Square* piece);

bool FastSetPiece_contains(int* m, Square* piece);