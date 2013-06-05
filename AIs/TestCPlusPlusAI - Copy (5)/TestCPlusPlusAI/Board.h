#pragma once


#include "Piece.h"
#include "q.h"

struct Board
{
	int whiteTakenOff;
	int blackTakenOff;

  Pieces curTurn;

	Piece					    board [BOARD_SIZE][BOARD_SIZE];	
};


#include "libertyGroup.h"



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
 
void Board_ToString
(
  Board* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos);
void Board_MetaToString(
  Board* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos);
void Board_LibsToString(
  Board* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos);

//returns number of squares filled
//outputs should be 4 pieces big
int SurroundingSquares(Piece* outputs[], Loc location, Board* board);