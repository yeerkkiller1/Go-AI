#include "FastSetReplacement.h"

#include "Constants.h"
#include "Square.h"

#include "q.h"

#include "Board.h"

void ctor_FastSetPiece0(int* m, Board* containingBoard)
{
  int x;

  DASSERT(containingBoard);

  m[cSquareIntPositions] = 0;

  for(x = 0; x < cSquareIntPositions; x++)
    m[x] = 0;
}

void ctor_FastSetPiece1(int* m, int* other)
{
  int x;
  m[cSquareIntPositions] = other[cSquareIntPositions];
  
  for(x = 0; x < cSquareIntPositions; x++)
    other[x] = m[x];
}

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(int* m, Square* square)
{
  //First checks if it already exists    
  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < cSquarePositions);

  {
    int x;
    for(x = 0; x < m[cSquareIntPositions]; x++)
        if(FastSetPiece_getAt(square->board, m, x) == square)
          return;
  }

  FastSetPiece_setAt(m, m[cSquareIntPositions], square);
  m[cSquareIntPositions]++;

  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < cSquarePositions);
}
 
int FastSetPiece_length(int* m)
{
  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < cSquarePositions);
  return m[cSquareIntPositions];
}

Square* FastSetPiece_getAt(Board* board, int* m, int pos)
{
  return Board_GetSqr(board, m, pos);
}

void FastSetPiece_setAt(int* m, int pos, Square* square)
{
  Board_SetSqr(square->board, m, pos, square);

  DASSERT((FastSetPiece_getAt(square->board, m, pos) == square));
}

Square* FastSetPiece_getAtOtherBoard(int* m, int pos, Board* board)
{  
  return Board_GetSqr(board, m, pos);
}

//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Square** operator++ ( Square**

void FastSetPiece_clear(int* m)
{
  m[cSquareIntPositions] = 0;
}

int FastSetPiece_size(int* m)
{
  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < cSquarePositions);
  return m[cSquareIntPositions];
}

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(Board* board, int* m, int pos)
{ 
  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < TOTAL_SQUARES);
  //Move everything being this pos forward by one

  //Could just leave it spacious, as we can move things in groups of 4 very very efficiently

  while(pos + 1 < m[cSquareIntPositions])
  {
    FastSetPiece_setAt(m, pos, Board_GetSqr(board, m, pos + 1));
    pos++;
  }  
  
  m[cSquareIntPositions]--;
  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < TOTAL_SQUARES);
}

void FastSetPiece_erase(int* m, Square* piece)
{
  int pos = 0;

  DASSERT(m);
  DASSERT(piece);

  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < TOTAL_SQUARES);

  //We also have to find it
  while(pos < m[cSquareIntPositions] && !Loc_Equ(&FastSetPiece_getAt(piece->board, m, pos)->location, &piece->location))
  {
    pos++;
  }
  //Move everything being this pos forward by one

  //Only subtract if we found it!
  if(pos != m[cSquareIntPositions])
  {
    FastSetPiece_eraseAt(piece->board, m, pos);
  }

  DASSERT(m[cSquareIntPositions] >= 0 && m[cSquareIntPositions] < TOTAL_SQUARES);
} 

bool FastSetPiece_contains(int* m, Square* piece)
{
  int x;
  for(x = 0; x < m[cSquareIntPositions]; x++)    
    if(Loc_Equ(&FastSetPiece_getAt(piece->board, m, x)->location, &piece->location))
      return true;
  return false;
}


/*
void ctor_FastSetPiece0(FastSetPiece* m, Board* containingBoard)
{
  int x;

  DASSERT(containingBoard);

  m->squarePositions[cSquareIntPositions] = 0;
  m->board = containingBoard;

  for(x = 0; x < cSquareIntPositions; x++)
    m->squarePositions[x] = 0;    
}

void ctor_FastSetPiece1(FastSetPiece* m, FastSetPiece* other)
{
  int x;
  m->squarePositions[cSquareIntPositions] = other->squarePositions[cSquareIntPositions];
  
  for(x = 0; x < cSquareIntPositions; x++)
    other->squarePositions[x] = m->squarePositions[x];

  m->board = other->board;
}

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetPiece* m, Square* square)
{
  //First checks if it already exists    
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < cSquarePositions);

  {
    int x;
    for(x = 0; x < m->squarePositions[cSquareIntPositions]; x++)    
      if(Loc_Equ(&Board_GetSqr(m->board, m->squarePositions, x)->location, &square->location))
        return;
  }

  FastSetPiece_setAt(m, m->squarePositions[cSquareIntPositions], square);
  m->squarePositions[cSquareIntPositions]++;
  
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < cSquarePositions);
}
 
int FastSetPiece_length(FastSetPiece* m)
{
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < cSquarePositions);
  return m->squarePositions[cSquareIntPositions];
}

Square* FastSetPiece_getAt(FastSetPiece* m, int pos)
{
  return Board_GetSqr(m->board, m->squarePositions, pos);
}

void FastSetPiece_setAt(FastSetPiece* m, int pos, Square* square)
{
  Board_SetSqr(m->board, m->squarePositions, pos, square);

  DASSERT((FastSetPiece_getAt(m, pos) == square));
}

Square* FastSetPiece_getAtOtherBoard(FastSetPiece* m, int pos, Board* board)
{  
  return Board_GetSqr(board, m->squarePositions, pos);
}

//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Square** operator++ ( Square**

void FastSetPiece_clear(FastSetPiece* m)
{
  m->squarePositions[cSquareIntPositions] = 0;
}

int FastSetPiece_size(FastSetPiece* m)
{
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < cSquarePositions);
  return m->squarePositions[cSquareIntPositions];
}

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetPiece* m, int pos)
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

void FastSetPiece_erase(FastSetPiece* m, Square* piece)
{
  int pos = 0;

  DASSERT(m);
  DASSERT(piece);

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

bool FastSetPiece_contains(FastSetPiece* m, Square* piece)
{
  int x;
  for(x = 0; x < m->squarePositions[cSquareIntPositions]; x++)    
    if(Loc_Equ(&FastSetPiece_getAt(m, x)->location, &piece->location))
      return true;
  return false;
}
*/