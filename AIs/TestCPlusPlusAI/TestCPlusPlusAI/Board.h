#pragma once

#include "Piece.h"
#include "libertyGroup.h"
#include <string>

#include "Constants.h"

class Board
{
public:	
	int whiteTakenOff;
	int blackTakenOff;

  Pieces curTurn;

	Board();
  Board( const Board& other );

  ~Board()
  {

  }

	void				  PlayPiece	(Pieces type, Location location);

  //Anyone who gets a piece can also set it... this is because
  //it would be inefficient to have them copy another piece
  //when we have already allocated one.
	Piece*			  GetPiece	    (Location location);
  static void * GetPieceVoid  (Location location, Board* board);

  //void				SetPiece	(Location location, Piece piece);

  void        RemoveGroup (libertyGroup* group);
  void        RemovePiece (Location location);  
	
  void        ChangeLiberties  (libertyGroup* group, int delta);

	libertyGroup*		GetLibertyGroup		  (Location location);
  static void *   GetLibertyGroupVoid (Location  location, Board* board);
  
  std::string ToString() const
  {
    std::string toReturn = "";

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

  std::string MetaToString() const
  {
    std::string toReturn = "";

    toReturn += " 012345678\n";
    for(int y = 0; y < BOARD_SIZE; y++)
    {
      toReturn += (char)(y + 48);
      for(int x = 0; x < BOARD_SIZE; x++)      
        toReturn += (char)(board[x][y].libGroup->size() + 48);      
      toReturn += "\n";
    }
    return toReturn;
  }

  std::string LibsToString() const
  {
    std::string toReturn = "";

    toReturn += " 012345678\n";
    for(int y = 0; y < BOARD_SIZE; y++)
    {
      toReturn += (char)(y + 48);
      for(int x = 0; x < BOARD_SIZE; x++)
        toReturn += (char)(board[x][y].libGroup->liberties.size() + 48);      
      toReturn += "\n";
    }
    return toReturn;
  }

private:
	Piece					    board [BOARD_SIZE][BOARD_SIZE];	
};