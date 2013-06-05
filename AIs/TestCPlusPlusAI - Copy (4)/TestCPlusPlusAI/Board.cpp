#include "Piece.h"
#include "Board.h"

#include "libertyGroup.h"
#include "Location.h"
#include "q.h"

#ifndef OPEN_CL
  #include <stdlib.h>
  #include <string.h>
#endif



void ctor_Board0(Board* m)
{  
  //While we don't need to allocate anything, we need to hook some pointers up
  
  int y = 0;
  int x = 0;

  for(y = 0; y < BOARD_SIZE; y++)
  {
    for(x = 0; x < BOARD_SIZE; x++)
    {
      ctor_Piece0(&m->board[x][y]);
      ctor_Loc1(&m->board[x][y].location, x,y);     
      m->board[x][y].libGroup = &(m->board[x][y].ownedLibertyGroup);
      m->board[x][y].libGroup->owner = &m->board[x][y];
    }
  }
  
 
  m->whiteTakenOff = 0;
  m->blackTakenOff = 0;
  m->curTurn = Black;
}

void ctor_Board1(Board* m, Board* other )
{    
  m->whiteTakenOff = other->whiteTakenOff;
  m->blackTakenOff = other->blackTakenOff;
  m->curTurn = other->curTurn;
  {
    //While we don't need to allocate anything, we need to hook some pointers up
    int y;
    int x;
    for(y = 0; y < BOARD_SIZE; y++)
    {
      for(x = 0; x < BOARD_SIZE; x++)
      {
        Piece* otherPiece = &other->board[x][y];
        Piece* curPiece = &m->board[x][y];

        //location
        ctor_Loc1(&curPiece->location, x, y);
        curPiece->pieceTypeCur = otherPiece->pieceTypeCur;
        curPiece->hadBlack = otherPiece->hadBlack;
        curPiece->hadWhite = otherPiece->hadWhite;
            
        //fix up all ownedLibertyGroup (as every group is owned)
        //and then hook up libGroup to the appropriate ownedLibertyGroup

        curPiece->ownedLibertyGroup.owner = curPiece;
      }
    }
   
    for(y = 0; y < BOARD_SIZE; y++)
    {
      for(x = 0; x < BOARD_SIZE; x++)
      {
        Piece* otherPiece = &other->board[x][y];
        Piece* curPiece = &m->board[x][y];
        
        FastSetPiece_clear(&curPiece->ownedLibertyGroup.group);
        FastSetPiece_clear(&curPiece->ownedLibertyGroup.liberties);

        {
          Piece** pieceInGroup;
          for(pieceInGroup = FastSetPiece_begin(&otherPiece->ownedLibertyGroup.group);
            pieceInGroup != FastSetPiece_end(&otherPiece->ownedLibertyGroup.group); pieceInGroup++)
          {
              FastSetPiece_insert(&curPiece->ownedLibertyGroup.group, &m->board[(*pieceInGroup)->location.x][(*pieceInGroup)->location.y]);
          }
        
          for(pieceInGroup = FastSetPiece_begin(&otherPiece->ownedLibertyGroup.liberties);
            pieceInGroup != FastSetPiece_end(&otherPiece->ownedLibertyGroup.liberties); pieceInGroup++)
          {
            FastSetPiece_insert(&curPiece->ownedLibertyGroup.liberties, &m->board[(*pieceInGroup)->location.x][(*pieceInGroup)->location.y]);
          }
        }
      }
    }

    for(y = 0; y < BOARD_SIZE; y++)
    {
      for(x = 0; x < BOARD_SIZE; x++)
      {
        Loc prevOwnerPos = other->board[x][y].libGroup->owner->location;

        //libGroup      
        m->board[x][y].libGroup = &m->board[prevOwnerPos.x][prevOwnerPos.y].ownedLibertyGroup;            
      }
    }

  #ifdef DEBUG_LEVEL_2
  #ifndef C
    for(int y = 0; y < BOARD_SIZE; y++)
    {
      for(int x = 0; x < BOARD_SIZE; x++)
      {      
        if(Piece_ToString(&m->board[x][y]) != Piece_ToString(&other->board[x][y]) ||
          Piece_Group(&m->board[x][y]) != Piece_Group(&other->board[x][y]) ||
          Piece_LibsInGroup(&m->board[x][y]) != Piece_LibsInGroup(&other->board[x][y]))
        {
          throw exception("copy of board failed!");
        }
      }
    }
  #endif
  #endif
    
  }
}


Piece*	Board_GetPiece		(Board* m, Loc* location)
{
#ifndef C
	if(!Loc_IsOnBoard(location))
		throw std::exception("placing off board");
#endif

	return &(m->board[location->x][location->y]);
}
//Just allows GetPiece to be passed as a function pointer to a template (as the return parameter cannot
//be templated apparently, this gets around that).

void  Board_RemovePiece  (Board* m, Loc location)
{
#ifndef C
  throw std::exception ("not implemented yet, as doing this is much less efficient than just removing an entire group.");
#endif
}

//Really just gets liberties from the piece
libertyGroup*	Board_GetLibertyGroup		(Board* m, Loc location)
{
#ifndef C
  if(!Loc_IsOnBoard(&location))
		throw std::exception("liberty group off board");
#endif

	return (m->board[location.x][location.y].libGroup);
}

void  Board_RemoveGroup (Board* m, libertyGroup* libGrpToRemove)
{
  Pieces pieceTypeCur = libGrpToRemove[0].owner->pieceTypeCur; //That way we always know what we should be removing

  Piece** pieceToRemove;
  for(pieceToRemove = FastSetPiece_begin(&libGrpToRemove->group);
    pieceToRemove != FastSetPiece_end(&libGrpToRemove->group); pieceToRemove++)
  {
#ifndef C
    if(pieceTypeCur != (*pieceToRemove)->pieceTypeCur)
    {
      throw std::exception ("group to remove not uniform");
    }
#endif

    //Remove it, and increase all the groups which gain a liberty        
    Loc locToRem = (*pieceToRemove)->location;

    Piece* sqrSur[4];
    int cSqrSur = SurroundingSquares(sqrSur, locToRem, m); 
    
    //All the surrounding groups which have been accounted for    
    int libsRemoved = 0;
    
    //Adjacent liberty increase
    int x;
    for(x = 0; x < cSqrSur; x++)
    {
      //Won't be added twice, as duplicates are not allowed in sets
      FastSetPiece_insert(&sqrSur[x]->libGroup->liberties, *pieceToRemove);
    }

    //Keep track of pieces removed
    if(pieceTypeCur == Black)
      m->blackTakenOff++;
    else
      m->whiteTakenOff++;      

    //Reset the square    
    m->board[locToRem.x][locToRem.y].pieceTypeCur = Empty;

    //Reset our liberty group, unless we are the owner then we stick around as we are needed!
    if(m->board[locToRem.x][locToRem.y].libGroup != &(m->board[locToRem.x][locToRem.y].ownedLibertyGroup))
    {
      m->board[locToRem.x][locToRem.y].libGroup = &(m->board[locToRem.x][locToRem.y].ownedLibertyGroup);
      FastSetPiece_clear(&m->board[locToRem.x][locToRem.y].libGroup->group); //Clean it up so its accurate
      FastSetPiece_clear(&m->board[locToRem.x][locToRem.y].libGroup->group);
    }
  }
    
  //This was explicitedly not cleared in the loop, so we clear it now as it is no longer needed
  //(and the owner is already pointing to the correct libertyGroup, its own, and it now has
  //the correct liberty information).
  FastSetPiece_clear(&libGrpToRemove->group);
  FastSetPiece_clear(&libGrpToRemove->liberties);
}


void	Board_PlayPiece	(Board* m, Pieces type, Loc location)
{  
#ifndef C
	if(type == Empty)
		throw std::exception("placing empty piece");

	if(Board_GetPiece(m, &location)->pieceTypeCur != Empty)
		throw std::exception("placing over piece");

  //Redundant
	if(!Loc_IsOnBoard(&location))
		throw std::exception("placing off board");
#endif

  //Play actual piece
  Piece* curPiece = Board_GetPiece(m, &location);
  curPiece->pieceTypeCur = type;  
  curPiece->hadBlack |= Piece_IsBlack(curPiece); //We set these on place
  curPiece->hadWhite |= Piece_IsWhite(curPiece);

  //Advance turn
  if(m->curTurn != type)  
  {
    int why = 0; //?why, it may be needed but you are likely just doing something wrong  
  }
  else    
  {
    m->curTurn = type == Black ? White : Black;  
  }


#ifndef C
  //May flip-flip around a lot when combining, as when combining
  //we set this to the largest, and then add the other
  if(curPiece->libGroup != &curPiece->ownedLibertyGroup)
  {
    throw exception ("empty piece should have its libGroup as its ownedLibertyGroup");
  }
#endif

  //Make cur liberty group, and fill it with our liberties
  {
    Piece* sqrSur[4]; int cSqrSur; int x;

	  libertyGroup* curfriendly = curPiece->libGroup;
    FastSetPiece_insert(&curfriendly->group, curPiece);
      
    cSqrSur = SurroundingSquares(sqrSur, location, m);    
          
    //Fill our liberties up, and take away ourself as a liberty from surrounding squares

    for(x = 0; x < cSqrSur; x++)
    {
      if(sqrSur[x]->pieceTypeCur == Empty)    
      {
        FastSetPiece_insert(&curfriendly->liberties, sqrSur[x]);
      }
      else
      { 
        FastSetPiece_erase(&sqrSur[x]->libGroup->liberties, curPiece);
      }
    } //End make cur liberty group
    //If we make the group dead, we handle it later (unless it is friendly, then prune failed and we broke the rules)   
	
    //Merge all friendly groups into 1 big group (keeping liberties),
    //and subtracting from enemy liberties
    for(x = 0; x < cSqrSur; x++)
    {	
      Piece* curPiece = sqrSur[x];
      libertyGroup* group = curPiece->libGroup;
		
		  if(FastSetPiece_size(&group->group) > 0)
		  {      
			  //friendly
			  if(curPiece->libGroup->owner->pieceTypeCur == type)	       
			  {		        
          libertyGroup * src;
          libertyGroup * dst;
        
          //group into curfriendly
          if(FastSetPiece_size(&group->group) < FastSetPiece_size(&curfriendly->group))
          {
            src = group;
            dst = curfriendly;
          }
          else //curfriendly into group
          {
            src = group;
            dst = curfriendly;
          }

          //Nothing to do
          if(src == dst)
          {
            continue;
          }

          //Go through all of src, first settings all pieces so they instead point
          //to the dst (except for the owner, he is handled last), adding the pieces 
          //to the dst and then afterwards, adding the liberties        

          //Add pieces to dst and point the libGroup they are part of as dst
          {
            Piece** pieceSrc;
            for(pieceSrc = FastSetPiece_begin(&src->group);
              pieceSrc != FastSetPiece_end(&src->group); pieceSrc++)
            {
              FastSetPiece_insert(&dst->group, *pieceSrc);
              //If its not the owner
              if((*pieceSrc)->libGroup != &(*pieceSrc)->ownedLibertyGroup)
                (*pieceSrc)->libGroup = dst;
            }
          }

          //Add all of their liberties to dst also
          {
            Piece** pieceSrcLib;
            for(pieceSrcLib = FastSetPiece_begin(&src->liberties);
              pieceSrcLib != FastSetPiece_end(&src->liberties); pieceSrcLib++)
            {
              FastSetPiece_insert(&dst->liberties, *pieceSrcLib);
            }
          }

          //Clean up src
          FastSetPiece_clear(&src->group);
          FastSetPiece_clear(&src->liberties);
          src->owner->libGroup = dst;          
        }
        else if (Piece_Opposite(group->owner) == type)
			  {
				  //If it died remove it (we already took away the liberty from it before
          if(FastSetPiece_size(&group->liberties) == 0)
            Board_RemoveGroup(m, group);
			  }
			  else
			  {
  #ifndef C
				  throw std::exception("group contains empty squares");
  #endif
			  }			
		  } //End loop of surrounding //End merge friendly group
    } 
  } //end of surrounding pieces
}

char* Board_ToString(Board* m) 
{
  char* toReturn = (char*)malloc(1000);
  int curPos = 0;

  memcpy(toReturn, " 012345678\n", strlen(" 012345678\n"));
  curPos += strlen(" 012345678\n");

  {
    int y, x;
    for(y = 0; y < BOARD_SIZE; y++)
    {
      toReturn[curPos++] = (char)(y + 48);
      for(x = 0; x < BOARD_SIZE; x++)
      {
        char* pieceTemp = Piece_ToString(&m->board[x][y]);
        memcpy(toReturn + curPos, pieceTemp, strlen(pieceTemp));
        curPos += strlen(pieceTemp);
        //delete pieceTemp;
      }
      toReturn[curPos++] = '\n';
    }
    toReturn[curPos++] = '\0';
    return toReturn;
  }
}

char* Board_MetaToString(Board* m) 
{
  char* toReturn = (char*)malloc(1000);
  int curPos = 0;

  memcpy(toReturn, " 012345678\n", strlen(" 012345678\n"));
  curPos += strlen(" 012345678\n");

  {
    int y, x;
    for(y = 0; y < BOARD_SIZE; y++)
    {
      toReturn[curPos++] = (char)(y + 48);
      for(x = 0; x < BOARD_SIZE; x++)
      {
        toReturn[curPos++] = (char)(FastSetPiece_size(&m->board[x][y].libGroup->group) + 48);
      }
      toReturn[curPos++] = '\n';
    }
    toReturn[curPos++] = '\0';
    return toReturn;
  }
}

char* Board_LibsToString(Board* m) 
{
  char* toReturn = (char*)malloc(1000);
  int curPos = 0;

  memcpy(toReturn, " 012345678\n", strlen(" 012345678\n"));
  curPos += strlen(" 012345678\n");

  {
    int y, x;
    for(y = 0; y < BOARD_SIZE; y++)
    {
      toReturn[curPos++] = (char)(y + 48);
      for(x = 0; x < BOARD_SIZE; x++)      
      {
        toReturn[curPos++] = (char)(FastSetPiece_size(&m->board[x][y].libGroup->liberties) + 48);
      }
      toReturn[curPos++] = '\n';
    }
    toReturn[curPos++] = '\0';
    return toReturn;
  }
}

