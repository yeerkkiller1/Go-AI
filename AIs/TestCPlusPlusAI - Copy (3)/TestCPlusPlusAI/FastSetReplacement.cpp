#include "FastSetReplacement.h"

#include "Constants.h"
#include "Piece.h"

#include "q.h"

#include <string.h>

void ctor_FastSetPiece(FastSetPiece* m)
{
  m->curDataHead = 0;
}

void ctor_FastSetPiece(FastSetPiece* m, FastSetPiece* other)
{
  if(!COPY_ALLOWED)
  {
    //Are you sure you want to copy it?
    int breakHere = 0;
  }
    
  m->curDataHead = other->curDataHead;

  memcpy(other->data, m->data, sizeof(Piece) * m->curDataHead);  
}

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetPiece* m, Piece* piece)
{
  //First checks if it already exists    

#ifdef DEBUG_LEVEL_2
#ifndef C
  for(int x = 0; x < m->curDataHead; x++)
  {
    if(m->data[x]->location.x < 0 || m->data[x]->location.x > 8)
    {
      throw exception("location in data is corrupt");
    }
  }

  if(m->curDataHead > 81 || m->curDataHead < 0)
  {
    throw exception("curDataHead is corrupt");
  }
#endif
#endif      

  int x;
  for(x = 0; x < m->curDataHead; x++)    
    if(Loc_Equ(&m->data[x]->location, &piece->location))
      return;


  m->data[m->curDataHead++] = piece;
    
#ifdef DEBUG_LEVEL_2
#ifndef C
  for(int x = 0; x < m->curDataHead; x++)
  {
    if(m->data[x]->location.x < 0 || m->data[x]->location.x > 8)
    {
      throw exception("location in data is corrupt");
    }
  }
#endif
#endif
}
  
Piece** FastSetPiece_begin(FastSetPiece* m) 
{
  return &m->data[0];
}

Piece** FastSetPiece_end(FastSetPiece* m)
{
  return &m->data[m->curDataHead];
}
    
//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Piece** operator++ ( Piece**

void FastSetPiece_clear(FastSetPiece* m)
{
  m->curDataHead = 0;
}

int FastSetPiece_size(FastSetPiece* m)
{
#ifdef DEBUG_LEVEL_1
  if(m->curDataHead < 0 || m->curDataHead > TOTAL_SQUARES)
    throw "impossible set size, likely uninitialized";
#endif
  return m->curDataHead;
}

//Should be faster than erase(Piece* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetPiece* m, int pos)
{  
  //Move everything being this pos forward by one

  while(pos + 1 < m->curDataHead)
  {
    m->data[pos] = m->data[pos + 1];
  }
  m->curDataHead--;
}

void FastSetPiece_erase(FastSetPiece* m, Piece* piece)
{
  int pos = 0;

  //We also have to find it
  while(pos < m->curDataHead && m->data[pos] != piece)
  {
    pos++;
  }
  //Move everything being this pos forward by one

  //Only subtract if we found it!
  if(pos != m->curDataHead)
  {
    m->curDataHead--;
  }

  while(pos < m->curDataHead)
  {
    m->data[pos] = m->data[pos + 1];
    pos++;
  }    
} 

bool FastSetPiece_contains(FastSetPiece* m, Piece* piece)
{
  int x;
  for(x = 0; x < m->curDataHead; x++)    
    if(Loc_Equ(&m->data[x]->location, &piece->location))
      return true;
  return false;
}