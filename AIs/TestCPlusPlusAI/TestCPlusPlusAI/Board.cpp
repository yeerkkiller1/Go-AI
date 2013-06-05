#include "Square.h"
#include "Board.h"

#include "libertyGroup.h"
#include "Location.h"
#include "q.h"

void ctor_Board0(Board* m)
{  
  //While we don't need to allocate anything, we need to hook some pointers up
  
  int y = 0;
  int x = 0;
  
  for(x = 0; x < BOARD_SIZE; x++)
  {
    for(y = 0; y < BOARD_SIZE; y++)
    {
      ctor_Square0(BOARD(m, x, y), m, x, y);
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
    for(x = 0; x < BOARD_SIZE; x++)
    {
      for(y = 0; y < BOARD_SIZE; y++)
      {
        Square* otherPiece = BOARD(other, x, y);
        Square* curSquare = BOARD(m, x, y);

        //location
        ctor_Square0(curSquare, m, x, y);
        curSquare->pieceTypeCur = otherPiece->pieceTypeCur;
        curSquare->hadBlack = otherPiece->hadBlack;
        curSquare->hadWhite = otherPiece->hadWhite;
            
        //fix up all ownedGroup (as every group is owned)
        //and then hook up group to the appropriate ownedGroup        
      }
    }
   
    for(x = 0; x < BOARD_SIZE; x++)
    {
      for(y = 0; y < BOARD_SIZE; y++)
      {
        Square* otherPiece = BOARD(other, x, y);
        Square* curSquare = BOARD(m, x, y);
        
        FastSetPiece_clear(curSquare->group);
        FastSetPiece_clear(curSquare->liberties);

        {
          int z;
          unsigned char* otherGroup = otherPiece->group;
          unsigned char* otherLibs = otherPiece->group;
          for(z = 0; z < FastSetPiece_length(otherGroup); z++)
          {
            FastSetPiece_insert(curSquare->group, FastSetPiece_getAtOtherBoard(otherGroup, z, m));
          }

          for(z = 0; z < FastSetPiece_length(otherGroup); z++)
          {
            FastSetPiece_insert(curSquare->liberties, FastSetPiece_getAtOtherBoard(otherLibs, z, m));
          }    
        }
      }
    }

    for(x = 0; x < BOARD_SIZE; x++)
    {
      for(y = 0; y < BOARD_SIZE; y++)
      {
        if(BOARD(other, x, y)->pieceTypeCur != Empty)
        {
          Loc prevOwnerPos = BOARD(other, x, y)->libGrp->location;

          //group      
          BOARD(m, x, y)->libGrp = BOARD(m, prevOwnerPos.x, prevOwnerPos.y);
        }
      }
    }

  #ifdef DEBUG_LEVEL_2
  #ifndef C
    for(int y = 0; y < BOARD_SIZE; y++)
    {
      for(int x = 0; x < BOARD_SIZE; x++)
      {      
        if(Square_ToString(BOARD(m, x, y)) != Square_ToString(&other->board[x][y]) ||
          Square_Group(BOARD(m, x, y)) != Square_Group(&other->board[x][y]) ||
          Square_LibsInGroup(BOARD(m, x, y)) != Square_LibsInGroup(&other->board[x][y]))
        {
          throw exception("copy of board failed!");
        }
      }
    }
  #endif
  #endif
    
  }
}


Square*	Board_GetPiece		(Board* m, Loc* location)
{
	return BOARD(m, location->x, location->y);
}

Square* Board_SafeAccess (Board* board, int x, int y)
{
  if(x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE)
    return NULL;

  return &board->board[x + y*BOARD_SIZE];
}

void  Board_RemovePiece  (Board* m, Loc location)
{
#ifndef C
  throw std::exception ("not implemented yet, as doing this is much less efficient than just removing an entire group.");
#endif
}

void  Board_RemoveGroup (Board* m, Square* libGrpToRemove)
{
  Pieces pieceTypeCur = libGrpToRemove->pieceTypeCur; //That way we always know what we should be removing

  //Square** pieceToRemove;
  int z;
  unsigned char* grpToRemove = libGrpToRemove->group;
  for(z = 0; z < FastSetPiece_length(grpToRemove); z++)
  {
    //Remove it, and increase all the groups which gain a liberty        
    Loc locToRem = FastSetPiece_getAt(m, grpToRemove, z)->location;

    Square* sqrSur[4];
    int cSqrSur;
    
    //All the surrounding groups which have been accounted for
    int libsRemoved = 0;
    
    //Adjacent liberty increase
    int x;

    cSqrSur = SurroundingSquares(sqrSur, locToRem, m);

    for(x = 0; x < cSqrSur; x++)
    {
      if(sqrSur[x] != NULL)
      {
        //sqrSur[x]->libGrp->liberties.curDataHead = 0;

        //Won't be added twice, as duplicates are not allowed in sets

        //No point in maintaining empty groups
        if(sqrSur[x]->pieceTypeCur != Empty)
        {
          FastSetPiece_insert(sqrSur[x]->libGrp->liberties, FastSetPiece_getAt(m, grpToRemove, z));
        }
      }
    }

    //Keep track of pieces removed
    if(pieceTypeCur == Black)
      m->blackTakenOff++;
    else
      m->whiteTakenOff++;      

    //Reset the square (when it is needed again it will be actually reset
    BOARD(m, locToRem.x, locToRem.y)->pieceTypeCur = Empty;
  }
}

void _CopySrcToDst(Square * src, Square * dst)
{
  //Nothing to do
  if(src == dst)
  {
    return;
  }

  //Go through all of src, first settings all pieces so they instead point
  //to the dst (except for the owner, he is handled last), adding the pieces 
  //to the dst and then afterwards, adding the liberties        
  
  //Add all pieces from src to dst, and repoint their group to the dst group  
  {
    unsigned char* srcGrp = src->group;
    int z;
    for(z = 0; z < FastSetPiece_length(srcGrp); z++)
    {
      FastSetPiece_insert(dst->group, FastSetPiece_getAt(src->board, srcGrp, z));
      FastSetPiece_getAt(src->board, srcGrp, z)->libGrp = dst;
    }
  }

  //Add all of their liberties to dst also
  {
    unsigned char* srcLib = src->liberties;
    int z;
    for(z = 0; z < FastSetPiece_length(srcLib); z++)
    {
      FastSetPiece_insert(dst->liberties, FastSetPiece_getAt(src->board, srcLib, z));
    }
  }

  //Clean up src
  FastSetPiece_clear(src->group);
  FastSetPiece_clear(src->liberties);
  src->libGrp = dst;
}

void	Board_PlayPiece	(Board* m, Pieces type, Loc location)
{  
  Square* sqrSur[4];
  int cSqrSur; 
  int x; 
  Square* curfriendly;

  //Play actual piece
  Square* curSquare = Board_GetPiece(m, &location);
  
  DASSERT(curSquare->pieceTypeCur == Empty);
  
  curSquare->pieceTypeCur = type;

  curSquare->hadBlack |= Square_IsBlack(curSquare); //We set these on place
  curSquare->hadWhite |= Square_IsWhite(curSquare);
  //Advance turn
  if(m->curTurn == type)
  {
    if(type == Black)
      m->curTurn = White;
    else
      m->curTurn = Black;
  }

  //Make cur liberty group, and fill it with our liberties
  //As our curSquare is empty, we can use its ownedGroup
	curfriendly = curSquare;  
  curSquare->libGrp = curSquare;
 
  FastSetPiece_clear(curSquare->group);
  
  FastSetPiece_clear(curSquare->liberties);
  FastSetPiece_insert(curfriendly->group, curSquare);
   
  cSqrSur = SurroundingSquares(sqrSur, location, m);

  //Fill our liberties up, and take away ourself as a liberty from surrounding squares
  for(x = 0; x < cSqrSur; x++)
  {
    if(sqrSur[x] != NULL)
    {
      if(sqrSur[x]->pieceTypeCur == Empty)
      {
        FastSetPiece_insert(curfriendly->liberties, sqrSur[x]);
      }
      else
      {
        sqrSur[x]->libGrp->hadBlack = true;
        FastSetPiece_erase(sqrSur[x]->libGrp->liberties, curSquare);
      }
    }
  } //End make cur liberty group
  //If we make the group dead, we handle it later (unless it is friendly, then prune failed and we broke the rules)
   
  //Merge all friendly groups into 1 big group (keeping liberties),
  //and subtracting from enemy liberties
  for(x = 0; x < cSqrSur; x++)
  {	    
    if(sqrSur[x] != NULL)
    {
      Square* group = sqrSur[x]->libGrp;
		
		  if(sqrSur[x]->pieceTypeCur != Empty)
		  {      
			  //friendly
			  if(group->pieceTypeCur == type)
			  {          
          Square* src;
          Square* dst;
        
          //group into curfriendly
          if(FastSetPiece_size(group->group) < FastSetPiece_size(curfriendly->group))
          {
            src = group;
            dst = curfriendly;
          }
          else //curfriendly into group
          {
            src = curfriendly;
            dst = group;
          }
          
          _CopySrcToDst(src, dst);

          curfriendly = dst;          
        }
        else if (Square_Opposite(group) == type)
			  {
				  //If it died remove it (we already took away the liberty from it before
          if(FastSetPiece_size(group->liberties) == 0)
            Board_RemoveGroup(m, group);
			  }
		  }
    }
  } //end of surrounding pieces
}

int SurroundingSquares(Square** outputs, Loc location, Board* board)
{       
  //Loc tempLoc;
  int numOutputs = 0;
  //*pNumOutputs = 0;

  outputs[0] = NULL;
  outputs[1] = NULL;
  outputs[2] = NULL;
  outputs[3] = NULL;
  
  //Make sure this is not coping anything (except location)
  if(location.x + 1 < BOARD_SIZE)
  {
    outputs[0] = BOARD(board, location.x + 1, location.y);
    numOutputs = numOutputs + 1;
  }
  //Board_GetPiece(board, ctor_Loc1(&tempLoc, location.x + 1, location.y));		

	if(location.x - 1 >= 0)	
  {
    outputs[1] = BOARD(board, location.x - 1, location.y);
    numOutputs = numOutputs + 1;
  }

	if(location.y + 1 < BOARD_SIZE)	
  {
    outputs[2] = BOARD(board, location.x, location.y + 1);
    numOutputs = numOutputs + 1;
  }

	if(location.y - 1 >= 0)	
  {
    outputs[3] = BOARD(board, location.x, location.y - 1);
    numOutputs = numOutputs + 1;
  }

  return 4;
}


#include "IntStream.h"
void Board_ToString
(
  Board* m, 
__global int* intStream,
  int* pCharPos
)
{
  
  WriteStringToIntStream(intStream, pCharPos, " 012345678\n");

  {
    unsigned char y, x;
    for(y = 0; y < BOARD_SIZE; y++)
    {
      WriteCharToIntStream(intStream, pCharPos, (unsigned char)(y + 48));
      for(x = 0; x < BOARD_SIZE; x++)
      {
        Square_ToString(BOARD(m, x, y), intStream, pCharPos);
      }
      WriteCharToIntStream(intStream, pCharPos, '\n');
    }
  }
  
}

void Board_MetaToString
(
  Board* m, 
__global int* intStream,
  int* pCharPos
)
{
  WriteStringToIntStream(intStream, pCharPos, " 012345678\n");

  {
    int y, x;
    for(y = 0; y < BOARD_SIZE; y++)
    {
      WriteCharToIntStream(intStream, pCharPos, (char)(y + 48));
      for(x = 0; x < BOARD_SIZE; x++)
      {
        if(BOARD(m, x, y)->pieceTypeCur == Empty)
        {
          WriteCharToIntStream(intStream, pCharPos, '0');
        }
        else
        {        
          char groupSize = (char)(FastSetPiece_size(BOARD(m, x, y)->libGrp->group) + 48);
          WriteCharToIntStream(intStream, pCharPos, groupSize);
        }
      }
      WriteCharToIntStream(intStream, pCharPos, '\n');
    }
  }
}

void Board_LibsToString
(
  Board* m, 
__global int* intStream,
  int* pCharPos
) 
{
  WriteStringToIntStream(intStream, pCharPos, " 012345678\n");

  {
    int y, x;
    for(y = 0; y < BOARD_SIZE; y++)
    {
      WriteCharToIntStream(intStream, pCharPos, (char)(y + 48));
      for(x = 0; x < BOARD_SIZE; x++)      
      {
        if(BOARD(m, x, y)->pieceTypeCur == Empty)
        {
          WriteCharToIntStream(intStream, pCharPos, '0');
        }
        else
        {
          char libertyCount = (char)(FastSetPiece_size(BOARD(m, x, y)->libGrp->liberties) + 48);
          WriteCharToIntStream(intStream, pCharPos, libertyCount);
        }
      }
      WriteCharToIntStream(intStream, pCharPos, '\n');
    }
  }
}

Square* Board_GetSqr(Board* m, unsigned char* positions, int squarePos)
{  
  unsigned char squarePosition = positions[squarePos];

  int x = squarePosition >> 4;
  //Really unoptimimum
  int y = squarePosition % (1 << 4);
  
  DASSERT(BOARD(m, x, y));
  
  //Or else we don't match compress position../
  DASSERT(squarePosition == Loc_CompressPosition(x, y));

  return BOARD(m, x, y);
}

void Board_SetSqr(Board* m, unsigned char* positions, int squarePos, Square* square)
{
  unsigned char charValue = Loc_CompressPosition(square->location.x, square->location.y);

  positions[squarePos] = charValue;
}