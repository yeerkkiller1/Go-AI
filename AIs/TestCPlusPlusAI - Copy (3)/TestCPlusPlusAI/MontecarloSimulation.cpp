#include "MontecarloSimulation.h"


#include <stdlib.h>
#include <stdio.h>



#include "FastRandom.h"

#include "q.h"

#include "Prune.h"



void ctor_SimulateResults(SimulationResults* m, Board board)
{
  COPY_ALLOWED = false;

  m->scoreInFavourOfBlack = 0;

  m->scoreInFavourOfBlack += board.whiteTakenOff;
  m->scoreInFavourOfBlack -= board.blackTakenOff;

  //We mark the squares, so we can spend time looking for the owner of a group doing work also

  #define MAX_BOARD_LOOPS 200

  int x, y;
  for(x = 0; x < BOARD_SIZE; x++)
  {
    for(y = 0; y < BOARD_SIZE; y++)      
    {
      Loc cur;
      ctor_Loc(&cur, x, y);
      if(Board_GetPiece(&board, &cur)->pieceTypeCur == Empty)
      {          
        int marked = 1;
        //Pieces originalType = board.GetPiece(cur).pieceTypeCur;

        Board_GetPiece(&board, &cur)->pieceTypeCur = Marked;

        //Spiral outwards until you hit a fill square, we are assuming that 
        //all enclosed areas are surrounded by one kind... and so that square is the owner

        int velX[4] = {1, 0, -1, 0};
        int velY[4] = {0, 1, 0, -1};

        int curTurn = 0;                    
        //(1, 0) -> (0, -1) -> (-1, 0) -> (0, 1)

        //We turn right

        //Sort of hackish... but it fixes some cases (still not great)
        int maxSearches = MAX_BOARD_LOOPS;
        int curSearchs = 0;

        while(++curSearchs < maxSearches)
        {
          ctor_Loc(&cur, cur.x + velX[curTurn], cur.y + velY[curTurn]);

          if(!Loc_IsOnBoard(&cur)) //Go back, and then turn
          {
            ctor_Loc(&cur, cur.x - velX[curTurn], cur.y - velY[curTurn]);

            curTurn = (curTurn + 1) % 4;
          }
          else if (Board_GetPiece(&board, &cur)->pieceTypeCur == Black)
          {
            m->scoreInFavourOfBlack += marked;
            break;
          }
          else if (Board_GetPiece(&board, &cur)->pieceTypeCur == White)
          {
            m->scoreInFavourOfBlack -= marked;
            break;
          }
          else //Empty or Marked 
          {              
            if(Board_GetPiece(&board, &cur)->pieceTypeCur == Empty) 
            {
              Board_GetPiece(&board, &cur)->pieceTypeCur = Marked;
              marked++;
            }
            else //Marked
            {
              //(do nothing)
            }
          }

          Loc corner;
          ctor_Loc(&corner, cur.x + velX[(curTurn + 1) % 4], cur.y + velY[(curTurn + 1) % 4]);
          if(Loc_IsOnBoard(&corner) &&
            (Board_GetPiece(&board, &cur)->pieceTypeCur == Empty))
            //Turn
          {
            curTurn = (curTurn + 1) % 4;
          }
        } //End of finding partial size of container and owner

        if(curSearchs == maxSearches)
        {            
#ifndef C
          if(m->scoreInFavourOfBlack - marked < 0 && m->scoreInFavourOfBlack + marked >= 0)
          {
            //We cannot call the game as the algorithm to find which pieces are owned by who
            //is not perfect (infact it sucks). It needs to be entirely closed, and so recursion
            //is not acceptable. A large enough manual stack may work, but something that is
            //even better should be created (a zigzag and loop following pattern perhaps).
            throw exception("due to problems with owned recognition, this game cannot be called");
          }
#endif
          m->scoreInFavourOfBlack += marked;
        }

      } //End of is empty
    } //x loop    
  } //y loop


} //function


void MonteCarloSimulate(Board* boardIn, int seed, SimulationResults* simulationResults)
{
  COPY_ALLOWED = true;
  Board board;
  ctor_Board(&board, boardIn);
  COPY_ALLOWED = false;

  init_genrand(seed);

  bool lastPassed = false;

  while(true)
  {
    int xPos = genrand_int32() % 9;
    int yPos = genrand_int32() % 9;
    //Find a position which we can place to, or pass

    int tries = 0; //We cut off after a certain amount of tries...
    //really late game this is inefficient... but for now its fine

    Loc curLoc;    

    while(Board_GetPiece(&board, ctor_Loc(&curLoc, xPos, yPos))->pieceTypeCur != Empty ||
          !SimplePrune(&board, board.curTurn, *ctor_Loc(&curLoc, xPos, yPos)))
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
#ifdef DEBUG_LEVEL_2
    Board boardBefore = board;
#endif
    Board_PlayPiece(&board, board.curTurn, *ctor_Loc(&curLoc, xPos, yPos));
    //Things have been changed so this code will not work anyway
#if 0

    #ifdef DEBUG_LEVEL_3
    Board temp2 = board;
    temp2.SetPiece(Loc(xPos, yPos), Piece(Marked));    

    cout << temp2.ToString().c_str();
    #endif

#ifdef DEBUG_LEVEL_1
    Board temp = board;
    for(int x = 0; x < BOARD_SIZE; x++)
    {
      for(int y = 0; y < BOARD_SIZE; y++)
      {
        //might as well check everyone square        
        Piece piece = temp.GetPiece(Loc(x,y));

        if(piece.IsBlack() || piece.IsWhite())
        {
          pieces.clear();
          DestructiveNonThreadSafeMakeLibertyGroup(temp, piece); 
          libertyGroup group = board.GetLibertyGroup(piece.location);
          
#ifdef DEBUG_LEVEL_2
          vector<Piece> emptyPieces;
          for_each(begin(pieces), end(pieces), [&emptyPieces, &board] (Piece piece){
            vector<Piece> surrounding = SurroundingSquares<Piece>(&Board_GetPieceVoid, 
              piece.location, &board);
            for_each(begin(surrounding), end(surrounding), [&emptyPieces](Piece piece) {
              if(piece.pieceTypeCur == Empty)
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
            boardBefore.PlayPiece(pieceTypeCur, Loc(xPos, yPos));

            pieces.clear();
            DestructiveNonThreadSafeMakeLibertyGroup(boardBefore, piece); 

#ifndef C
            throw exception("invalid group");
#endif
          }

          /*
          if(emptyPieces.size() != group.liberties)             
          {
            boardBefore.PlayPiece(pieceTypeCur, Loc(xPos, yPos));
            throw exception("invalid liberty group");
          }
          */
        }
      }
    }
#endif
#endif
    
    //Double check all liberties are correct and that groups were properly made!
  }

  COPY_ALLOWED = true;
  ctor_SimulateResults(simulationResults, board);
}