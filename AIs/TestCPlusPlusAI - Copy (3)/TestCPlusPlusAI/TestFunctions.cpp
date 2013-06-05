#include "Board.h"
#include "q.h"



/*
set<Piece> pieces;
void DestructiveNonThreadSafeMakeLibertyGroup(Board& board, Pieces pieceTypeCur, Loc location)
{  
  pieces.insert(pieceTypeCur);

  set<Piece*> splitPieces;

  vector<Piece*> surrounding = SurroundingSquares<Piece>(&Board_GetPieceVoid, 
    location, &board);
  for_each(begin(surrounding), end(surrounding), [&pieceTypeCur, &splitPieces] (Piece* piece) {
    if(piece->pieceTypeCur == pieceTypeCur)
      splitPieces.insert(piece);
  });

  board.GetPiece(location)->pieceTypeCur = Marked;//Marks it, so we don't go to it again

  //split
  for_each(begin(splitPieces), end(splitPieces), [&board, pieceTypeCur] (Piece* piece) {
    if(board.GetPiece(piece->location)->pieceTypeCur != Marked) //Double check, incase it has changed
      DestructiveNonThreadSafeMakeLibertyGroup(board, pieceTypeCur, piece->location);
  });
}

*/