#pragma once




#define Pieces  int
#define Black   1 << 5
#define White   1 << 7
#define Empty   1 << 9
#define Marked  1 << 11

#include "Location.h"
#include "libertyGroup.h"

#include "q.h"

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
