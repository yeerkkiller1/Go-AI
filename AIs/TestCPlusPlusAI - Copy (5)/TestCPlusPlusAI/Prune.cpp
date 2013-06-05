#include "Prune.h"
#include "Board.h"
#include "q.h"

//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board* board, Pieces type, Loc location)
{
  Piece* pieceCur = Board_GetPiece(board, &location);
  
  {
    //Prevent placing where you previously got taken
    if(pieceCur->hadBlack || pieceCur->hadWhite)
      return false;
  }

  {
    //Prevent placing when you have TOO many friendly and no threats
    int friendly = 0;
    int enemy = 0;
    int empty = 0;

    Piece* sqrSur[4];
    int cSqrSur = SurroundingSquares(sqrSur, location, board);

    int x;
    for(x = 0; x < cSqrSur; x++)
    {
      if(sqrSur[x]->pieceTypeCur == type)    
        friendly++;
      else if(sqrSur[x]->pieceTypeCur != Empty && Piece_Opposite(sqrSur[x]) == type)
        enemy++;
      else
        empty++;
    }  

    //Check of current against optimized non copy version
  #ifdef DEBUG_LEVEL_1
  #ifndef C
    Board copy;
    ctor_Board1(&copy, board);
    Board_PlayPiece(&copy, type, location);

    int copyLiberties = FastSetPiece_size(&Board_GetLibertyGroup(&copy, location)->liberties);
    bool isSuicide = IsSuicide(board, type, location, friendly, enemy, empty, sqrSur, cSqrSur, pieceCur);

    if((copyLiberties == 0) != isSuicide)
    {      
      Board copy2;
      ctor_Board1(&copy2, board);
      Board_PlayPiece(&copy2, type, location);

      copyLiberties = FastSetPiece_size(&Board_GetLibertyGroup(&copy2, location)->liberties);
      isSuicide = IsSuicide(board, type, location, friendly, enemy, empty, sqrSur, cSqrSur, pieceCur);

      throw exception ("copy not same as previous algorithm... error in optimization");    
    }    
  #endif
  #endif

    if(IsSuicide(board, type, location, friendly, enemy, empty, sqrSur, cSqrSur, pieceCur))
    {
      return false;
    }
  
    //If friendly is all but 1
    if(enemy == 0 && friendly >= (cSqrSur - 1))
      return false;

    return true;
  }
}

//Returns true if it is suicide (so completely not allowed)
bool IsSuicide(Board* board, Pieces type, Loc location, int friendly, int enemy, int empty, Piece* sqrSur[], int cSqrSur, Piece* pieceCur)
{  
  int x;
  //It cannot be suicide if we have empty squares
  if(empty != 0)
    return false;
      
  {    
    //It cannot be suicide if any of our friendly neighbours have more than 2 liberties      
    if(friendly >= 0)
      for(x = 0; x < cSqrSur; x++)
        if(sqrSur[x]->pieceTypeCur == type)
          if(FastSetPiece_size(&sqrSur[x]->libGroup->liberties) >= 2)
            return false;

    //It cannot be suicide if we kill any enemy groups
    if(enemy >= 0)
      for(x = 0; x < cSqrSur; x++)
        if(sqrSur[x]->pieceTypeCur != Empty && Piece_Opposite(sqrSur[x]) == type)
          if(FastSetPiece_size(&sqrSur[x]->libGroup->liberties) == 1) //Then we will kill them
            return false;
  }

  //We have not filtered out, so we just need to start creating the liberty group we would
  //have if we placed, and then see if it is big enough (we stop early, eventually we might
  //be smarter and analysis if the liberty group is good or optimize it down to simply keeping
  //track of one liberty)
  {
    FastSetPiece libertiesFound;  
    ctor_FastSetPiece0(&libertiesFound);

    for(x = 0; x < cSqrSur; x++)
    {
      if(sqrSur[x]->pieceTypeCur == type)
      {
        FastSetPiece* libs = &sqrSur[x]->libGroup->liberties;
        Piece** lib;
        for(lib = FastSetPiece_begin(libs); lib != FastSetPiece_end(libs); lib++)
        {
          if((*lib) != pieceCur)
          {
            FastSetPiece_insert(&libertiesFound, *lib);
            if(FastSetPiece_size(&libertiesFound) >= 2) //Whenever our group gets too big, it cannot be suicide          
              return false;          
          }
        }
      }
      else if(sqrSur[x]->pieceTypeCur == Empty)
      {
        //We already found empty, this is sort of overkill
        FastSetPiece_insert(&libertiesFound, sqrSur[x]);
        if(FastSetPiece_size(&libertiesFound) >= 2) //Whenever our group gets too big, it cannot be suicide          
          return false;  
      }
    }

    return FastSetPiece_size(&libertiesFound) == 0;    
  }
}

