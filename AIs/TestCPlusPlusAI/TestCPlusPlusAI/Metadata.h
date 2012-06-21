#pragma once

//Try not to include all of this... just include the ones you need (maybe)

//Just includes the appropriate metadata files
#include "Board.h"
#include "libertyGroup.h"
#include "Piece.h"
#include "Location.h"


/*
//The is basically the interface for Metadata.
//If a different impl is defined then Metadata can work differently.
//It will also declare its functions here, but its declartions should
//never be over 3 lines (and should basically just be function calls
//to its impl.
class Metadata
{
private:
  class impl;
  unique_ptr<impl> pimpl;

public:
  Metadata::Metadata();
  Metadata::~Metadata();

  //All of the Metadata declarations    

  virtual int GetExampleInt() = 0;
};
*/