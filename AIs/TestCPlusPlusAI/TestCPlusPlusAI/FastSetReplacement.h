#pragma once

template <class T>
class FastSet;

#include <set>

#include "Constants.h"

//As its a set, it will never contain duplicates (no matter what is done to it)

//Piece** refers to where it is stored in its current storage location
//Piece* refers to where it is on the board
//Piece refers to what it is on the board

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

  FastSet(const FastSet& other) : curDataHead(other.curDataHead)
  {
    if(!Location::COPY_ALLOWED)
    {
      //Are you sure you want to copy it?
      int breakHere = 0;
    }
    
    for(int x = 0; x < curDataHead; x++)
      data[x] = other.data[x];
  }

  FastSet operator= (const FastSet& other)
  {
    if(!Location::COPY_ALLOWED)
    {
      //Are you sure you want to copy it?
      int breakHere = 0;
    }
    
    curDataHead = other.curDataHead;

    for(int x = 0; x < curDataHead; x++)
      data[x] = other.data[x];
  }

  //Still some problems with move semantics, so just don't do it
  /*
  FastSet (FastSet&& other) : 
    curDataHead(other.curDataHead),
    data(other.data)
  {
    curDataHead = other.curDataHead;
  }
  */

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
  
  T*const* begin() const
  {
    return &data[0];
  }
  T** begin()
  {
    return &data[0];
  }

  T*const* end() const
  {
    return &data[curDataHead];
  }
  T** end()
  {
    return &data[curDataHead];
  }
    
  //no need to overload it, if we increment the pointer by 1 it performs the desired functionality
  //friend Piece** operator++ (const Piece**

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
  
  //and move constructor
  

};