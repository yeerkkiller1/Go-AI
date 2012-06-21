#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <cstdlib>

#include <vector>

#include <chrono>
//#include <string>
#include <memory>

#include "Constants.h"

//#include "q.h"

using namespace std;

enum Pieces { Black, White, Empty, Marked};

//This is abstracted even though for now it will only be two ints
//in the future it will likely have (static?) helper functions for simple things
//that will be extremely optimized.
class Location
{
public :
	int x, y;

  //Will give it invalid values, and eventually will be made to crash if not initalized before use
  Location()
  {
    x = -10;
    y = -10;
  }
	Location(int x, int y) : x(x), y(y) {}

	bool IsOnBoard();

  friend bool operator== (Location &p1, Location &p2);

	friend bool operator!= (Location &p1, Location &p2); 
};

class Piece
{
public:
	bool IsBlack();
	bool IsWhite();
	bool IsEmpty();	

  //Only set after it is added to the board
  Location location;

	Pieces Opposite();

	Piece() : curPiece(Empty), hadBlack(false), hadWhite(false) {}
	Piece(Pieces initial) : curPiece(initial), hadBlack(false), hadWhite(false) {}
	~Piece(){}

	friend bool operator== (Piece p1, Piece p2);
	friend bool operator!= (Piece p1, Piece p2);  	

	Pieces curPiece;

  //Initially false, set when we place and not unset after we remove 
  //(so if hadBlack and curPiece == Empty, it has a black captured :D )
  bool hadBlack;
  bool hadWhite;

  string ToString() const
  {
    if(curPiece == Black)
      return "b";
    else if(curPiece == White)
      return "w";
    else if(curPiece == Marked)
      return "M";
    else
      return " ";
  }
};

//Hmm this may be a bit strange...
class libertyGroup : public vector<Piece>
{
public:
	//ATTENTION! This must be maintained by an outside source as we do not maintain it!
	int liberties; //This should be copied properly as it is a basic datatype?

	libertyGroup() : liberties(0)
	{ }
};

//Actually, we won't use pimpl as many of these things will need to be fast
//and pimpl is slightly slower
class Board
{
public:	
	int whiteTakenOff;
	int blackTakenOff;

  Pieces curTurn;

	Board():whiteTakenOff(0), blackTakenOff(0), curTurn(Black)  
  { 
    for(int y = 0; y < BOARD_SIZE; y++)
      for(int x = 0; x < BOARD_SIZE; x++)
        board[x][y].location = Location(x,y);
  }

	void				  PlayPiece	(Piece piece, Location location);
	Piece				  GetPiece	(Location location);
  static void * GetPieceVoid (Location location, Board board);

  void				SetPiece	(Location location, Piece piece);

  void        RemoveGroup (libertyGroup group);
  void        RemovePiece (Location location);  
	
  void        ChangeLiberties  (libertyGroup group, int delta);

	libertyGroup		GetLibertyGroup		(Location location);
  static void *   GetLibertyGroupVoid (Location location, Board board);
  
  string ToString() const
  {
    string toReturn = "";

    toReturn += " 012345678\n";
    for(int y = 0; y < BOARD_SIZE; y++)
    {
      toReturn += (char)(y + 48);
      for(int x = 0; x < BOARD_SIZE; x++)      
        toReturn += board[x][y].ToString();      
      toReturn += "\n";
    }
    return toReturn;
  }

  string MetaToString() const
  {
    string toReturn = "";

    toReturn += " 012345678\n";
    for(int y = 0; y < BOARD_SIZE; y++)
    {
      toReturn += (char)(y + 48);
      for(int x = 0; x < BOARD_SIZE; x++)      
        toReturn += (char)(libertyBoard[x][y].size() + 48);      
      toReturn += "\n";
    }
    return toReturn;
  }

  string LibsToString() const
  {
    string toReturn = "";

    toReturn += " 012345678\n";
    for(int y = 0; y < BOARD_SIZE; y++)
    {
      toReturn += (char)(y + 48);
      for(int x = 0; x < BOARD_SIZE; x++)
        toReturn += (char)(libertyBoard[x][y].liberties + 48);      
      toReturn += "\n";
    }
    return toReturn;
  }

private:
	Piece					    board [BOARD_SIZE][BOARD_SIZE];	
	libertyGroup			libertyBoard [BOARD_SIZE][BOARD_SIZE];	
};

void * GetLibertyGroupVoid (Location location, Board board);

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