#pragma once

#include <set>
#include "Piece.h"

//These are compared directly, as you should not copy lib groups (they should all
//reference the original square's group, which founded the group)

//The group contains the pieces connected, liberties is all the empty squares that they
//consider as liberties to them
class libertyGroup : public std::set<Piece*>
{
public:
	//ATTENTION! This must be maintained by an outside source as we do not maintain it!
  //By definition a liberty is an empty square, so that is what this contains
	set<Piece*> liberties;


  //MUST BE FILLED IN BY BOARD!
  Piece* owner;

  libertyGroup( const libertyGroup& other );

  libertyGroup()
  {
    //Should probably mark the class as unusable until owner is filled in...
    //as we want it to be defaultedly constructed (so its on the stack) but it does
    //need some initialiaation
  }

};