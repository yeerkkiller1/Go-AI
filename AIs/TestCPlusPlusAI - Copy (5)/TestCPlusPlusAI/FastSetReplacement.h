#pragma once

#include "q.h"
#include "Constants.h"

//As its a set, it will never contain duplicates (no matter what is done to it)

//Piece** refers to where it is stored in its current storage location
//Piece* refers to where it is on the board
//Piece refers to what it is on the board

struct FastSetPiece// : public std::set<Piece *>
{
  //Main data storage, never needs to be increased as we cannot have duplicates and
  //the board is only so big!

  Piece* data[TOTAL_SQUARES];  
  int curDataHead;
};

void ctor_FastSetPiece0(FastSetPiece* m);
void ctor_FastSetPiece1(FastSetPiece* m, FastSetPiece* other);

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetPiece* m, Piece* piece);
  
Piece** FastSetPiece_begin(FastSetPiece* m);

Piece** FastSetPiece_end(FastSetPiece* m);
    
//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Piece** operator++ ( Piece**

void FastSetPiece_clear(FastSetPiece* m);

int FastSetPiece_size(FastSetPiece* m);

//Should be faster than erase(Piece* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetPiece* m, int pos);

void FastSetPiece_erase(FastSetPiece* m, Piece* piece);

bool FastSetPiece_contains(FastSetPiece* m, Piece* piece);