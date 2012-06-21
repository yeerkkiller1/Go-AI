#include "Metadata.h"
#include "MontecarloSimulation.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "FastRandom.h"

#include "q.h"

using namespace std;

vector<Piece> pieces;
void DestructiveNonThreadSafeMakeLibertyGroup(Board& board, Piece curPiece)
{  
  pieces.push_back(curPiece);

  vector<Piece> splitPieces;

  vector<Piece> surrounding = SurroundingSquares<Piece>(&Board::GetPieceVoid, 
    curPiece.location, &board);
  for_each(begin(surrounding), end(surrounding), [&curPiece, &splitPieces] (Piece& piece) {
    if(curPiece.curPiece == piece.curPiece)
      splitPieces.push_back(piece);      
  });

  curPiece.curPiece = Marked;
  board.SetPiece(curPiece.location, curPiece); //Marks it, so we don't go to it again

  //split
  for_each(begin(splitPieces), end(splitPieces), [&board] (Piece& piece) {
    if(board.GetPiece(piece.location).curPiece != Marked) //Double check, incase it has changed
      DestructiveNonThreadSafeMakeLibertyGroup(board, piece);
  });
}



SimulationResults::SimulationResults(Board board)
{
  scoreInFavourOfBlack = 0;

  scoreInFavourOfBlack += board.whiteTakenOff;
  scoreInFavourOfBlack -= board.blackTakenOff;

  //We mark the squares, so we can spend time looking for the owner of a group doing work also

  #define MAX_BOARD_LOOPS 200

  for(int x = 0; x < BOARD_SIZE; x++)
  {      
    for(int y = 0; y < BOARD_SIZE; y++)      
    {
      Piece mark(Marked);
        
      Location cur(x, y);        
      if(board.GetPiece(cur).curPiece == Empty)
      {          
        int marked = 1;
        //Pieces originalType = board.GetPiece(cur).curPiece;
          
        board.SetPiece(cur, mark);            

        //Spiral outwards until you hit a fill square, we are assuming that 
        //all enclosed areas are surrounded by one kind... and so that square is the owner

        int velX[4] = {1, 0, -1, 0};
        int velY[4] = {0, 1, 0, -1};

        int curTurn = 0;                    
        //(1, 0) -> (0, -1) -> (-1, 0) -> (0, 1)
        Location corner;

        //We turn right

        //Sort of hackish... but it fixes some cases (still not great)
        int maxSearches = MAX_BOARD_LOOPS;
        int curSearchs = 0;

        while(++curSearchs < maxSearches)
        {
          cur = Location(cur.x + velX[curTurn], cur.y + velY[curTurn]);

          if(!cur.IsOnBoard()) //Go back, and then turn
          {
            cur = Location(cur.x - velX[curTurn], cur.y - velY[curTurn]);

            curTurn = (curTurn + 1) % 4;
          }
          else if (board.GetPiece(cur).curPiece == Black)
          {
            scoreInFavourOfBlack += marked;
            break;
          }
          else if (board.GetPiece(cur).curPiece == White)
          {
            scoreInFavourOfBlack -= marked;
            break;
          }
          else //Empty or Marked 
          {              
            if(board.GetPiece(cur).curPiece == Empty) 
            {
              board.SetPiece(cur, mark);
              marked++;
            }
            else //Marked
            {
              //(do nothing)
            }
          }

          corner = Location(cur.x + velX[(curTurn + 1) % 4], cur.y + velY[(curTurn + 1) % 4]);
          if(corner.IsOnBoard() &&
            (board.GetPiece(corner).curPiece == Empty))
            //Turn
          {
            curTurn = (curTurn + 1) % 4;
          }
        } //End of finding partial size of container and owner

        if(curSearchs == maxSearches)
        {            
          if(scoreInFavourOfBlack - marked < 0 && scoreInFavourOfBlack + marked >= 0)
          {
            //We cannot call the game as the algorithm to find which pieces are owned by who
            //is not perfect (infact it sucks). It needs to be entirely closed, and so recursion
            //is not acceptable. A large enough manual stack may work, but something that is
            //even better should be created (a zigzag and loop following pattern perhaps).
            throw exception("due to problems with owned recognition, this game cannot be called");
          }
          scoreInFavourOfBlack += marked;                          
        }

      } //End of is empty
    } //x loop    
  } //y loop


} //function


SimulationResults MonteCarloSimulate(Board board, int seed)
{    
  init_genrand(seed);

  bool lastPassed = false;

  while(true)
  {
    Piece curPiece(board.curTurn);

    int xPos = genrand_int32() % 9;
    int yPos = genrand_int32() % 9;
    //Find a position which we can place to, or pass

    int tries = 0; //We cut off after a certain amount of tries...
    //really late game this is inefficient... but for now its fine



    while(board.GetPiece(Location(xPos, yPos)).curPiece != Empty ||
          !SimplePrune(board, curPiece, Location(xPos, yPos)))
    {
      xPos = genrand_int32() % 9;
      yPos = genrand_int32() % 9;

      if(++tries > 100 )
        break;
    }

    if(tries > 100)
    {
      if(lastPassed) //Two passes in a row, game over!
      {
        break;
      }
      //else

      lastPassed = true;
      continue;
    }
    else
    {
      lastPassed = false;
    }
    //else

    //We get to play it!
    Board boardBefore = board;
    board.PlayPiece(curPiece, Location(xPos, yPos));
    
    #ifdef DEBUG_LEVEL_3
    Board temp2 = board;
    temp2.SetPiece(Location(xPos, yPos), Piece(Marked));    

    cout << temp2.ToString().c_str();
    #endif

#ifdef DEBUG_LEVEL_1
    Board temp = board;
    for(int x = 0; x < BOARD_SIZE; x++)
    {
      for(int y = 0; y < BOARD_SIZE; y++)
      {
        //might as well check everyone square        
        Piece piece = temp.GetPiece(Location(x,y));

        if(piece.IsBlack() || piece.IsWhite())
        {
          pieces.clear();
          DestructiveNonThreadSafeMakeLibertyGroup(temp, piece); 
          libertyGroup group = board.GetLibertyGroup(piece.location);
          
#ifdef DEBUG_LEVEL_2
          vector<Piece> emptyPieces;
          for_each(begin(pieces), end(pieces), [&emptyPieces, &board] (Piece piece){
            vector<Piece> surrounding = SurroundingSquares<Piece>(&Board::GetPieceVoid, 
              piece.location, &board);
            for_each(begin(surrounding), end(surrounding), [&emptyPieces](Piece piece) {
              if(piece.curPiece == Empty)
              {
                bool alreadyFound = false;
                for_each(begin(emptyPieces), end(emptyPieces), [&piece, &alreadyFound](Piece prevPiece){
                  if(prevPiece.location == piece.location)
                    alreadyFound = true;
                });
                if(!alreadyFound)
                  emptyPieces.push_back(piece);
              }
            });
          });
#endif

          if(pieces.size() != group.size())             
          {
            boardBefore.PlayPiece(curPiece, Location(xPos, yPos));

            pieces.clear();
            DestructiveNonThreadSafeMakeLibertyGroup(boardBefore, piece); 


            throw exception("invalid group");
          }

          /*
          if(emptyPieces.size() != group.liberties)             
          {
            boardBefore.PlayPiece(curPiece, Location(xPos, yPos));
            throw exception("invalid liberty group");
          }
          */
        }
      }
    }
#endif
    
    //Double check all liberties are correct and that groups were properly made!
  }

  return SimulationResults(board);
}

//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board board, Piece piece, Location location)
{  
  Piece square = board.GetPiece(location);

  //Prevent placing where you previously got taken
  if(square.hadBlack || square.hadWhite)
    return false;

  //Prevent placing in immediate suicide  
  //Scope and make a temp copy, and then place and see if it is suicide! (bad... but for not it works)
  {
    //Eventually screen lots of these calls out... as usually they are actually not needed

    Board copy = board;
    copy.PlayPiece(piece, location);
    if(copy.GetLibertyGroup(location).liberties == 0)
    {
      //Then its suicide, and we don't place there!
      return false;
    }
  }
  
  //Prevent placing when you have TOO many friendly and no threats
  int friendly = 0;
  int enemy = 0;

  vector<Piece> surrSqrs = SurroundingSquares<Piece>(&Board::GetPieceVoid, location, &board);            
  for_each(begin(surrSqrs), end(surrSqrs), [&piece, &friendly, &enemy] (Piece& surSqr) {       
    if(surSqr.curPiece == piece.curPiece)    
      friendly++;    
    else    
      enemy++;    
  });


  friendly += 4 - surrSqrs.size(); //Off the board is good too

  if(enemy == 0 && friendly >= 3)
    return false;

  return true;
}