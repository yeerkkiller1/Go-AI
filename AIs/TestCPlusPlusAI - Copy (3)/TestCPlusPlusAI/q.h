#pragma once
static bool COPY_ALLOWED = false;

//Higher levels are slower but check more
//#define DEBUG_LEVEL_1
//#define DEBUG_LEVEL_2
//#define DEBUG_LEVEL_3 //Diagnostic output


#include "Location.h"

struct Board;
struct Piece;




//returns number of squares filled
//outputs should be 4 pieces big
int SurroundingSquares(Piece* outputs[], Loc location, Board* board);