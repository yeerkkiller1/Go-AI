#include <stdlib.h>
#include <vector>



#include "Location.h"
#include "Board.h"

using namespace std;

template <typename T>
vector<T*> SurroundingSquares (void * (*LocFnc) (Location, Board*), Location location, Board * board)
{
	if(!location.IsOnBoard())
		throw exception("calling surronding squares with a location off the board!");

  //vector for now, so we don't accidently remove duplicates!
	vector<T*> outputs;

	if(location.x + 1 < BOARD_SIZE)	
		outputs.push_back(
		(T*) LocFnc(Location(location.x + 1, location.y), board)		
		);

	if(location.x - 1 >= 0)	
		outputs.push_back(
		(T*) LocFnc(Location(location.x - 1, location.y), board)		
		);

	if(location.y + 1 < BOARD_SIZE)	
		outputs.push_back(
		(T*) LocFnc(Location(location.x, location.y + 1), board)		
		);

	if(location.y - 1 >= 0)	
		outputs.push_back(
		(T*) LocFnc(Location(location.x, location.y - 1), board)		
		);

	return outputs;
}