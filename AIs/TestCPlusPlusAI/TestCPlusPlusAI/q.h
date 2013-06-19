//#pragma once

//Higher levels are slower but check more
//#define DEBUG_LEVEL_1
//#define DEBUG_LEVEL_2
//#define DEBUG_LEVEL_3 //Diagnostic output
#include "Constants.h"

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