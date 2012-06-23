#include "Location.h"
#include "Constants.h"

bool Location::COPY_ALLOWED = false;

bool	Location::IsOnBoard	()
{
	return (x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0);
}

bool operator== (const Location &p1, const Location &p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

bool operator!= (const Location &p1, const Location &p2)
{
  return p1.x != p2.x || p1.y != p2.y;
}