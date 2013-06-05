#pragma once

typedef struct FastSetPiece FastSetPiece;

#include "Constants.h"

struct Piece;
//As its a set, it will never contain duplicates (no matter what is done to it)

//Piece** refers to where it is stored in its current storage location
//Piece* refers to where it is on the board
//Piece refers to what it is on the board

struct FastSetPiece // : public std::set<Piece *>
{
  //Main data storage, never needs to be increased as we cannot have duplicates and
  //the board is only so big!

  Piece* data[TOTAL_SQUARES];  
  int curDataHead;
};

void ctor_FastSetPiece(FastSetPiece* m);
void ctor_FastSetPiece(FastSetPiece* m, FastSetPiece* other);

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetPiece* m, Piece* piece);
  
Piece** FastSetPiece_begin(FastSetPiece* m);

Piece** FastSetPiece_end(FastSetPiece* m);
    
//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Piece** operator++ ( Piece**

void FastSetPiece_clear(FastSetPiece* m);

int FastSetPiece_size(FastSetPiece* m);


//Should be faster than erase(Piece* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetPiece* m, Piece** piece);
  
void FastSetPiece_erase(FastSetPiece* m, Piece* piece);

bool FastSetPiece_contains(FastSetPiece* m, Piece* piece);

#include "Piece.h"

//Old FastSet template, we cannot have templates...
//but if we want to use this like a template we can turn
//it into a macro
/*

template <class T>
class FastSet // : public std::set<Piece *>
{
  //Main data storage, never needs to be increased as we cannot have duplicates and
  //the board is only so big!

private:
  T* data[TOTAL_SQUARES];  
  int curDataHead;

public:
  FastSet() : curDataHead(0)
  {

  }

  FastSet( FastSet& other) : curDataHead(other.curDataHead)
  {
    if(!COPY_ALLOWED)
    {
      //Are you sure you want to copy it?
      int breakHere = 0;
    }
    
    for(int x = 0; x < curDataHead; x++)
      data[x] = other.data[x];
  }

  FastSet operator= ( FastSet& other)
  {
    if(!COPY_ALLOWED)
    {
      //Are you sure you want to copy it?
      int breakHere = 0;
    }
    
    curDataHead = other.curDataHead;

    for(int x = 0; x < curDataHead; x++)
      data[x] = other.data[x];
  }

  //If you need to return bool, but for now it is not used so it is not implemented
  void insert(T* piece)
  {
    //First checks if it already exists    

    for(int x = 0; x < curDataHead; x++)
    {
      if(data[x]->location.x < 0 || data[x]->location.x > 8)
      {
        int bad = 0;
      }
    }
    
    if(curDataHead >= 70)
    {
      int breakHere = 0;
    }

    for(int x = 0; x < curDataHead; x++)    
      if(data[x] == piece)
        return;


    data[curDataHead++] = piece;
    
    for(int x = 0; x < curDataHead; x++)
    {
      if(data[x]->location.x < 0 || data[x]->location.x > 8)
      {
        int bad = 0;
      }
    }
  }
  
  T** begin() 
  {
    return &data[0];
  }
  T** begin()
  {
    return &data[0];
  }

  T** end() 
  {
    return &data[curDataHead];
  }
  T** end()
  {
    return &data[curDataHead];
  }
    
  //no need to overload it, if we increment the pointer by 1 it performs the desired functionality
  //friend Piece** operator++ ( Piece**

  void clear()
  {
    curDataHead = 0;
  }

  int size()
  {
    return curDataHead;
  }

  //Should be faster than erase(Piece* piece)
  void erase(T** piece)
  {
    int pos = (piece - &(data[0]));
    //Move everything being this pos forward by one

    while(pos + 1 < curDataHead)
    {
      data[pos] = data[pos + 1];
    }
    curDataHead--;
  }
  
  void erase(T* piece)
  {
    int pos = 0;

    //We also have to find it
    while(pos < curDataHead && data[pos] != piece)
    {
      pos++;
    }
    //Move everything being this pos forward by one

    //Only subtract if we found it!
    if(pos != curDataHead)
    {
      curDataHead--;
    }

    while(pos < curDataHead)
    {
      data[pos] = data[pos + 1];
      pos++;
    }    
  }  
  
  //and move ructor
  

};
  */