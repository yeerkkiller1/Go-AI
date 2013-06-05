#include "FastSetReplacement.h"

#include "Constants.h"
#include "Square.h"

#include "q.h"

#include "Board.h"

/*
void ctor_FastSetPiece0(FastSetLinked* m, Board* containingBoard)
{
  int x;

  DASSERT(containingBoard);

  m->sharedStorage = &containingBoard->sharedFastsetStorage;
  
  m->curDataStart = m->sharedStorage->curDataHead;
  m->curDataEnd = m->sharedStorage->curDataHead;

  m->board = containingBoard;
}

void ctor_FastSetPiece1(FastSetLinked* m, FastSetPiece* other)
{
  int x;
  m->squarePositions[cSquareIntPositions] = other->curDataHead;
  
  for(x = 0; x < cSquarePositions; x++)
    other->squarePositions[x] = m->squarePositions[x];

  m->board = other->board;
}

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetLinked* m, Square* square)
{
  //First checks if it already exists
  {
    int x;
    for(x = m->curDataStart; x < m->curDataEnd; x++)    
      if(Loc_Equ(&Board_GetSqr(m->board, m->sharedStorage->squarePositions, x)->location, &square->location))
        return;
  }

  if(m->curDataEnd != m->sharedStorage->curDataHead)
  {
    //We have to make room for ourselves.
  }
  FastSetPiece_setAt(m, m->squarePositions[cSquareIntPositions]++, square);
  //m->data[m->squarePositions[cSquareIntPositions]++] = piece;
  
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < TOTAL_SQUARES);

}
 
int FastSetPiece_length(FastSetLinked* m)
{
  return m->squarePositions[cSquareIntPositions];
}

Square* FastSetPiece_getAt(FastSetLinked* m, int pos)
{
  return Board_GetSqr(m->board, m->squarePositions, pos);
}

void FastSetPiece_setAt(FastSetLinked* m, int pos, Square* square)
{
  Board_SetSqr(m->board, m->squarePositions, pos, square);

  DASSERT((FastSetPiece_getAt(m, pos) == square));
}

Square* FastSetPiece_getAtOtherBoard(FastSetLinked* m, int pos, Board* board)
{
  return Board_GetSqr(board, m->squarePositions, pos);
}

//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Square** operator++ ( Square**

void FastSetPiece_clear(FastSetLinked* m)
{
  m->squarePositions[cSquareIntPositions] = 0;
}

int FastSetPiece_size(FastSetLinked* m)
{  
  return m->squarePositions[cSquareIntPositions];
}

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetLinked* m, int pos)
{ 
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < TOTAL_SQUARES);
  //Move everything being this pos forward by one

  //Could just leave it spacious, as we can move things in groups of 4 very very efficiently

  while(pos + 1 < m->squarePositions[cSquareIntPositions])
  {
    FastSetPiece_setAt(m, pos, Board_GetSqr(m->board, m->squarePositions, pos + 1));
    pos++;
  }  
  
  m->squarePositions[cSquareIntPositions]--;
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < TOTAL_SQUARES);
}

void FastSetPiece_erase(FastSetLinked* m, Square* piece)
{
  int pos = 0;

  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < TOTAL_SQUARES);

  //We also have to find it
  while(pos < m->squarePositions[cSquareIntPositions] && !Loc_Equ(&FastSetPiece_getAt(m, pos)->location, &piece->location))
  {
    pos++;
  }
  //Move everything being this pos forward by one

  //Only subtract if we found it!
  if(pos != m->squarePositions[cSquareIntPositions])
  {
    FastSetPiece_eraseAt(m, pos);
  }

  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < TOTAL_SQUARES);
} 

bool FastSetPiece_contains(FastSetLinked* m, Square* piece)
{
  int x;
  for(x = 0; x < m->squarePositions[cSquareIntPositions]; x++)    
    if(Loc_Equ(&FastSetPiece_getAt(m, x)->location, &piece->location))
      return true;
  return false;
}
*/