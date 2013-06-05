#pragma once

//Same as FastSetPiece but instead of having its own storage it uses the storage of another
//FastSetPiece (which is shares with others)

#include "q.h"
#include "Constants.h"

/*
struct FastSetLinked
{
  Board* board;

  FastSetPiece* sharedStorage;

  int curDataStart;
  int curDataEnd;
};

void ctor_FastSetPiece0(FastSetLinked* m, Board* containingBoard);
void ctor_FastSetPiece1(FastSetLinked* m, FastSetPiece* other);

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetLinked* m, Square* square);
  
int FastSetPiece_length(FastSetLinked* m);

Square* FastSetPiece_getAt(FastSetLinked* m, int pos);

//Set is not needed by anyone outside... I think?
void FastSetPiece_setAt(FastSetLinked* m, int pos, Square* square);

Square* FastSetPiece_getAtOtherBoard(FastSetLinked* m, int pos, Board* board);


void FastSetPiece_clear(FastSetLinked* m);

int FastSetPiece_size(FastSetLinked* m);

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetLinked* m, int pos);

void FastSetPiece_erase(FastSetLinked* m, Square* piece);

bool FastSetPiece_contains(FastSetLinked* m, Square* piece);
*/