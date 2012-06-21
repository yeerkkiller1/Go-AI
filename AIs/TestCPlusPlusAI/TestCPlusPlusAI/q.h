#pragma once

//template <class T, size_t ROW, size_t COL>
//using Matrix = std::array<std::array<T, COL>, ROW>;

//template <T> 

//Higher levels are slower but check more
//#define DEBUG_LEVEL_1
//#define DEBUG_LEVEL_2
//#define DEBUG_LEVEL_3 //Diagnostic output


#include "Metadata.h"
#include <stdlib.h>

#include <vector>

using namespace std;

//Calls the function with all its valid surrounding squares and returns
//an array of the returned inputs
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