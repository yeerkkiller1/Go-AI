#include "Location.h"

#include "Constants.h"

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

#include "IntStream.h"
void Loc_ToString(Loc* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                  int* pCharPos)
{
  WriteCharToIntStream(intStream, pCharPos, (char)(m->x + 48));
  WriteCharToIntStream(intStream, pCharPos, ',');
  WriteCharToIntStream(intStream, pCharPos, ' ');
  WriteCharToIntStream(intStream, pCharPos, (char)(m->y + 48));
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

unsigned char Loc_CompressPosition(int x, int y)
{
  return (x << 4) + y;
}