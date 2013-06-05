#include "Location.h"

#include "Constants.h"

#ifndef OPEN_CL
  #include <stdlib.h>
#endif

bool	Loc_IsOnBoard	(Loc* piece)
{
	return (piece->x < BOARD_SIZE && piece->y < BOARD_SIZE && piece->x >= 0 && piece->y >= 0);
}

bool Loc_Equ(Loc* p1,  Loc* p2)
{
  return p1->x == p2->x && p1->y == p2->y;
}

bool Loc_NotEqu (Loc* p1,  Loc* p2)
{
  return p1->x != p2->x || p1->y != p2->y;
}

char* Loc_ToString(Loc* m)
{
  char* toReturn = (char*)malloc(10);
  toReturn[0] = (char)(m->x + 48);
  toReturn[1] = ',';
  toReturn[2] = ' ';  
  toReturn[3] = (char)(m->y + 48);
  toReturn[4] = '\0';
  return toReturn;
}

void ctor_Loc0(Loc* obj)
{
  obj->x = -10;
  obj->y = -10;
}

Loc* ctor_Loc1(Loc* obj, int x, int y)
{
  obj->x = x;
  obj->y = y;

  return obj;
}

void ctor_Loc2 (Loc* obj,  Loc* other )
{
  obj->x = other->x;
  obj->y = other->y;
}

Loc* ctor_leak_Loc(int x, int y)
{
  Loc* leaker = (Loc*)malloc(sizeof(Loc));
  ctor_Loc1(leaker, x, y);
  return leaker;
}