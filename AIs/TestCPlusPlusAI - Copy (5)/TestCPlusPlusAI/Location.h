#pragma once

#include "q.h"
#include "Constants.h"


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