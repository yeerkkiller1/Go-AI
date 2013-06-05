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

void Piece_ToString(Piece* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                    int* pCharPos);
void Piece_Group(Piece* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                 int* pCharPos);
void Piece_LibsInGroup(Piece* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                       int* pCharPos);