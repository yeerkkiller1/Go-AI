#include "Board.h"
#include "q.h"

#include <algorithm>

//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board& board, Pieces type, Location location)
{  
  Piece* pieceCur = board.GetPiece(location);

  //Prevent placing where you previously got taken
  if(pieceCur->hadBlack || pieceCur->hadWhite)
    return false;

  //Prevent placing in immediate suicide  
  //Scope and make a temp copy, and then place and see if it is suicide! (bad... but for not it works)
  {
    //Eventually screen lots of these calls out... as usually they are actually not needed

    Board copy(board);
    copy.PlayPiece(type, location);
    if(copy.GetLibertyGroup(location)->size() == 0)
    {
      //Then its suicide, and we don't place there!
      return false;
    }
  }
  
  //Prevent placing when you have TOO many friendly and no threats
  int friendly = 0;
  int enemy = 0;

  vector<Piece*> pieceAdjs = SurroundingSquares<Piece>(&Board::GetPieceVoid, location, &board);            
  for_each(begin(pieceAdjs), end(pieceAdjs), [&type, &friendly, &enemy] (Piece* pieceSur) {       
    if(pieceSur->pieceTypeCur == type)    
      friendly++;    
    else    
      enemy++;    
  });


  friendly += 4 - pieceAdjs.size(); //Off the board is good too

  if(enemy == 0 && friendly >= 3)
    return false;

  return true;
}