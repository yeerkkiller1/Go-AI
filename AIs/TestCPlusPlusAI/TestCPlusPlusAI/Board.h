#pragma once


#include "Square.h"
#include "q.h"

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


#include "libertyGroup.h"



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
//decide how to compress it within the unsigned char* position).
Square* Board_GetSqr(Board* m, unsigned char* positions, int squarePos);

//The counterpart to Board_GetSqr
void Board_SetSqr(Board* m, unsigned char* positions, int squarePos, Square* square);

//returns number of squares filled
//outputs should be 4 pieces big
int SurroundingSquares(Square** outputs, Loc location, Board* board);