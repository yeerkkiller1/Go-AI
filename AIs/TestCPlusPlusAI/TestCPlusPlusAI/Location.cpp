#include "Location.h"
#include "Constants.h"

bool	Location::IsOnBoard	()
{
	return (x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0);
}

bool operator== (Location &p1, Location &p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

bool operator!= (Location &p1, Location &p2)
{
  return p1.x != p2.x || p1.y != p2.y;
}