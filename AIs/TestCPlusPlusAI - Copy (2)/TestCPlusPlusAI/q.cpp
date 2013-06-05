


#include "Location.h"
#include "Board.h"


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
    ctor_Loc(&tempLoc, location.x + 1, location.y));		

	if(location.x - 1 >= 0)	
    outputs[numOutputs++] = Board_GetPiece(board, 
    ctor_Loc(&tempLoc, location.x - 1, location.y));

	if(location.y + 1 < BOARD_SIZE)	
    outputs[numOutputs++] = Board_GetPiece(board, 
    ctor_Loc(&tempLoc, location.x, location.y + 1));

	if(location.y - 1 >= 0)	
    outputs[numOutputs++] = Board_GetPiece(board, 
    ctor_Loc(&tempLoc, location.x, location.y - 1));

  return numOutputs;
}