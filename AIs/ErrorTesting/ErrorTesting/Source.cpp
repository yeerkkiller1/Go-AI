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

#ifdef OPEN_CL
  #define COMPRESS_MEMORY
#endif

#ifdef OPEN_CL
  #define LOCAL_MEM_FENCE barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE)
#else
  #define LOCAL_MEM_FENCE 
#endif

//#pragma once

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
#define BOARD(boardPointer, x,y) (&( boardPointer )->board[x + y*BOARD_SIZE])
#endif


#define Pieces  int
#define Black   1 << 5
#define White   1 << 7
#define Empty   1 << 9
#define Marked  1 << 11

struct Loc
{
	int x, y;  	
};
struct FastSetPiece// : public std::set<Square *>
{
  Board* board;

  //Square* data[TOTAL_SQUARES];  

  //We need enough for 81*5 squares as that is the max sum of liberties and
  //pieces played (it is actually a bit less, but this is fine)
//#define cSquareIntPositions 102
#ifdef COMPRESS_MEMORY
  #define cSquareIntPositions 1
#else
  #define cSquareIntPositions 21
#endif

#define cSquarePositions 408

  int squarePositions[cSquareIntPositions];
  int curDataHead;
};
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
struct Square
{
  //MUST BE FILLED IN BY BOARD!
  Loc location;  

  Square* group;
  //This is NOT our liberties, this instead is the group we own, that way if we need one we
  //can get one right away.
  libertyGroup ownedGroup;
	
	Pieces pieceTypeCur;

  //Initially false, set when we place and not unset after we remove 
  //(so if hadBlack and pieceTypeCur == Empty, it has a black captured :D )
  bool hadBlack;
  bool hadWhite;
};

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

#ifndef OPEN_CL
int main(){}
#endif

void RunBasicSimulations
(
__global int* intStream,
  int* pCharPos
)
{
  Board board;
  Board* pBoard = &board;
  Board* m = pBoard;
  m->board[0].group = &m->board[0];  
}

//"C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\EntryPoint.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\q.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastRandom.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Location.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetReplacement.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\libertyGroup.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Square.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Board.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Prune.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\IntStream.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Tests.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Distributions.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\FastSetLinked.cpp" -run