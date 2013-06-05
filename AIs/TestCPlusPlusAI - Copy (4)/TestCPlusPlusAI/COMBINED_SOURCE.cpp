//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Board.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Piece.h START
//////////////////////////////////////////////////////////////////////////
#pragma once




#define Pieces  short
#define Black   1 << 5
#define White   1 << 7
#define Empty   1 << 9
#define Marked  1 << 11

//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Location.h START
//////////////////////////////////////////////////////////////////////////
#pragma once

typedef struct Loc Loc;

//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Constants.h START
//////////////////////////////////////////////////////////////////////////
#pragma once

#define BOARD_SIZE 9
#define TOTAL_SQUARES 81

#define ERROR_CHECKING
#define ERR

#ifndef C
  #define C
#endif

#define bool char


#ifndef false
  #define false 0
#endif

#ifndef true
  #define true  1
#endif
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Constants.h END
//////////////////////////////////////////////////////////////////////////


//This is abstracted even though for now it will only be two ints
//in the future it will likely have (static?) helper functions for simple things
//that will be extremely optimized.
struct Loc
{
	int x, y;  	
};

bool Loc_IsOnBoard(Loc* m);

char* Loc_ToString(Loc* m);

bool Loc_Equ(Loc* p1, Loc* p2);
bool Loc_NotEqu (Loc* p1, Loc* p2);   

void ctor_Loc0(Loc* obj);

Loc* ctor_Loc1(Loc* obj, int x, int y);

void ctor_Loc2 (Loc* obj,  Loc* other );

Loc* ctor_leak_Loc(int x, int y);
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Location.h END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\libertyGroup.h START
//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetReplacement.h START
//////////////////////////////////////////////////////////////////////////
#pragma once


typedef struct Piece Piece;
//As its a set, it will never contain duplicates (no matter what is done to it)

//Piece** refers to where it is stored in its current storage location
//Piece* refers to where it is on the board
//Piece refers to what it is on the board

typedef struct FastSetPiece FastSetPiece;

struct FastSetPiece // : public std::set<Piece *>
{
  //Main data storage, never needs to be increased as we cannot have duplicates and
  //the board is only so big!

  Piece* data[TOTAL_SQUARES];  
  int curDataHead;
};

void ctor_FastSetPiece0(FastSetPiece* m);
void ctor_FastSetPiece1(FastSetPiece* m, FastSetPiece* other);

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetPiece* m, Piece* piece);
  
Piece** FastSetPiece_begin(FastSetPiece* m);

Piece** FastSetPiece_end(FastSetPiece* m);
    
//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Piece** operator++ ( Piece**

void FastSetPiece_clear(FastSetPiece* m);

int FastSetPiece_size(FastSetPiece* m);

//Should be faster than erase(Piece* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetPiece* m, int pos);
  
void FastSetPiece_erase(FastSetPiece* m, Piece* piece);

bool FastSetPiece_contains(FastSetPiece* m, Piece* piece);

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
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetReplacement.h END
//////////////////////////////////////////////////////////////////////////

typedef struct Piece Piece;

//These are compared directly, as you should not copy lib groups (they should all
//reference the original square's group, which founded the group)

typedef struct libertyGroup libertyGroup;
//The group contains the pieces connected, liberties is all the empty squares that they
//consider as liberties to them
struct libertyGroup
{
	//ATTENTION! This must be maintained by an outside source as we do not maintain it!
  //By definition a liberty is an empty square, so that is what this contains
	FastSetPiece liberties;
  FastSetPiece group;

  //MUST BE FILLED IN BY BOARD!
  Piece* owner;

};

void ctor_libertyGroup0(libertyGroup* m, libertyGroup* other);
void ctor_libertyGroup1(libertyGroup* m);
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\libertyGroup.h END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\q.h START
//////////////////////////////////////////////////////////////////////////
#pragma once

//Higher levels are slower but check more
//#define DEBUG_LEVEL_1
//#define DEBUG_LEVEL_2
//#define DEBUG_LEVEL_3 //Diagnostic output


#ifdef DEBUG_LEVEL_3 //Diagnostic output
#ifndef DEBUG_LEVEL_2
#define DEBUG_LEVEL_2
#endif
#endif

#ifdef DEBUG_LEVEL_2
#ifndef DEBUG_LEVEL_1
#define DEBUG_LEVEL_1
#endif
#endif


typedef struct Board Board;
typedef struct Piece Piece;




//returns number of squares filled
//outputs should be 4 pieces big
int SurroundingSquares(Piece* outputs[], Loc location, Board* board);
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\q.h END
//////////////////////////////////////////////////////////////////////////



typedef struct Piece Piece;
//You should really not copy these, as they contain all the information on the 
//spot on the board and stuff (SHOULD REALLY BE CALLED SQUARE!)
struct Piece
{
  //MUST BE FILLED IN BY BOARD!
  Loc location;  

  libertyGroup * libGroup;
  //This is NOT our liberties, this instead is the group we own, that way if we need one we
  //can get one right away.
  libertyGroup ownedLibertyGroup;
	
	Pieces pieceTypeCur;

  //Initially false, set when we place and not unset after we remove 
  //(so if hadBlack and pieceTypeCur == Empty, it has a black captured :D )
  bool hadBlack;
  bool hadWhite;
};


void ctor_Piece0(Piece* obj);
void ctor_Piece1(Piece* obj, Pieces initial);

void ctor_PieceCopy(Piece* obj, Piece* other);

bool Piece_Equ ( Piece* p1,  Piece* p2);	
bool Piece_NotEqu ( Piece* p1,  Piece* p2);	
bool Piece_LessThan ( Piece* p1,  Piece* p2);  	
bool Piece_GreaterThan ( Piece* p1,  Piece* p2);

Pieces Piece_Opposite(Piece* m);
bool Piece_IsBlack(Piece* m);
bool Piece_IsWhite(Piece* m);
bool Piece_IsEmpty(Piece* m);

char* Piece_ToString(Piece* m);

char* Piece_Group(Piece* m);

char* Piece_LibsInGroup(Piece* m);
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Piece.h END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Board.h START
//////////////////////////////////////////////////////////////////////////
#pragma once

typedef struct Board Board;



struct Board
{
	int whiteTakenOff;
	int blackTakenOff;

  Pieces curTurn;

	Piece					    board [BOARD_SIZE][BOARD_SIZE];	
};





void ctor_Board0(Board* m);

void ctor_Board1(Board* m,  Board* other );



void				  Board_PlayPiece	(Board* m, Pieces type, Loc location);

//Anyone who gets a piece can also set it... this is because 
//it would be inefficient to have them copy another piece 
//when we have already allocated one.
Piece*			  Board_GetPiece	    (Board* m, Loc* location);

//void				SetPiece	(Loc location, Piece piece);

void        Board_RemoveGroup (Board* m, libertyGroup* group);
void        Board_RemovePiece (Board* m, Loc location);  
	
void        Board_ChangeLiberties  (Board* m, libertyGroup* group, int delta);

libertyGroup*		Board_GetLibertyGroup		  (Board* m, Loc location);
  
char* Board_ToString(Board* m);

char* Board_MetaToString(Board* m);

char* Board_LibsToString(Board* m);
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Board.h END
//////////////////////////////////////////////////////////////////////////



#include <stdlib.h>
#include <string.h>



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


//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Board.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastRandom.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastRandom.h START
//////////////////////////////////////////////////////////////////////////
/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#pragma once


/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* ant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */
/* initializes mt[N] with a seed */
void init_genrand(unsigned long s);

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length);

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void);

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void);

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void);

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void);

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void);

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void);
/* These real versions are due to Isaku Wada, 2002/01/09 added */

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastRandom.h END
//////////////////////////////////////////////////////////////////////////


/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastRandom.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetReplacement.cpp START
//////////////////////////////////////////////////////////////////////////



#include <string.h>

void ctor_FastSetPiece0(FastSetPiece* m)
{
  m->curDataHead = 0;
}

void ctor_FastSetPiece1(FastSetPiece* m, FastSetPiece* other)
{
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

  {
    int x;
    for(x = 0; x < m->curDataHead; x++)    
      if(Loc_Equ(&m->data[x]->location, &piece->location))
        return;
  }


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
#ifndef C
  if(m->curDataHead < 0 || m->curDataHead > TOTAL_SQUARES)
    throw "impossible set size, likely uninitialized";
#endif
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
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetReplacement.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\libertyGroup.cpp START
//////////////////////////////////////////////////////////////////////////

#include <string.h>

void ctor_libertyGroup0(libertyGroup* m, libertyGroup* other)
{
  //The Pieces still need to be remapped
  memcpy(other, m, sizeof(libertyGroup));
}

void ctor_libertyGroup1(libertyGroup* m)
{
  ctor_FastSetPiece0(&m->group);
  ctor_FastSetPiece0(&m->liberties); 
}
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\libertyGroup.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Location.cpp START
//////////////////////////////////////////////////////////////////////////


#include <stdlib.h>

bool	Loc_IsOnBoard	(Loc* piece)
{
	return (piece->x < BOARD_SIZE && piece->y < BOARD_SIZE && piece->x >= 0 && piece->y >= 0);
}

bool Loc_Equ(Loc* p1,  Loc* p2)
{
  return p1->x == p2->x && p1->y == p2->y;
}

bool Loc_NotEqu (Loc* p1,  Loc* p2)
{
  return p1->x != p2->x || p1->y != p2->y;
}

char* Loc_ToString(Loc* m)
{
  char* toReturn = (char*)malloc(10);
  toReturn[0] = (char)(m->x + 48);
  toReturn[1] = ',';
  toReturn[2] = ' ';  
  toReturn[3] = (char)(m->y + 48);
  toReturn[4] = '\0';
  return toReturn;
}

void ctor_Loc0(Loc* obj)
{
  obj->x = -10;
  obj->y = -10;
}

Loc* ctor_Loc1(Loc* obj, int x, int y)
{
  obj->x = x;
  obj->y = y;

  return obj;
}

void ctor_Loc2 (Loc* obj,  Loc* other )
{
  obj->x = other->x;
  obj->y = other->y;
}

Loc* ctor_leak_Loc(int x, int y)
{
  Loc* leaker = (Loc*)malloc(sizeof(Loc));
  ctor_Loc1(leaker, x, y);
  return leaker;
}
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Location.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.h START
//////////////////////////////////////////////////////////////////////////
#pragma once


//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Prune.h START
//////////////////////////////////////////////////////////////////////////
#pragma once



//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board* board, Pieces type, Loc location);

bool IsSuicide(Board* board, Pieces type, Loc location, int friendly, int enemy, int empty, Piece* sqrSur[], int cSqrSur, Piece* pieceCur);


//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Prune.h END
//////////////////////////////////////////////////////////////////////////

typedef struct SimulationResults SimulationResults;

struct SimulationResults
{
  int scoreInFavourOfBlack;  
};

void MonteCarloSimulate(Board* initial, int seed, SimulationResults* simResults);

void ctor_SimulateResults(SimulationResults* m, Board board);

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.h END
//////////////////////////////////////////////////////////////////////////




void ctor_SimulateResults(SimulationResults* m, Board board)
{
  m->scoreInFavourOfBlack = 0;

  m->scoreInFavourOfBlack += board.whiteTakenOff;
  m->scoreInFavourOfBlack -= board.blackTakenOff;

  //We mark the squares, so we can spend time looking for the owner of a group doing work also

  #define MAX_BOARD_LOOPS 200

  {
    int x, y;
    for(x = 0; x < BOARD_SIZE; x++)
    {
      for(y = 0; y < BOARD_SIZE; y++)      
      {
        Loc cur;
        int velX[4] = {1, 0, -1, 0};
        int velY[4] = {0, 1, 0, -1};
        //(1, 0) -> (0, -1) -> (-1, 0) -> (0, 1)
        int curTurn = 0;

        ctor_Loc1(&cur, x, y);
        if(Board_GetPiece(&board, &cur)->pieceTypeCur == Empty)
        {          
          //Sort of hackish... but it fixes some cases (still not great)
          int maxSearches = MAX_BOARD_LOOPS;
          int curSearchs = 0;

          int marked = 1;

          int curTurn = 0;

          Board_GetPiece(&board, &cur)->pieceTypeCur = Marked;

          //Spiral outwards until you hit a fill square, we are assuming that 
          //all enclosed areas are surrounded by one kind... and so that square is the owner          
                                   
          //We turn right
          
          while(++curSearchs < maxSearches)
          {
            ctor_Loc1(&cur, cur.x + velX[curTurn], cur.y + velY[curTurn]);

            if(!Loc_IsOnBoard(&cur)) //Go back, and then turn
            {
              ctor_Loc1(&cur, cur.x - velX[curTurn], cur.y - velY[curTurn]);

              curTurn = (curTurn + 1) % 4;
            }
            else if (Board_GetPiece(&board, &cur)->pieceTypeCur == Black)
            {
              m->scoreInFavourOfBlack += marked;
              break;
            }
            else if (Board_GetPiece(&board, &cur)->pieceTypeCur == White)
            {
              m->scoreInFavourOfBlack -= marked;
              break;
            }
            else //Empty or Marked 
            {              
              if(Board_GetPiece(&board, &cur)->pieceTypeCur == Empty) 
              {
                Board_GetPiece(&board, &cur)->pieceTypeCur = Marked;
                marked++;
              }
              else //Marked
              {
                //(do nothing)
              }
            }

            {
              Loc corner;
              ctor_Loc1(&corner, cur.x + velX[(curTurn + 1) % 4], cur.y + velY[(curTurn + 1) % 4]);
              if(Loc_IsOnBoard(&corner) &&
                (Board_GetPiece(&board, &cur)->pieceTypeCur == Empty))
                //Turn
              {
                curTurn = (curTurn + 1) % 4;
              }
            }
          } //End of finding partial size of container and owner

          if(curSearchs == maxSearches)
          {            
  #ifndef C
            if(m->scoreInFavourOfBlack - marked < 0 && m->scoreInFavourOfBlack + marked >= 0)
            {
              //We cannot call the game as the algorithm to find which pieces are owned by who
              //is not perfect (infact it sucks). It needs to be entirely closed, and so recursion
              //is not acceptable. A large enough manual stack may work, but something that is
              //even better should be created (a zigzag and loop following pattern perhaps).
              throw exception("due to problems with owned recognition, this game cannot be called");
            }
  #endif
            m->scoreInFavourOfBlack += marked;
          }

        } //End of is empty
      } //x loop    
    } //y loop
  }

} //function


void MonteCarloSimulate(Board* boardIn, int seed, SimulationResults* simulationResults)
{
  bool lastPassed = false;
  Board board;

  ctor_Board1(&board, boardIn);

  init_genrand(seed);  

  while(true)
  {
    int xPos = genrand_int32() % 9;
    int yPos = genrand_int32() % 9;
    //Find a position which we can place to, or pass

    int tries = 0; //We cut off after a certain amount of tries...
    //really late game this is inefficient... but for now its fine

    Loc curLoc;    

    while(Board_GetPiece(&board, ctor_Loc1(&curLoc, xPos, yPos))->pieceTypeCur != Empty ||
          !SimplePrune(&board, board.curTurn, *ctor_Loc1(&curLoc, xPos, yPos)))
    {
      xPos = genrand_int32() % 9;
      yPos = genrand_int32() % 9;

      if(++tries > 100 )
        break;
    }

    if(tries > 100)
    {
      if(lastPassed) //Two passes in a row, game over!
      {
        break;
      }
      //else

      lastPassed = true;
      continue;
    }
    else
    {
      lastPassed = false;
    }
    //else

    //We get to play it!
    {
    #ifdef DEBUG_LEVEL_2
        Board boardBefore = board;
    #endif
        Board_PlayPiece(&board, board.curTurn, *ctor_Loc1(&curLoc, xPos, yPos));
    }
    //Things have been changed so this code will not work anyway
#if 0

    #ifdef DEBUG_LEVEL_3
    Board temp2 = board;
    temp2.SetPiece(Loc(xPos, yPos), Piece(Marked));    

    cout << temp2.ToString().c_str();
    #endif

#ifdef DEBUG_LEVEL_1
    Board temp = board;
    for(int x = 0; x < BOARD_SIZE; x++)
    {
      for(int y = 0; y < BOARD_SIZE; y++)
      {
        //might as well check everyone square        
        Piece piece = temp.GetPiece(Loc(x,y));

        if(piece.IsBlack() || piece.IsWhite())
        {
          pieces.clear();
          DestructiveNonThreadSafeMakeLibertyGroup(temp, piece); 
          libertyGroup group = board.GetLibertyGroup(piece.location);
          
#ifdef DEBUG_LEVEL_2
          vector<Piece> emptyPieces;
          for_each(begin(pieces), end(pieces), [&emptyPieces, &board] (Piece piece){
            vector<Piece> surrounding = SurroundingSquares<Piece>(&Board_GetPieceVoid, 
              piece.location, &board);
            for_each(begin(surrounding), end(surrounding), [&emptyPieces](Piece piece) {
              if(piece.pieceTypeCur == Empty)
              {
                bool alreadyFound = false;
                for_each(begin(emptyPieces), end(emptyPieces), [&piece, &alreadyFound](Piece prevPiece){
                  if(prevPiece.location == piece.location)
                    alreadyFound = true;
                });
                if(!alreadyFound)
                  emptyPieces.push_back(piece);
              }
            });
          });
#endif

          if(pieces.size() != group.size())             
          {
            boardBefore.PlayPiece(pieceTypeCur, Loc(xPos, yPos));

            pieces.clear();
            DestructiveNonThreadSafeMakeLibertyGroup(boardBefore, piece); 

#ifndef C
            throw exception("invalid group");
#endif
          }
        }
      }
    }
#endif
#endif
    
    //Double check all liberties are correct and that groups were properly made!
  }

  ctor_SimulateResults(simulationResults, board);
}

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Piece.cpp START
//////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h>

void ctor_Piece0(Piece* obj)
{
  obj->pieceTypeCur = Empty;
  obj->hadBlack = false;
  obj->hadWhite = false;

  ctor_libertyGroup1(&obj->ownedLibertyGroup);
  obj->libGroup = &obj->ownedLibertyGroup;
}
void ctor_Piece1(Piece* obj, Pieces initial)
{
  ctor_Piece0(obj);
  obj->pieceTypeCur = initial;  
}

void ctor_PieceCopy(Piece* obj, Piece* other)
{
  //Copy the memory, this should work?
  memcpy(obj, other, sizeof(Piece));

    //Hmm are you sure you want to do this? Plus I am not sure if it works
    //(usually you should not move stuff around, just use the references
    //in the original class and use delta theory to prevent coping)  
}

bool	Piece_IsBlack(Piece* m)
{
	return m->pieceTypeCur == Black;
}
bool	Piece_IsWhite(Piece* m)
{
	return m->pieceTypeCur == White;
}
bool	Piece_IsEmpty(Piece* m)
{
	return m->pieceTypeCur == Empty;
}

Pieces	Piece_Opposite(Piece* m)
{
	if(m->pieceTypeCur == Black)
		return White;
	else if(m->pieceTypeCur == White) 
		return Black;
	else 
  {
#ifndef C
		throw std::exception("Called opposite of an empty square");
#endif
    return Empty;
  }
}



bool Piece_Equ(Piece* p1,  Piece* p2)  { return Loc_Equ(&p1->location, &p2->location); }
bool Piece_NotEqu(Piece* p1,  Piece* p2)  { return Loc_NotEqu(&p1->location, &p2->location); }
bool Piece_LessThan(Piece* p1,  Piece* p2)	  { return (p1->location.y < p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x < p2->location.x); }
bool Piece_GreaterThan(Piece* p1,  Piece* p2)	  { return (p1->location.y > p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x > p2->location.x); }

char* Piece_ToString(Piece* m) 
{
  if(m->pieceTypeCur == Black)
    return "b";
  else if(m->pieceTypeCur == White)
    return "w";
  else if(m->pieceTypeCur == Marked)
    return "M";
  else
    return " ";
}

char* Piece_Group(Piece* m) 
{  
  char* curText = (char*)malloc(1000);  
  int curPos = 0;

  
  Piece** pieceInGroup;
  for(pieceInGroup = FastSetPiece_begin(&m->libGroup->group);
    pieceInGroup != FastSetPiece_end(&m->libGroup->group); pieceInGroup++)
  {    
    char * strLocTemp = Loc_ToString(&(*pieceInGroup)->location);

    curText[curPos++] = '(';

    memcpy(curText + curPos, strLocTemp, strlen(strLocTemp));
    curPos += strlen(strLocTemp);
    //delete locTemp;
    curText[curPos++] = ')';
    curText[curPos++] = ' ';
  }
  

  curText[curPos] = '\0';
  return curText;
}

char* Piece_LibsInGroup(Piece* m) 
{
  char* curText = (char*)malloc(1000);
  int curPos = 0;

  Piece** libInGroup;
  for(libInGroup = FastSetPiece_begin(&m->libGroup->liberties);
    libInGroup != FastSetPiece_end(&m->libGroup->liberties); libInGroup++)
  {
    char * locTemp = Loc_ToString(&(*libInGroup)->location);

    curText[curPos++] = '(';

    memcpy(curText + curPos, locTemp, strlen(locTemp));
    curPos += strlen(locTemp);

    //delete locTemp;
    curText[curPos++] = ')';
    curText[curPos++] = ' ';    
  }

  curText[curPos] = '\0';
  return curText;
}

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Piece.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Prune.cpp START
//////////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Prune.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\q.cpp START
//////////////////////////////////////////////////////////////////////////






int SurroundingSquares(Piece* outputs[], Loc location, Board* board)
{     
#ifdef DEBUG_LEVEL_2
#ifndef C
	if(!Loc_IsOnBoard(&location))
		throw exception("calling surronding squares with a location off the board!");
#endif
#endif
   
  int numOutputs = 0;
  Loc tempLoc;

  //Make sure this is not coping anything (except location)
  if(location.x + 1 < BOARD_SIZE)	
    outputs[numOutputs++] = Board_GetPiece(board, 
    ctor_Loc1(&tempLoc, location.x + 1, location.y));		

	if(location.x - 1 >= 0)	
    outputs[numOutputs++] = Board_GetPiece(board, 
    ctor_Loc1(&tempLoc, location.x - 1, location.y));

	if(location.y + 1 < BOARD_SIZE)	
    outputs[numOutputs++] = Board_GetPiece(board, 
    ctor_Loc1(&tempLoc, location.x, location.y + 1));

	if(location.y - 1 >= 0)	
    outputs[numOutputs++] = Board_GetPiece(board, 
    ctor_Loc1(&tempLoc, location.x, location.y - 1));

  return numOutputs;
}

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\q.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\EntryPoint.cpp START
//////////////////////////////////////////////////////////////////////////



#include <time.h>







#include <stdio.h>



//setup a build console to use new C++ 11 stuff (not sure if it will already)
//and full optimization

//Avg Sim time (milli)    1st Result    2nd   3rd     Avg Result    Avg Black Win
//(per montecarlo     
//simulation)

//0.3
//0.109                   -18           11    24      9.469         0.715


int main ()
{  
#if 0
  Board board;

  bool black = true;
  int x = 0, y = 0;
  cin >> x;
  cin >> y;
  while(x >= 0 && y >= 0)
  {
#ifndef C
    try
    {
#endif
      Piece piece;
      piece.pieceTypeCur = black ? Black : White;    
      black = !black;

      board.PlayPiece(piece, Loc(x, y));
      cout << board.ToString().c_str() << "\n";
      cout << board.MetaToString().c_str() << "\n";
      cout << board.LibsToString().c_str() << "\n";
      cout << "\n";
      cout << "\n";

     cin >> x;
      cin >> y;
#ifndef C
    }
    catch (exception e)
    {      
      cout << "not possible as: " << e.what() << "\n";

      cin >> x;
      cin >> y;
    }
#endif
  }
#endif
  
#ifndef C
  try
  {
#endif
  Board board;
  Loc hardCodedLocation;
  ctor_Board0(&board);

  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 4, 4));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 4, 5));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 4, 6));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 5, 4));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 5, 6));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 6, 4));
  Board_PlayPiece(&board, White, *ctor_Loc1(&hardCodedLocation, 6, 5));
  Board_PlayPiece(&board, White, *ctor_Loc1(&hardCodedLocation, 6, 6));

  //board.PlayPiece(Black, Loc(7, 4));
  //board.PlayPiece(Black, Loc(7, 6));
  //board.PlayPiece(Black, Loc(8, 5));
  //board.PlayPiece(Black, Loc(8, 6));

#ifdef C
  printf(Board_ToString(&board)); printf("\n");
  printf(Board_MetaToString(&board)); printf("\n");
  printf(Board_LibsToString(&board)); printf("\n");  
#else
  cout << Board_ToString(&board).c_str() << "\n";
  cout << Board_MetaToString(&board).c_str() << "\n";
  cout << Board_LibsToString(&board).c_str() << "\n";  
#endif  
            
  {
    long t1 = clock(); long totalTime; double time;
    SimulationResults results;
    MonteCarloSimulate(&board, 20452, &results);
    printf( "%d black score\n", results.scoreInFavourOfBlack );  

    totalTime = (clock() - t1);
	  time = totalTime;    
    printf( "%.2f time for first\n", time );
  
    MonteCarloSimulate(&board, 3452, &results);
    printf( "%d black score\n", results.scoreInFavourOfBlack );  
  
    MonteCarloSimulate(&board, 3422, &results);
    printf( "%d black score\n", results.scoreInFavourOfBlack );  
  }
  
  {
    SimulationResults results;
    long t1 = clock(); long totalTime; double time;
    double averageResult = 0.0;

    int cSimulations = 10000;
    double blackWinPercentage = 0;
    int x;

    #ifdef DEBUG_LEVEL_1
      cSimulations -= 5000;
    #endif
    #ifdef DEBUG_LEVEL_2
      cSimulations -= 3000;
    #endif
    #ifdef DEBUG_LEVEL_3 //Diagnostic output
      cSimulations -= 1000;
    #endif

    for(x = 0; x < cSimulations; x++)
    {
      MonteCarloSimulate(&board, genrand_int32(), &results);
      averageResult += results.scoreInFavourOfBlack;

      if(results.scoreInFavourOfBlack > 0)
        blackWinPercentage++;
    }
  
    averageResult /= cSimulations;
    blackWinPercentage /= cSimulations;

    totalTime = (clock() - t1);
	  time = (double)totalTime / (double)cSimulations;  

    printf( "%.4f average time (%.2f per second) and %g average result with %g average black win percentage\n", time, (1000 / time), averageResult, blackWinPercentage); 
  }
  return 0;

#ifndef C
  } catch(exception e)
  {
    cout << e.what();
  }
#endif
  
  
  return 1;
}

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\EntryPoint.cpp END
//////////////////////////////////////////////////////////////////////////
