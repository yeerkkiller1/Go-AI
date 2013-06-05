#pragma once
#include "FastSetReplacement.h"

//These are compared directly, as you should not copy lib groups (they should all
//reference the original square's group, which founded the group)

//The group contains the pieces connected, liberties is all the empty squares that they
//consider as liberties to them
struct libertyGroup
{
	//ATTENTION! This must be maintained by an outside source as we do not maintain it!
  //By definition a liberty is an empty square, so that is what this contains
	FastSetPiece liberties;
  FastSetPiece group;

  //MUST BE FILLED IN BY BOARD!
  Piece* owner;
};

void ctor_libertyGroup0(libertyGroup* m, libertyGroup* other);
void ctor_libertyGroup1(libertyGroup* m);