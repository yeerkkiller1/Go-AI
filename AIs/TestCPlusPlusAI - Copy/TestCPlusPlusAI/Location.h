#pragma once

typedef struct Loc Loc;

#include "Constants.h"


//This is abstracted even though for now it will only be two ints
//in the future it will likely have (static?) helper functions for simple things
//that will be extremely optimized.
struct Loc
{
	int x, y;  	
};

bool Loc_IsOnBoard(Loc* m);

char* Loc_ToString(Loc* m);

bool Loc_Equ(Loc* p1, Loc* p2);
bool Loc_NotEqu (Loc* p1, Loc* p2);   

void ctor_Loc(Loc* obj);

Loc* ctor_Loc(Loc* obj, int x, int y);

void ctor_Loc (Loc* obj,  Loc* other );

Loc* ctor_leak_Loc(int x, int y);