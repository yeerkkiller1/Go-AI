//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\q.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START q.h START
//////////////////////////////////////////////////////////////////////////
//#pragma once

//Higher levels are slower but check more
//#define DEBUG_LEVEL_1
//#define DEBUG_LEVEL_2
//#define DEBUG_LEVEL_3 //Diagnostic output
//////////////////////////////////////////////////////////////////////////
//START Constants.h START
//////////////////////////////////////////////////////////////////////////
#pragma once

#define BOARD_SIZE 9
#define TOTAL_SQUARES 81

#define ERROR_CHECKING
#define ERR

#ifndef C
  #define C
#endif

#define bool int

#ifndef false
  #define false 0
#endif

#ifndef true
  #define true  1
#endif

#ifndef OPEN_CL
#define __global
#endif

#ifndef OPEN_CL
#define __constant
#endif

#ifndef NULL
#define NULL 0
#endif

//#define byte unsigned char

#ifdef OPEN_CL
  #ifdef PROFILE
    #error "OPEN_CL and PROFILE are not compatible, do not compile both at the same time"
  #endif
#endif

//#ifdef OPEN_CL
  //#define COMPRESS_MEMORY
//#endif

#ifdef OPEN_CL
  #define LOCAL_MEM_FENCE barrier(CLK_LOCAL_MEM_FENCE)
#else
  #define LOCAL_MEM_FENCE 
#endif


  //We need enough for 81*5 squares as that is the max sum of liberties and
  //pieces played (it is actually a bit less, but this is fine)
//#define cSquareIntPositions 102
#ifdef COMPRESS_MEMORY
  #define cSquareIntPositions 1
#else
  #define cSquareIntPositions 21
#endif

#define cSquarePositions 408
//////////////////////////////////////////////////////////////////////////
//END Constants.h END
//////////////////////////////////////////////////////////////////////////

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
typedef struct Square Square;
typedef struct Loc Loc;
typedef struct FastSetPiece FastSetPiece;
typedef struct libertyGroup libertyGroup;

#ifdef DEBUG  
#define DASSERT(boolArgument) \
if(!(boolArgument))             \
{                             \
  __debugbreak();             \
}
#else
#define DASSERT
#endif

#ifdef DEBUG
#define BOARD(board, x,y) Board_SafeAccess(board, x, y)
#else
#define BOARD(boardPointer, x,y) (&( boardPointer )->board[(x) + (y)*BOARD_SIZE])
#endif
//////////////////////////////////////////////////////////////////////////
//END q.h END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\q.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastRandom.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START FastRandom.h START
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

//These cannot be global so they must be declared somewhere else
//static unsigned long mt[N]; /* the array for the state vector  */
//static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s, unsigned long* mt, int* pmti);

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(unsigned long* mt, int* pmti);
//////////////////////////////////////////////////////////////////////////
//END FastRandom.h END
//////////////////////////////////////////////////////////////////////////

// initializes mt[N] with a seed 
void init_genrand(unsigned long s, unsigned long* mt, int* pmti)
{
    mt[0]= s & 0xffffffffUL;
    for ((*pmti)=1; *pmti<N; (*pmti)++) {
        mt[*pmti] = 
	    (1812433253UL * (mt[*pmti-1] ^ (mt[*pmti-1] >> 30)) + *pmti); 
        // See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. 
        // In the previous versions, MSBs of the seed affect   
        // only MSBs of the array mt[].                        
        // 2002/01/09 modified by Makoto Matsumoto             
        mt[*pmti] &= 0xffffffffUL;
        // for >32 bit machines 
    }
}



// initialize by an array with array-length 
// init_key is the array for initializing keys 
// key_length is its length 
// slight change for C++, 2004/2/26 
void init_by_array(unsigned long init_key[], int key_length, unsigned long* mt, int* pmti)
{
    int i, j, k;
    init_genrand(19650218UL, mt, pmti);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; // non linear 
        mt[i] &= 0xffffffffUL; // for WORDSIZE > 32 machines 
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; // non linear 
        mt[i] &= 0xffffffffUL; // for WORDSIZE > 32 machines 
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; // MSB is 1; assuring non-zero initial array  
}


// generates a random number on [0,0xffffffff]-interval 
//mt must be of size N and mti should start off at N + 1
unsigned long genrand_int32(unsigned long* mt, int* pmti)
{
    unsigned long y;
    unsigned long mag01[2]={0x0UL, MATRIX_A};
    // mag01[x] = x * MATRIX_A  for x=0,1

    if (*pmti >= N) { // generate N words at one time 
        int kk;

        if (*pmti == N+1)   // if init_genrand() has not been called,
            init_genrand(5489UL, mt, pmti); // a default initial seed is used

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

        *pmti = 0;
    }
  
    y = mt[(*pmti)++];

    // Tempering
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}


// generates a random number on [0,0x7fffffff]-interval 
long genrand_int31(unsigned long* mt, int* pmti)
{
    return (long)(genrand_int32(mt, pmti)>>1);
}

// generates a random number on [0,1]-real-interval 
double genrand_real1(unsigned long* mt, int* pmti)
{
    return genrand_int32(mt, pmti)*(1.0/4294967295.0); 
    // divided by 2^32-1  
}

// generates a random number on [0,1)-real-interval 
double genrand_real2(unsigned long* mt, int* pmti)
{
    return genrand_int32(mt, pmti)*(1.0/4294967296.0); 
    // divided by 2^32 
}

// generates a random number on (0,1)-real-interval 
double genrand_real3(unsigned long* mt, int* pmti)
{
    return (((double)genrand_int32(mt, pmti)) + 0.5)*(1.0/4294967296.0); 
    // divided by 2^32 
}

// generates a random number on [0,1) with 53-bit resolution
double genrand_res53(unsigned long* mt, int* pmti) 
{ 
    unsigned long a=genrand_int32(mt, pmti)>>5, b=genrand_int32(mt, pmti)>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
// These real versions are due to Isaku Wada, 2002/01/09 added 
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastRandom.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Location.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START Location.h START
//////////////////////////////////////////////////////////////////////////
#pragma once



//This is abstracted even though for now it will only be two ints
//in the future it will likely have (static?) helper functions for simple things
//that will be extremely optimized.
struct Loc
{
	int x, y;
};

bool Loc_IsOnBoard(Loc* m);

void Loc_ToString(Loc* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                  int* pCharPos);

bool Loc_Equ(Loc* p1, Loc* p2);
bool Loc_NotEqu (Loc* p1, Loc* p2);   

void ctor_Loc0(Loc* obj);

Loc* ctor_Loc1(Loc* obj, int x, int y);

void ctor_Loc2 (Loc* obj,  Loc* other );

unsigned char Loc_CompressPosition(int x, int y);
//////////////////////////////////////////////////////////////////////////
//END Location.h END
//////////////////////////////////////////////////////////////////////////


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

//////////////////////////////////////////////////////////////////////////
//START IntStream.h START
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef OPEN_CL
  #include <stdlib.h>
#endif


void WriteCharToIntStream
(
__global int* intStream,
  int* pCharPos,
  unsigned char charValue
);

char GetCharFromIntStream
(
__global int* intStream,
  int charPos
);
void WriteStringToIntStream
(
__global int* intStream,
  int* pCharPos,
#ifdef OPEN_CL
  const __constant char* stringToWrite
#else
  const char* stringToWrite
#endif
);

//Does not round, simply trims the float
void WriteFloatToIntStream
(
__global int* intStream,
  int* pCharPos,
  float number,
  int decimalPlaces
);
//////////////////////////////////////////////////////////////////////////
//END IntStream.h END
//////////////////////////////////////////////////////////////////////////
void Loc_ToString(Loc* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                  int* pCharPos)
{
  WriteCharToIntStream(intStream, pCharPos, (char)(m->x + 48));
  WriteCharToIntStream(intStream, pCharPos, ',');
  WriteCharToIntStream(intStream, pCharPos, ' ');
  WriteCharToIntStream(intStream, pCharPos, (char)(m->y + 48));
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

unsigned char Loc_CompressPosition(int x, int y)
{
  return (x << 4) + y;
}
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Location.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetReplacement.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START FastSetReplacement.h START
//////////////////////////////////////////////////////////////////////////
#pragma once


//#include "FastSetLinked.h"

//As its a set, it will never contain duplicates (no matter what is done to it)

//Square** refers to where it is stored in its current storage location
//Square* refers to where it is on the board
//Square refers to what it is on the board

/*
struct FastSetPiece// : public std::set<Square *>
{
  Board* board;
  
  int squarePositions[cSquareIntPositions + 1];
  //int curDataHead;
};
*/

void ctor_FastSetPiece0(int* m, Board* containingBoard);
void ctor_FastSetPiece1(int* m, int* other);

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(int* m, Square* square);
  
int FastSetPiece_length(int* m);

Square* FastSetPiece_getAt(Board* board, int* m, int pos);

//Set is not needed by anyone outside... I think?
void FastSetPiece_setAt(int* m, int pos, Square* square);

Square* FastSetPiece_getAtOtherBoard(int* m, int pos, Board* board);


void FastSetPiece_clear(int* m);

int FastSetPiece_size(int* m);

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(Board* board, int* m, int pos);

void FastSetPiece_erase(int* m, Square* piece);

bool FastSetPiece_contains(int* m, Square* piece);
//////////////////////////////////////////////////////////////////////////
//END FastSetReplacement.h END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//START Square.h START
//////////////////////////////////////////////////////////////////////////
#pragma once




#define Pieces  int
#define Black   1 << 5
#define White   1 << 7
#define Empty   1 << 9
#define Marked  1 << 11

//////////////////////////////////////////////////////////////////////////
//START libertyGroup.h START
//////////////////////////////////////////////////////////////////////////
#pragma once

//These are compared directly, as you should not copy lib groups (they should all
//reference the original square's group, which founded the group)

//The group contains the pieces connected, liberties is all the empty squares that they
//consider as liberties to them
/*
struct libertyGroup
{
	//ATTENTION! This must be maintained by an outside source as we do not maintain it!
  
  //Contains a list of empty squares this group considers liberties
	FastSetPiece liberties;

  //Contains all the squares in this group
  FastSetPiece group;

  //MUST BE FILLED IN BY BOARD!
  //Square* owner;
};
*/

/*
//void ctor_libertyGroup0(libertyGroup* m, libertyGroup* other);
void ctor_libertyGroup1(libertyGroup* m, Board* containingBoard);
*/
//////////////////////////////////////////////////////////////////////////
//END libertyGroup.h END
//////////////////////////////////////////////////////////////////////////


//You should really not copy these, as they contain all the information on the 
//spot on the board and stuff (SHOULD REALLY BE CALLED SQUARE!)

struct Square
{
  //MUST BE FILLED IN BY BOARD (well ctor_Square0 called in ctor_Board)
  Loc location;
  int compressedSetLocation; //Should really be in Loc... but... that would be slower

  Pieces pieceTypeCur;

  //Initially false, set when we place and not unset after we remove 
  //(so if hadBlack and pieceTypeCur == Empty, it has a black captured :D )
  bool hadBlack;
  bool hadWhite;

  Board* board;

  Square* libGrp;
  //This is NOT our liberties, this instead is the group we own, that way if we need one we
  //can get one right away.
  //libertyGroup ownedGroup;

  //We inline libertyGroup now... to make OpenCL compile
  //Contains a list of empty squares this group considers liberties
	int liberties[cSquareIntPositions + 1];

  //Contains all the squares in this group
  int group[cSquareIntPositions + 1];	
};


void ctor_Square0(Square* obj, Board* containingBoard, int x, int y);
void ctor_Square1(Square* obj, Board* containingBoard, Pieces initial, int x, int y);

void ctor_SquareCopy(Square* obj, Square* other);

bool Square_Equ ( Square* p1,  Square* p2);	
bool Square_NotEqu ( Square* p1,  Square* p2);	
bool Square_LessThan ( Square* p1,  Square* p2);  	
bool Square_GreaterThan ( Square* p1,  Square* p2);

Pieces Square_Opposite(Square* m);
bool Square_IsBlack(Square* m);
bool Square_IsWhite(Square* m);
bool Square_IsEmpty(Square* m);

void Square_ToString(Square* m, __global int* intStream, int* pCharPos);
void Square_Group(Square* m, __global int* intStream, int* pCharPos);
void Square_LibsInGroup(Square* m, __global int* intStream, int* pCharPos);

//////////////////////////////////////////////////////////////////////////
//END Square.h END
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//START Board.h START
//////////////////////////////////////////////////////////////////////////
#pragma once



struct Board
{
	int whiteTakenOff;
	int blackTakenOff;

  Pieces curTurn;

#ifdef COMPRESS_MEMORY
  Square					    board [1];  
#else
	Square					    board [BOARD_SIZE*BOARD_SIZE];  
#endif
};





void ctor_Board0(Board* m);

void ctor_Board1(Board* m,  Board* other );



void	Board_PlayPiece	(Board* m, Pieces type, Loc location);

//Anyone who gets a piece can also set it... this is because 
//it would be inefficient to have them copy another piece 
//when we have already allocated one.
Square*			  Board_GetPiece	    (Board* m, Loc* location);

//Always checks all bounds, and return NULL if it is not valid
Square*       Board_SafeAccess    (Board* m, int x, int y);

//void				SetPiece	(Loc location, Square piece);

void        Board_RemoveGroup (Board* m, Square* libGrpToRemove);
void        Board_RemovePiece (Board* m, Loc location);  
	
void Board_ToString (Board* m, __global int* intStream, int* pCharPos);

void Board_MetaToString(Board* m, __global int* intStream, int* pCharPos);
void Board_LibsToString(Board* m, __global int* intStream, int* pCharPos);

//Allows you to store a Square position in an int array (and allows us to
//decide how to compress it within the int* position).
Square* Board_GetSqr(Board* m, int* positions, int squarePos);

//The counterpart to Board_GetSqr
void Board_SetSqr(Board* m, int* positions, int squarePos, Square* square);

//returns number of squares filled
//outputs should be 4 pieces big
int SurroundingSquares(Square** outputs, Loc location, Board* board);
//////////////////////////////////////////////////////////////////////////
//END Board.h END
//////////////////////////////////////////////////////////////////////////

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
#ifndef OPEN_CL
    FastSetPiece_eraseAt(piece->board, m, pos);
#endif
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

#ifndef OPEN_CL
  FastSetPiece_setAt(m, m->squarePositions[cSquareIntPositions], square);
  m->squarePositions[cSquareIntPositions]++;
#endif
  
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
  #ifndef OPEN_CL
    FastSetPiece_setAt(m, pos, Board_GetSqr(m->board, m->squarePositions, pos + 1));
    pos++;
#endif
  }  
  
  #ifndef OPEN_CL
  m->squarePositions[cSquareIntPositions]--;
#endif
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
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetReplacement.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\libertyGroup.cpp START
//////////////////////////////////////////////////////////////////////////

/*
void ctor_libertyGroup0(libertyGroup* m, libertyGroup* other)
{
  //The Pieces still need to be remapped
  *other = *m;
}
*/

/*
void ctor_libertyGroup1(libertyGroup* m, Board* containingBoard)
{
  ctor_FastSetPiece0(&m->group, containingBoard);
  ctor_FastSetPiece0(&m->liberties, containingBoard);
}
*/
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\libertyGroup.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Square.cpp START
//////////////////////////////////////////////////////////////////////////

void ctor_Square0(Square* m, Board* containingBoard, int x, int y)
{
  m->pieceTypeCur = Empty;
  m->hadBlack = false;
  m->hadWhite = false;

  m->board = containingBoard;

  ctor_FastSetPiece0(m->group, containingBoard);
  ctor_FastSetPiece0(m->liberties, containingBoard);    

  ctor_Loc1(&m->location, x, y);

  m->compressedSetLocation = Loc_CompressPosition(x, y);
}

void ctor_Square1(Square* obj, Board* containingBoard, Pieces initial, int x, int y)
{
  ctor_Square0(obj, containingBoard, x, y);
  obj->pieceTypeCur = initial;    
}

void ctor_SquareCopy(Square* obj, Square* other)
{
  //Copy the memory, this should work?
  *obj = *other;

    //Hmm are you sure you want to do this? Plus I am not sure if it works
    //(usually you should not move stuff around, just use the references
    //in the original class and use delta theory to prevent coping)  
}

bool	Square_IsBlack(Square* m)
{
	return m->pieceTypeCur == Black;
}
bool	Square_IsWhite(Square* m)
{
	return m->pieceTypeCur == White;
}
bool	Square_IsEmpty(Square* m)
{
	return m->pieceTypeCur == Empty;
}

Pieces	Square_Opposite(Square* m)
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

bool Square_Equ(Square* p1,  Square* p2)  { return Loc_Equ(&p1->location, &p2->location); }
bool Square_NotEqu(Square* p1,  Square* p2)  { return Loc_NotEqu(&p1->location, &p2->location); }
bool Square_LessThan(Square* p1,  Square* p2)	  { return (p1->location.y < p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x < p2->location.x); }
bool Square_GreaterThan(Square* p1,  Square* p2)	  { return (p1->location.y > p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x > p2->location.x); }

void Square_ToString(Square* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                    int* pCharPos) 
{
  if(m->pieceTypeCur == Black)
    WriteCharToIntStream(intStream, pCharPos, 'B');
  else if(m->pieceTypeCur == White)
    WriteCharToIntStream(intStream, pCharPos, 'W');
  else if(m->pieceTypeCur == Marked)
    WriteCharToIntStream(intStream, pCharPos, 'M');
  else if(m->hadBlack)
    WriteCharToIntStream(intStream, pCharPos, 'b');
  else if(m->hadWhite)
    WriteCharToIntStream(intStream, pCharPos, 'w');
  else
    WriteCharToIntStream(intStream, pCharPos, ' ');
}

void Square_Group(Square* m, __global int* intStream, int* pCharPos) 
{    
  int* libGrp = m->libGrp->group;
  int z;
  for(z = 0; z < FastSetPiece_length(libGrp); z++)
  { 
    WriteCharToIntStream(intStream, pCharPos, '(');
    Loc_ToString(&FastSetPiece_getAt(m->board, libGrp, z)->location, intStream, pCharPos);
    WriteCharToIntStream(intStream, pCharPos, ')');
    WriteCharToIntStream(intStream, pCharPos, ' ');
  }
}

void Square_LibsInGroup(Square* m, __global int* intStream, int* pCharPos) 
{
  int* libs = m->libGrp->liberties;
  int z;
  for(z = 0; z < FastSetPiece_length(libs); z++)
  {
    WriteCharToIntStream(intStream, pCharPos, '(');
    Loc_ToString(&FastSetPiece_getAt(m->board, libs, z)->location, intStream, pCharPos);
    WriteCharToIntStream(intStream, pCharPos, ')');
    WriteCharToIntStream(intStream, pCharPos, ' ');
  }
}

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Square.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Board.cpp START
//////////////////////////////////////////////////////////////////////////


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
          int* otherGroup = otherPiece->group;
          int* otherLibs = otherPiece->group;
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
  int* grpToRemove = libGrpToRemove->group;
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
    int* srcGrp = src->group;
    int z;
    for(z = 0; z < FastSetPiece_length(srcGrp); z++)
    {
      FastSetPiece_insert(dst->group, FastSetPiece_getAt(src->board, srcGrp, z));
      FastSetPiece_getAt(src->board, srcGrp, z)->libGrp = dst;
    }
  }

  //Add all of their liberties to dst also
  {
    int* srcLib = src->liberties;
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
        //#ifndef OPEN_CL
        FastSetPiece_erase(sqrSur[x]->libGrp->liberties, curSquare);
        //#endif
      }
    }
  } //End make cur liberty group
  //If we make the group dead, we handle it later (unless it is friendly, then prune failed and we broke the rules)
   
  #ifndef OPEN_CL
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
#endif
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

Square* Board_GetSqr(Board* m, int* positions, int squarePos)
{  
  unsigned char squarePosition = (unsigned char)(positions[squarePos/4] >> (squarePos % 4 * 8));

  int x = squarePosition >> 4;
  //Really unoptimimum
  int y = squarePosition - (x * (1 << 4));
  
  DASSERT(BOARD(m, x, y));
  
  return BOARD(m, x, y);
}

void Board_SetSqr(Board* m, int* positions, int squarePos, Square* square)
{
  unsigned char charValue = Loc_CompressPosition(square->location.x, square->location.y);

  int intOffset = squarePos % 4;

  int charAsInt = charValue << ((intOffset) * 8);
  int charMaskInverted = ((1 << 8) - 1) << ((intOffset) * 8);
  int charMask = (-1) ^ charMaskInverted;
  int intPos = squarePos / 4;


  positions[intPos] &= charMask;
  positions[intPos] |= charAsInt;  
}
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Board.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Prune.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START Prune.h START
//////////////////////////////////////////////////////////////////////////
#pragma once



//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board* board, Pieces type, Loc location);

bool IsSuicide(Board* board, Pieces type, Loc location, int friendly, int enemy, int empty, Square** sqrSur, int cSqrSur, Square* pieceCur);
//////////////////////////////////////////////////////////////////////////
//END Prune.h END
//////////////////////////////////////////////////////////////////////////

//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board* board, Pieces type, Loc location)
{
  Square* pieceCur = Board_GetPiece(board, &location);
  
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

    Square* sqrSur[4];
    int cSqrSur;

    int x;

    cSqrSur = SurroundingSquares(sqrSur, location, board);
    for(x = 0; x < cSqrSur; x++)
    {
      if(sqrSur[x] != NULL)
      {
        if(sqrSur[x]->pieceTypeCur == type)    
          friendly++;
        else if(sqrSur[x]->pieceTypeCur != Empty && Square_Opposite(sqrSur[x]) == type)
          enemy++;
        else
          empty++;
      }
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
  
    {
      int realCSqrSur = 0;

      if(sqrSur[0] != NULL)
        realCSqrSur++;
      if(sqrSur[1] != NULL)
        realCSqrSur++;
      if(sqrSur[2] != NULL)
        realCSqrSur++;
      if(sqrSur[3] != NULL)
        realCSqrSur++;

      //If friendly is all but 1
      if(enemy == 0 && friendly >= (realCSqrSur - 1))
        return false;
    }

    return true;
  }
}

//Returns true if it is suicide (so completely not allowed)
bool IsSuicide(Board* board, Pieces type, Loc location, int friendly, int enemy, int empty, Square** sqrSur, int cSqrSur, Square* pieceCur)
{  
  int x;
  //It cannot be suicide if we have empty squares
  if(empty != 0)
    return false;
      
  {    
    //It cannot be suicide if any of our friendly neighbours have more than 2 liberties      
    if(friendly >= 0)
      for(x = 0; x < cSqrSur; x++)
        if(sqrSur[x] != NULL)
          if(sqrSur[x]->pieceTypeCur == type)
            if(FastSetPiece_size(sqrSur[x]->libGrp->liberties) >= 2)
              return false;

    //It cannot be suicide if we kill any enemy groups
    if(enemy >= 0)
      for(x = 0; x < cSqrSur; x++)
        if(sqrSur[x] != NULL)
          if(sqrSur[x]->pieceTypeCur != Empty && Square_Opposite(sqrSur[x]) == type)
            if(FastSetPiece_size(sqrSur[x]->libGrp->liberties) == 1) //Then we will kill them
              return false;
  }

  //We have not filtered out, so we just need to start creating the liberty group we would
  //have if we placed, and then see if it is big enough (we stop early, eventually we might
  //be smarter and analysis if the liberty group is good or optimize it down to simply keeping
  //track of one liberty)
  {
    int libertiesFound[cSquareIntPositions + 1];
    ctor_FastSetPiece0(libertiesFound, board);

    for(x = 0; x < cSqrSur; x++)
    {
      if(sqrSur[x] != NULL)
      {
        if(sqrSur[x]->pieceTypeCur == type)
        {
          int* libs = sqrSur[x]->libGrp->liberties;
          int z;
          for(z = 0; z < FastSetPiece_length(libs); z++)
          {
            if(FastSetPiece_getAt(board, libs, z) != pieceCur)
            {
              FastSetPiece_insert(libertiesFound, FastSetPiece_getAt(board, libs, z));
              if(FastSetPiece_size(libertiesFound) >= 2) //Whenever our group gets too big, it cannot be suicide
                return false;          
            }
          }
        }
        else if(sqrSur[x]->pieceTypeCur == Empty)
        {
          //We already found empty, this is sort of overkill
          FastSetPiece_insert(libertiesFound, sqrSur[x]);
          if(FastSetPiece_size(libertiesFound) >= 2) //Whenever our group gets too big, it cannot be suicide
            return false;  
        }
      }
    }

    return FastSetPiece_size(libertiesFound) == 0;    
  }
}


//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Prune.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START MontecarloSimulation.h START
//////////////////////////////////////////////////////////////////////////
#pragma once



typedef struct SimulationResults SimulationResults;

struct SimulationResults
{
  int scoreInFavourOfBlack;  
  int blackPlays;
  int whitePlays;
};

void MonteCarloSimulate
(
  Board* boardIn,
  int seed,
  SimulationResults* simulationResults,
  unsigned long* mt,
  int* pmti
);

void ctor_SimulateResults(SimulationResults* m, Board board);

#ifdef PROFILE
  int* GetProfileStream();
  int* GetProfileStreamPos();
#endif
//////////////////////////////////////////////////////////////////////////
//END MontecarloSimulation.h END
//////////////////////////////////////////////////////////////////////////




#ifdef PROFILE
int profileStream[100000];
int profilePos = 0;

int* GetProfileStream()
{
  return profileStream;
}

int* GetProfileStreamPos()
{
  return &profilePos;
}
#endif

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

          //int curTurn = 0;

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

#ifdef PROFILE
  profileStream[profilePos++] = m->scoreInFavourOfBlack;
#endif

} //function

void MonteCarloSimulate
(
  Board* boardIn,
  int seed,
  SimulationResults* simulationResults,  
  unsigned long* mt,
  int* pmti
)
{
  bool lastPassed = false;
  Board board;
  
  ctor_Board1(&board, boardIn);
  
  init_genrand(seed, mt, pmti);  

  while(true)
  {
    int xPos = genrand_int32(mt, pmti) % 9;
    int yPos = genrand_int32(mt, pmti) % 9;
    //Find a position which we can place to, or pass

    int tries = 0; //We cut off after a certain amount of tries...
    //really late game this is inefficient... but for now its fine

    Loc curLoc;    

    while(Board_GetPiece(&board, ctor_Loc1(&curLoc, xPos, yPos))->pieceTypeCur != Empty ||
          !SimplePrune(&board, board.curTurn, *ctor_Loc1(&curLoc, xPos, yPos)))
    {
      xPos = genrand_int32(mt, pmti) % 9;
      yPos = genrand_int32(mt, pmti) % 9;

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
        Board_PlayPiece(&board, board.curTurn, *ctor_Loc1(&curLoc, xPos, yPos));

        if(board.curTurn == Black)
        {
          simulationResults->blackPlays++;
        }
        else if(board.curTurn == White)
        {
          simulationResults->whitePlays++;
        }
    }
    
    //Double check all liberties are correct and that groups were properly made!
  }

  ctor_SimulateResults(simulationResults, board);
}

//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\IntStream.cpp START
//////////////////////////////////////////////////////////////////////////

void WriteCharToIntStream
(
__global int* intStream,
  int* pCharPos,
  unsigned char charValue
)
{
  int intOffset = *pCharPos % 4;

  int charAsInt = charValue << ((intOffset) * 8);
  int charMaskInverted = ((1 << 8) - 1) << ((intOffset) * 8);
  int charMask = (-1) ^ charMaskInverted;

  int intPos = *pCharPos / 4;
  
  intStream[intPos] &= charMask;
  intStream[intPos] |= charAsInt;

  *pCharPos = *pCharPos + 1;
  
}

char GetCharFromIntStream
(
__global int* intStream,
  int charPos
)
{
  int intOffset = charPos % 4;

  char charValue = (char)(intStream[charPos / 4] >> (intOffset * 8));
  return charValue;
}

void WriteStringToIntStream
(
__global int* intStream,
  int* pCharPos,
  const __constant char* stringToWrite
)
{
  int curPos = 0;

  while(stringToWrite[curPos] != '\0')
  {
    WriteCharToIntStream(intStream, pCharPos, stringToWrite[curPos++]);
  }
}

//Does not round, simply trims the float
void WriteFloatToIntStream
(
__global int* intStream,
  int* pCharPos,
  float number,
  int decimalPlaces
)
{
  //We need to find the first digit, and so the largest digit.
  //Increase the magnitude until we find it
  float curMagnitude = 1;

  if(number < 0)
  {
    WriteCharToIntStream(intStream, pCharPos, '-');
    number *= -1;
  }

  while(number / curMagnitude >= 10)
  {
    curMagnitude *= 10;
  }

  while(curMagnitude >= 1 || decimalPlaces > 0)
  {
    int curDigit = number / curMagnitude;
    WriteCharToIntStream(intStream, pCharPos, (char)(curDigit + 48));
    number -= curDigit * curMagnitude;
    curMagnitude /= 10;

    if(curMagnitude < 0.5 && curMagnitude > 0.05)
    {
      if(decimalPlaces > 0)
        WriteCharToIntStream(intStream, pCharPos, '.');      
    }
    else if(curMagnitude < 0.05)
    {
      decimalPlaces--;
    }
    
  }  
}

/*
void WriteCharToIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos,
  char charValue
)
{
  int intOffset = *pCharPos % 4;

  int charAsInt = charValue << ((intOffset) * 8);
  int charMaskInverted = ((1 << 8) - 1) << ((intOffset) * 8);
  int charMask = (-1) ^ charMaskInverted;

  intStream[*pCharPos / 4] &= charMask;
  intStream[*pCharPos / 4] |= charAsInt;

  (*pCharPos)++;
}

char GetCharFromIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int charPos
)
{
  int intOffset = charPos % 4;

  char charValue = (char)(intStream[charPos / 4] >> (intOffset * 8));
  return charValue;
}

void WriteStringToIntStream
(
#ifdef OPEN_CL
__global int* intStream,
#else
  int* intStream,
#endif

  int* pCharPos,
#ifdef OPEN_CL
  const __constant char* stringToWrite
#else
  const char* stringToWrite
#endif
)
{
  int curPos = 0;

  while(stringToWrite[curPos] != '\0')
  {
    WriteCharToIntStream(intStream, pCharPos, stringToWrite[curPos++]);
  }
}

//Does not round, simply trims the float
void WriteFloatToIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos,
  float number,
  int decimalPlaces
)
{
  //We need to find the first digit, and so the largest digit.
  //Increase the magnitude until we find it
  float curMagnitude = 1;
  int x;

  if(number < 0)
  {
    WriteCharToIntStream(intStream, pCharPos, '-');
    number *= -1;
  }

  while(number / curMagnitude >= 10)
  {
    curMagnitude *= 10;
  }

  while(curMagnitude >= 1 || decimalPlaces > 0)
  {
    int curDigit = number / curMagnitude;
    WriteCharToIntStream(intStream, pCharPos, (char)(curDigit + 48));
    number -= curDigit * curMagnitude;
    curMagnitude /= 10;

    if(curMagnitude < 0.5 && curMagnitude > 0.05)
    {
      if(decimalPlaces > 0)
        WriteCharToIntStream(intStream, pCharPos, '.');      
    }
    else if(curMagnitude < 0.05)
    {
      decimalPlaces--;
    }
    
  }  
}
*/
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\IntStream.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Tests.cpp START
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START Tests.h START
//////////////////////////////////////////////////////////////////////////
#pragma once


void RunBasicSimulations
(
__global int* intStream,
  int* pCharPos
);

void RunSpeedSimulation
(
__global int* intStream,
  int* pCharPos,
  int cSimulations,
  int seed
);
//////////////////////////////////////////////////////////////////////////
//END Tests.h END
//////////////////////////////////////////////////////////////////////////




void SetupBoard
(
  Board* pBoard
)
{
  Loc hardCodedLocation;

  ctor_Board0(pBoard);

 
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 4));  
#ifndef OPEN_CL  
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 5));  
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 6));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 5, 4));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 5, 6));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 6, 4));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 6, 5));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 6, 6));
#endif
#if 0
#if PROFILE
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 0));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 1));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 2));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 1, 0));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 1, 2));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 2, 0));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 2, 1));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 2, 2));

  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 3, 0));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 3, 1));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 3, 2));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 0));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 2));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 5, 0));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 5, 1));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 5, 2));

  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 6, 0));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 6, 1));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 6, 2));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 7, 0));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 7, 2));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 8, 0));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 8, 1));
  Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 8, 2));

  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 3));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 4));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 5));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 6));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 7));
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 0, 8));
#endif
#endif
}

void RunBasicSimulations
(
__global int* intStream,
  int* pCharPos
)
{
  int streamPos = 0;

  //These are needed for FastRandom function calls
  unsigned long mt[N];
  int mti=N+1;
 
  Board board;
 
 
  SetupBoard(&board);
 #ifndef OPEN_CL
  Board_ToString(&board, intStream, pCharPos);
  WriteCharToIntStream(intStream, pCharPos, '\n');  
  
  Board_MetaToString(&board, intStream, pCharPos);
  WriteCharToIntStream(intStream, pCharPos, '\n');

  Board_LibsToString(&board, intStream, pCharPos);
  WriteCharToIntStream(intStream, pCharPos, '\n');    
   
  {
    SimulationResults results;
            
    MonteCarloSimulate(&board, 20452, &results, mt, &mti);    
    WriteStringToIntStream(intStream, pCharPos, "(");
    WriteFloatToIntStream(intStream, pCharPos, (float)results.scoreInFavourOfBlack, 0);
    WriteStringToIntStream(intStream, pCharPos, ") <1st black score>\n");
     
    MonteCarloSimulate(&board, 3452, &results, mt, &mti);
    WriteStringToIntStream(intStream, pCharPos, "(");
    WriteFloatToIntStream(intStream, pCharPos, (float)results.scoreInFavourOfBlack, 0);
    WriteStringToIntStream(intStream, pCharPos, ") <2nd black score>\n");
  
    MonteCarloSimulate(&board, 3422, &results, mt, &mti);
    WriteStringToIntStream(intStream, pCharPos, "(");
    WriteFloatToIntStream(intStream, pCharPos, (float)results.scoreInFavourOfBlack, 0);
    WriteStringToIntStream(intStream, pCharPos, ") <3rd black score>\n");    
  }
#endif
}

void RunSpeedSimulation
(
__global int* intStream,
  int* pCharPos,
  int cSimulations,
  int seed
)
{
  //These are needed for FastRandom function calls
  unsigned long mt[N];
  int mti=N+1;

  SimulationResults results;
  Board board;
      
  double averageResult = 0.0;

  double blackWinPercentage = 0;
  int x;

  SetupBoard(&board);

  init_genrand(seed, mt, &mti);

  results.blackPlays = 0;
  results.whitePlays = 0;

  for(x = 0; x < cSimulations; x++)
  {
    MonteCarloSimulate(&board, genrand_int32(mt, &mti), &results, mt, &mti);
    averageResult += results.scoreInFavourOfBlack;

    if(results.scoreInFavourOfBlack > 0)
      blackWinPercentage++;
  }
  

  WriteStringToIntStream(intStream, pCharPos, "(");
  WriteFloatToIntStream(intStream, pCharPos, (float)averageResult / cSimulations, 5);
  WriteStringToIntStream(intStream, pCharPos, ") <average result> with ");

  WriteStringToIntStream(intStream, pCharPos, "(");
  WriteFloatToIntStream(intStream, pCharPos, (float)blackWinPercentage / cSimulations, 5);
  WriteStringToIntStream(intStream, pCharPos, ") <average black win percentage>\n");

  WriteStringToIntStream(intStream, pCharPos, "(");
  WriteFloatToIntStream(intStream, pCharPos, (float)results.blackPlays / cSimulations, 5);
  WriteStringToIntStream(intStream, pCharPos, ") <average black plays>\n");

  WriteStringToIntStream(intStream, pCharPos, "(");
  WriteFloatToIntStream(intStream, pCharPos, (float)results.whitePlays / cSimulations, 5);
  WriteStringToIntStream(intStream, pCharPos, ") <average white plays>\n");

}
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Tests.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Distributions.cpp START
//////////////////////////////////////////////////////////////////////////
#ifdef PROFILE





#include <math.h>

#include <stdio.h>

float GetAverage(int* data, int count)
{
  float sum = 0;
  int x;
  for(x = 0; x < count; x++)
    sum += data[x];
  return sum / count;
}

float GetSampleStandardDeviation(int* data, int count)
{
  float standardDeviation = 0;
  int x;

  float average = GetAverage(data, count);

  for(x = 0; x < count; x++)
  {
    standardDeviation += (average - data[x]) * (average - data[x]);
  }

  return sqrt(standardDeviation / (count - 1));
}

void CreateDataOutput(int* data, int count, int* outputStream, int* pCharPos)
{ 
  int x;
  for(x = 0; x < count; x++)
  {
    WriteFloatToIntStream(outputStream, pCharPos, (float)data[x], 0);
    WriteCharToIntStream(outputStream, pCharPos, '\n');
  }
}

void CreateDataFloatOutput(float* data, int count, int* outputStream, int* pCharPos, int decimalPlaces)
{ 
  int x;
  for(x = 0; x < count; x++)
  {
    WriteFloatToIntStream(outputStream, pCharPos, data[x], decimalPlaces);
    WriteCharToIntStream(outputStream, pCharPos, '\n');
  }
}
float GetDistributionCoefficient(float targetCertainty);
void TestEarlyTermination(float targetCertainty, int targetValue, float numberOfTrials)
{  
  int* intStream = (int *)calloc(1000000, sizeof(int));
  int charPos = 0;
    
  float* metaStream = (float *)calloc(1000000, sizeof(float));
  int metaCharPos = 0;

  float* metaStream2 = (float *)calloc(1000000, sizeof(float));
  int metaCharPos2 = 0;

  //Start simulating until it is determined we are greater than the target certainty.
  //Then continue to check if we were right.

  //float targetCertainty = 0.75;
  //float targetValue = 5; //We need to be targetCertainty which side of 5 we are on

  //http://en.wikipedia.org/wiki/Confidence_interval 
  //1.96 is for 95% confidence interval (95% of the time within this interval)
  //2.23648 //95% better
  //2.2414  //95% better

  //1.28155 //80% (but yields 75%?)
  //1.64485 //80% better (yields 85.5%?)
  //2.920 (90%)
  //2.015 (85%)
  //t-distribution with 1 degree of freedom (one-sided) appears to be the best...
  //which means the formula is the same as the standard formula but
  //the constant is -cot(pi * certainty percent)
  float confidenceConstant = GetDistributionCoefficient(targetCertainty);

  float totalGuesses = 0;
  float infiniteGuesses = 20000;
  float realAverage = 6.8727;

  float correctGuesses = 0;
  float guessesSkipped = 0;
  //float numberOfTrials = 1000;

  float averageGuessNumber = 0;

  int x;
  int y;
  int z;

  unsigned long mt[N];
  int mti = N + 1;

  init_genrand(2349217, mt, &mti);

  for(x = 0; x < numberOfTrials; x++)
  {
    bool guessed = false;
    bool above = false;
    *GetProfileStreamPos() = 0;
    for(y = 0; y < infiniteGuesses; y++)
    {
      charPos = 0;      
      RunSpeedSimulation(intStream, &charPos, 1, genrand_int32(mt, &mti));

      if(!guessed)
      {
        float standardDeviation = GetSampleStandardDeviation(GetProfileStream(), *GetProfileStreamPos());
        float average = GetAverage(GetProfileStream(), *GetProfileStreamPos());
        //This can of course be greatly optimized...
        float intervalRange = confidenceConstant*(standardDeviation/sqrt((float)(y + 1)));
        float lowerEndpoint = average - intervalRange;
        float upperEndpoint = average + intervalRange;

        if(upperEndpoint < targetValue)
        {
          guessed = true;          
          above = false;

          averageGuessNumber += (y + 1);
          break;
        }
        else if(lowerEndpoint >= targetValue)
        {
          guessed = true;
          above = true;

          averageGuessNumber += (y + 1);
          break;
        }
      }
    }

    {
      if(!guessed)
      {
        guessesSkipped++;
      }
      else
      {
        if(above && realAverage >= 5)
        {
          correctGuesses++;
        }
        else if(!above && realAverage < 5)
        {
          correctGuesses++;
        }
      }  
    }
  }

  {
    int stream[1000];
    int pos = 0;
    float percentCorrect = (float)correctGuesses / numberOfTrials;
    float percentSkipped = (float)guessesSkipped / numberOfTrials;
    averageGuessNumber /= numberOfTrials;
    metaCharPos = 0;
    WriteStringToIntStream(stream, &pos, "\n\nPercent correct: ");
    WriteFloatToIntStream(stream, &pos, percentCorrect, 5);
    WriteStringToIntStream(stream, &pos, "\nTarget Percent correct: ");
    WriteFloatToIntStream(stream, &pos, targetCertainty, 5);
    WriteStringToIntStream(stream, &pos, "\nPercent skipped: ");
    WriteFloatToIntStream(stream, &pos, percentSkipped, 5);
    WriteStringToIntStream(stream, &pos, "\nAverage guess number: ");
    WriteFloatToIntStream(stream, &pos, averageGuessNumber, 5);
    WriteStringToIntStream(stream, &pos, "\nDistribution coefficient: ");
    WriteFloatToIntStream(stream, &pos, confidenceConstant, 5);
    WriteCharToIntStream(stream, &pos, ' ');
    WriteCharToIntStream(stream, &pos, '\0');

    printf("%s", (char*)stream);
  }

  //float intervalRange = 1.28155*(standardDeviation/sqrt((float)subSamples));
  //float lowerEndpoint = average - intervalRange;
  //float upperEndpoint = average + intervalRange;
}

float GetDistributionCoefficient(float targetCertainty)
{
  return -1 / tan(3.1415926535897932384626433 * targetCertainty);
}
void TestDistributions()
{  
  int* profileStream = GetProfileStream();
  (*GetProfileStreamPos()) = 0;

  return;

  TestEarlyTermination(0.50, 6, 1000);
  TestEarlyTermination(0.60, 6, 1000);
  TestEarlyTermination(0.70, 6, 1000);
  TestEarlyTermination(0.80, 6, 1000);
  TestEarlyTermination(0.90, 6, 1000);
  TestEarlyTermination(0.95, 6, 1000);

  (*GetProfileStreamPos()) = 0;

  {

    int* intStream = (int *)calloc(1000000, sizeof(int));
    int charPos = 0;
    int cSimulations = 10000;

    //We do a lot of simulations, and then compile the results in the metaStream
    //BASICALLY: The less the trials vary the more we can rely on them and so the less we need to run
    //we don't bother checking the variance in the individual trials... (although it is likely possible to calculate
    //what we need from that, its just harder), instead we do lots of trial runs to determine how different sample
    //sizes impact the result. Then we will pick one that looks good. It would be ideal to do this at run time
    //as the board will change the number of samples we need to run... but for now there is no need.
    float* metaStream = (float *)calloc(1000000, sizeof(float));
    int metaCharPos = 0;

    float* metaStream2 = (float *)calloc(1000000, sizeof(float));
    int metaCharPos2 = 0;

    unsigned long mt[N];
    int mti = N + 1;

    init_genrand(2349217, mt, &mti);

    RunSpeedSimulation(intStream, &charPos, 60000, genrand_int32(mt, &mti));
    charPos = 0;
    CreateDataOutput(GetProfileStream(), *GetProfileStreamPos(), intStream, &charPos);

    {
      int numberOfSamplings = 20000;
      int samples = 100;
      int subSamples = 10;
      int x;
      float average;
      float standardDeviation;

      //float standardMaxStandardDeviation;

      //int standardDeviationSamples = 10000;

      //The average and standard deviation themselves are inexact, so we create a confidence interval around the
      //the standard deviation based on a large number of samples, and use this to increase or decrease the standard deviation
      //calculated in future samplings

      //charPos = 0;
      //*GetProfileStreamPos() = 0;
      //RunSpeedSimulation(intStream, &charPos, standardDeviationSamples, genrand_int32(mt, &mti));

      //average = GetAverage(GetProfileStream(), *GetProfileStreamPos());
      //standardDeviation = GetSampleStandardDeviation(GetProfileStream(), *GetProfileStreamPos());

      //standardMaxStandardDeviation = average + 1.96*(standardDeviation/sqrt((float)standardDeviationSamples));

      //for(x = 0; x < numberOfSamplings; x++)
      {        
        charPos = 0;
        *GetProfileStreamPos() = 0;
        RunSpeedSimulation(intStream, &charPos, samples, genrand_int32(mt, &mti));

        average = GetAverage(GetProfileStream(), *GetProfileStreamPos());
        standardDeviation = GetSampleStandardDeviation(GetProfileStream(), *GetProfileStreamPos());

        {
          //http://en.wikipedia.org/wiki/Confidence_interval 
          //1.96 is for 95% confidence interval (95% of the time within this interval)
          //2.23648 //95% better
          //2.2414  //95% better

          //1.28155 //80% (but yields 75%?)
          //1.64485 //80% better (yields 85.5%?)
          float intervalRange = 1.28155*(standardDeviation/sqrt((float)subSamples));
          float lowerEndpoint = average - intervalRange;
          float upperEndpoint = average + intervalRange;

          int amountInInterval = 0;
          int amountOutofInterval = 0;

          for(x = 0; x < numberOfSamplings; x++)
          {
            charPos = 0;
            *GetProfileStreamPos() = 0;
            RunSpeedSimulation(intStream, &charPos, subSamples, genrand_int32(mt, &mti));

            {
              float result = GetAverage(GetProfileStream(), *GetProfileStreamPos());

              if(result > lowerEndpoint && result < upperEndpoint)
              {
                amountInInterval++;
              }
              else
              {
                amountOutofInterval++;
              }
            }
          }

          {
            int stream[1000];
            int pos = 0;
            float percentCorrect = (float)amountInInterval / numberOfSamplings;
            float percentInCorrect = (float)amountOutofInterval / numberOfSamplings;
            metaCharPos = 0;
            WriteStringToIntStream(stream, &pos, "\n\nPercent correct: ");
            WriteFloatToIntStream(stream, &pos, percentCorrect, 5);
            WriteCharToIntStream(stream, &pos, ' ');
            WriteCharToIntStream(stream, &pos, '\0');

            printf("%s", (char*)stream);
          }
        }
      }

      charPos = 0;
      CreateDataFloatOutput(metaStream, metaCharPos, intStream, &charPos, 3);
      charPos = 0;
      CreateDataFloatOutput(metaStream2, metaCharPos2, intStream, &charPos, 3);
    }


    RunSpeedSimulation(intStream, &charPos, cSimulations, 73471);

    charPos = 0;
    CreateDataOutput(GetProfileStream(), *GetProfileStreamPos(), intStream, &charPos);

    RunSpeedSimulation(intStream, &charPos, cSimulations, 73470);
  }
}

#endif
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Distributions.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetLinked.cpp START
//////////////////////////////////////////////////////////////////////////




/*
void ctor_FastSetPiece0(FastSetLinked* m, Board* containingBoard)
{
  int x;

  DASSERT(containingBoard);

  m->sharedStorage = &containingBoard->sharedFastsetStorage;
  
  m->curDataStart = m->sharedStorage->curDataHead;
  m->curDataEnd = m->sharedStorage->curDataHead;

  m->board = containingBoard;
}

void ctor_FastSetPiece1(FastSetLinked* m, FastSetPiece* other)
{
  int x;
  m->squarePositions[cSquareIntPositions] = other->curDataHead;
  
  for(x = 0; x < cSquarePositions; x++)
    other->squarePositions[x] = m->squarePositions[x];

  m->board = other->board;
}

//Could return if it inserts it, but for now it is not used so it is not implemented
void FastSetPiece_insert(FastSetLinked* m, Square* square)
{
  //First checks if it already exists
  {
    int x;
    for(x = m->curDataStart; x < m->curDataEnd; x++)    
      if(Loc_Equ(&Board_GetSqr(m->board, m->sharedStorage->squarePositions, x)->location, &square->location))
        return;
  }

  if(m->curDataEnd != m->sharedStorage->curDataHead)
  {
    //We have to make room for ourselves.
  }
  FastSetPiece_setAt(m, m->squarePositions[cSquareIntPositions]++, square);
  //m->data[m->squarePositions[cSquareIntPositions]++] = piece;
  
  DASSERT(m->squarePositions[cSquareIntPositions] >= 0 && m->squarePositions[cSquareIntPositions] < TOTAL_SQUARES);

}
 
int FastSetPiece_length(FastSetLinked* m)
{
  return m->squarePositions[cSquareIntPositions];
}

Square* FastSetPiece_getAt(FastSetLinked* m, int pos)
{
  return Board_GetSqr(m->board, m->squarePositions, pos);
}

void FastSetPiece_setAt(FastSetLinked* m, int pos, Square* square)
{
  Board_SetSqr(m->board, m->squarePositions, pos, square);

  DASSERT((FastSetPiece_getAt(m, pos) == square));
}

Square* FastSetPiece_getAtOtherBoard(FastSetLinked* m, int pos, Board* board)
{
  return Board_GetSqr(board, m->squarePositions, pos);
}

//no need to overload it, if we increment the pointer by 1 it performs the desired functionality
//friend Square** operator++ ( Square**

void FastSetPiece_clear(FastSetLinked* m)
{
  m->squarePositions[cSquareIntPositions] = 0;
}

int FastSetPiece_size(FastSetLinked* m)
{  
  return m->squarePositions[cSquareIntPositions];
}

//Should be faster than erase(Square* piece), as you give the exact location to the piece
void FastSetPiece_eraseAt(FastSetLinked* m, int pos)
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

void FastSetPiece_erase(FastSetLinked* m, Square* piece)
{
  int pos = 0;

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

bool FastSetPiece_contains(FastSetLinked* m, Square* piece)
{
  int x;
  for(x = 0; x < m->squarePositions[cSquareIntPositions]; x++)    
    if(Loc_Equ(&FastSetPiece_getAt(m, x)->location, &piece->location))
      return true;
  return false;
}
*/
//////////////////////////////////////////////////////////////////////////
//END C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetLinked.cpp END
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//START EntryPoint.cpp START
//////////////////////////////////////////////////////////////////////////


#include <time.h>







#include <stdio.h>


#ifdef PROFILE
//////////////////////////////////////////////////////////////////////////
//START Distributions.h START
//////////////////////////////////////////////////////////////////////////
#ifdef PROFILE

void TestDistributions();

#endif
//////////////////////////////////////////////////////////////////////////
//END Distributions.h END
//////////////////////////////////////////////////////////////////////////
#endif
//setup a build console to use new C++ 11 stuff (not sure if it will already)
//and full optimization

//Avg Sim time (milli)    1st Result    2nd   3rd     Avg Result    Avg Black Win
//(per montecarlo     
//simulation)

//0.3
//0.109                   -18           11    24      9.469         0.715

int main (int argc, char** argv)
{   

#ifndef C
  try
  {
#endif
 
  
long t1 = clock(); double time;

int cSimulations = 1000;

int intStream[10000];
int charPos = 0;

//We have to time both... as in OpenCL we need to time both too
RunBasicSimulations(intStream, &charPos);
RunSpeedSimulation(intStream, &charPos, cSimulations, 73471);
    
time = clock() - t1;

WriteCharToIntStream(intStream, &charPos, '{');
WriteFloatToIntStream(intStream, &charPos, (cSimulations * 1000) / (time), 2);
WriteStringToIntStream(intStream, &charPos, " per second");
WriteCharToIntStream(intStream, &charPos, '}');

WriteCharToIntStream(intStream, &charPos, '\0');

printf("%s", (char*)intStream);

#ifndef C
  } catch(exception e)
  {
    cout << e.what();
  }
#endif  

#ifdef PROFILE
  TestDistributions();
#endif

  //Gives you a chance to read the output
  //DASSERT(false);

  //while(true);

  return 0;
}

//////////////////////////////////////////////////////////////////////////
//END EntryPoint.cpp END
//////////////////////////////////////////////////////////////////////////
