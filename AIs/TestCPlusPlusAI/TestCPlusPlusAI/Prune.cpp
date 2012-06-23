#define DEBUG_LEVEL_1

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

  //Prevent placing in immediate suicide  
  //Scope and make a temp copy, and then place and see if it is suicide! (bad... but for not it works)
  bool isSuicide = true;
  {
    Board copy(board);
    copy.PlayPiece(type, location);
    if(copy.GetLibertyGroup(location)->liberties.size() == 0)
    {
      return false;
    }

    /*
    //Eventually screen lots of these calls out... as usually they are actually not needed

    //set<Piece *> libertiesFound;

    set<Piece *> libertiesFound;  
    //It cannot be suicide if we have empty squares
    //if(empty == 0)
    //{      
      //It cannot be suicide if any of our friendly neighbours have more than 2 liberties      
      if(friendly >= 0)
      {
        for_each(begin(pieceAdjs), end(pieceAdjs), [&type, &isSuicide] (Piece* pieceSur) {       
          if(pieceSur->pieceTypeCur == type)
            if(pieceSur->libGroup->liberties.size() >= 2)          
              isSuicide = false;          
        });

        if(!isSuicide)
          goto doSuicideEnd;
      }

      //It cannot be suicide if we kill any enemy groups
      if(enemy >= 0)
      {
        for_each(begin(pieceAdjs), end(pieceAdjs), [&type, &isSuicide] (Piece* pieceSur) {
          if(pieceSur->pieceTypeCur != Empty && pieceSur->Opposite() == type)
            if(pieceSur->libGroup->liberties.size() == 1)
              isSuicide = false;
        });

        if(!isSuicide)
          goto doSuicideEnd;
      }


      
      //We have not filtered out, so we just need to start creating the liberty group we would
      //have if we placed, and then see if it is big enough (we stop early, eventually we might
      //be smarter and analysis if the liberty group is good or optimize it down to simply keeping
      //track of one liberty)

      
      for(auto pieceSur = pieceAdjs.begin(); pieceSur != pieceAdjs.end(); pieceSur++)
      {
        if((*pieceSur)->pieceTypeCur == type)
        {
          set<Piece *> libs = (*pieceSur)->libGroup->liberties;        
          for(auto lib = libs.begin(); lib != libs.end(); lib++)
          {
            if((*lib) != pieceCur)
            {
              libertiesFound.insert(*lib);
              if(libertiesFound.size() >= 2)
              {
                isSuicide = false;
                goto doSuicideEnd;
              }
            }
          }
        }
        else if((*pieceSur)->pieceTypeCur == Empty)
        {
          //We already found empty, this is sort of overkill
          libertiesFound.insert(*pieceSur);
          if(libertiesFound.size() >= 2)
          {
            isSuicide = false;
            goto doSuicideEnd;
          }
        }
      }

      //maybe filter this?
      //if(libertiesFound.size() == 1)
      
      if(libertiesFound.size() == 0)
      {
        isSuicide = true;
        goto doSuicideEnd;
      }            
      else
      {
        isSuicide = false;
      }
    //}
    
doSuicideEnd:
    int nothing = 0;    

    //if(!isSuicide)
      //return false;

//#ifdef DEBUG_LEVEL_1
    Board copy(board);
    copy.PlayPiece(type, location);
    if((copy.GetLibertyGroup(location)->liberties.size() == 0) != isSuicide)
    {      
      std::cout << board.ToString() << endl << endl;
      std::cout << location.x << " " << location.y << endl;
      std::cout << (type == Black ? "b" : "w") << endl;

      std::cout << isSuicide << endl;
      std::cout << copy.GetLibertyGroup(location)->liberties.size() << endl;
      std::cout << libertiesFound.size() << endl;      

      Board copy2(board);
      copy2.PlayPiece(type, location);

      throw exception ("copy not same as previous algorithm... error in optimization");
      //Then its suicide, and we don't place there!
      return false;
    }
    */
//#endif
  }    

  //If friendly is all but 1
  if(enemy == 0 && friendly >= pieceAdjs.size() - 1)
    return false;

  return true;
}