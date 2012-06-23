
#include "Prune.h"
#include "Board.h"
#include "q.h"

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board& board, Pieces type, Location location)
{  
  Piece* pieceCur = board.GetPiece(location);

  //Prevent placing where you previously got taken
  if(pieceCur->hadBlack || pieceCur->hadWhite)
    return false;

  //Prevent placing when you have TOO many friendly and no threats
  int friendly = 0;
  int enemy = 0;
  int empty = 0;

  vector<Piece*> pieceAdjs = SurroundingSquares<Piece>(&Board::GetPieceVoid, location, &board);            
  for_each(begin(pieceAdjs), end(pieceAdjs), [&type, &friendly, &enemy, &empty] (Piece* pieceSur) {       
    if(pieceSur->pieceTypeCur == type)    
      friendly++;    
    else if(pieceSur->pieceTypeCur != Empty && pieceSur->Opposite() == type)
      enemy++;    
    else    
      empty++;    
  });

  //Check of current against optimized non copy version
#ifdef DEBUG_LEVEL_1
  Board copy(board);
  copy.PlayPiece(type, location);
  if((copy.GetLibertyGroup(location)->liberties.size() == 0) != 
    IsSuicide(board, type, location, friendly, enemy, empty, pieceAdjs, pieceCur))
  {      
    throw exception ("copy not same as previous algorithm... error in optimization");    
  }    
#endif

  if(IsSuicide(board, type, location, friendly, enemy, empty, pieceAdjs, pieceCur))
  {
    return false;
  }
  
  //If friendly is all but 1
  if(enemy == 0 && friendly >= (pieceAdjs.size() - 1))
    return false;

  return true;
}

//Returns true if it is suicide (so completely not allowed)
inline bool IsSuicide(Board& board, Pieces type, Location location, int friendly, int enemy, int empty, vector<Piece*> pieceAdjs, Piece* pieceCur)
{          
  //It cannot be suicide if we have empty squares
  if(empty != 0)
    return false;
      
  //It cannot be suicide if any of our friendly neighbours have more than 2 liberties      
  if(friendly >= 0)  
    for(auto pieceSur = pieceAdjs.begin(); pieceSur != pieceAdjs.end(); pieceSur++)    
      if((*pieceSur)->pieceTypeCur == type)
        if((*pieceSur)->libGroup->liberties.size() >= 2)          
          return false;   

  //It cannot be suicide if we kill any enemy groups
  if(enemy >= 0)  
    for(auto pieceSur = pieceAdjs.begin(); pieceSur != pieceAdjs.end(); pieceSur++)    
      if((*pieceSur)->pieceTypeCur != Empty && (*pieceSur)->Opposite() == type)
        if((*pieceSur)->libGroup->liberties.size() == 1) //Then we will kill them
          return false;      

  //We have not filtered out, so we just need to start creating the liberty group we would
  //have if we placed, and then see if it is big enough (we stop early, eventually we might
  //be smarter and analysis if the liberty group is good or optimize it down to simply keeping
  //track of one liberty)
  FastSet<Piece> libertiesFound;  
      
  for(auto pieceSur = pieceAdjs.begin(); pieceSur != pieceAdjs.end(); pieceSur++)
  {
    if((*pieceSur)->pieceTypeCur == type)
    {
      FastSet<Piece>& libs = (*pieceSur)->libGroup->liberties;
      for(auto lib = libs.begin(); lib != libs.end(); lib++)
      {
        if((*lib) != pieceCur)
        {
          libertiesFound.insert(*lib);
          if(libertiesFound.size() >= 2) //Whenever our group gets too big, it cannot be suicide          
            return false;          
        }
      }
    }
    else if((*pieceSur)->pieceTypeCur == Empty)
    {
      //We already found empty, this is sort of overkill
      libertiesFound.insert(*pieceSur);
      if(libertiesFound.size() >= 2) //Whenever our group gets too big, it cannot be suicide          
        return false;  
    }
  }

  return libertiesFound.size() == 0;    
}