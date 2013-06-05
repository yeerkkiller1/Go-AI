#include "MontecarloSimulation.h"

#include "FastRandom.h"

#include "q.h"

#include "Prune.h"

void ctor_SimulateResults(SimulationResults* m, Board board)
{
  m->scoreInFavourOfBlack = 0;

  m->scoreInFavourOfBlack += board.whiteTakenOff;
  m->scoreInFavourOfBlack -= board.blackTakenOff;

  //We mark the squares, so we can spend time looking for the owner of a group doing work also

  #define MAX_BOARD_LOOPS 200

  {
    int x, y;
    for(x = 0; x < BOARD_SIZE; x++)
    {
      for(y = 0; y < BOARD_SIZE; y++)      
      {
        Loc cur;
        int velX[4] = {1, 0, -1, 0};
        int velY[4] = {0, 1, 0, -1};
        //(1, 0) -> (0, -1) -> (-1, 0) -> (0, 1)
        int curTurn = 0;

        ctor_Loc1(&cur, x, y);
        if(Board_GetPiece(&board, &cur)->pieceTypeCur == Empty)
        {          
          //Sort of hackish... but it fixes some cases (still not great)
          int maxSearches = MAX_BOARD_LOOPS;
          int curSearchs = 0;

          int marked = 1;

          int curTurn = 0;

          Board_GetPiece(&board, &cur)->pieceTypeCur = Marked;

          //Spiral outwards until you hit a fill square, we are assuming that 
          //all enclosed areas are surrounded by one kind... and so that square is the owner          
                                   
          //We turn right
          
          while(++curSearchs < maxSearches)
          {
            ctor_Loc1(&cur, cur.x + velX[curTurn], cur.y + velY[curTurn]);

            if(!Loc_IsOnBoard(&cur)) //Go back, and then turn
            {
              ctor_Loc1(&cur, cur.x - velX[curTurn], cur.y - velY[curTurn]);

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

            {
              Loc corner;
              ctor_Loc1(&corner, cur.x + velX[(curTurn + 1) % 4], cur.y + velY[(curTurn + 1) % 4]);
              if(Loc_IsOnBoard(&corner) &&
                (Board_GetPiece(&board, &cur)->pieceTypeCur == Empty))
                //Turn
              {
                curTurn = (curTurn + 1) % 4;
              }
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
  }

} //function


void MonteCarloSimulate
(
  Board* boardIn, 
  int seed, 
  SimulationResults* simulationResults,
  unsigned long* mt, 
  int* pmti
)
{
  bool lastPassed = false;
  Board board;

  ctor_Board1(&board, boardIn);

  init_genrand(seed, mt, pmti);  

  while(true)
  {
    int xPos = genrand_int32(mt, pmti) % 9;
    int yPos = genrand_int32(mt, pmti) % 9;
    //Find a position which we can place to, or pass

    int tries = 0; //We cut off after a certain amount of tries...
    //really late game this is inefficient... but for now its fine

    Loc curLoc;    

    while(Board_GetPiece(&board, ctor_Loc1(&curLoc, xPos, yPos))->pieceTypeCur != Empty ||
          !SimplePrune(&board, board.curTurn, *ctor_Loc1(&curLoc, xPos, yPos)))
    {
      xPos = genrand_int32(mt, pmti) % 9;
      yPos = genrand_int32(mt, pmti) % 9;

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
    {
    #ifdef DEBUG_LEVEL_2
        Board boardBefore = board;
    #endif
        Board_PlayPiece(&board, board.curTurn, *ctor_Loc1(&curLoc, xPos, yPos));
    }
    
    //Double check all liberties are correct and that groups were properly made!
  }

  ctor_SimulateResults(simulationResults, board);
}
