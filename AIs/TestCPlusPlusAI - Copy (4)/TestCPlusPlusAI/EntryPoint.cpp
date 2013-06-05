


#include <time.h>


#include "q.h"

#include "MontecarloSimulation.h"


#include "FastRandom.h"

#include "Constants.h"

#include <stdio.h>



//setup a build console to use new C++ 11 stuff (not sure if it will already)
//and full optimization

//Avg Sim time (milli)    1st Result    2nd   3rd     Avg Result    Avg Black Win
//(per montecarlo     
//simulation)

//0.3
//0.109                   -18           11    24      9.469         0.715


int main ()
{  
#if 0
  Board board;

  bool black = true;
  int x = 0, y = 0;
  cin >> x;
  cin >> y;
  while(x >= 0 && y >= 0)
  {
#ifndef C
    try
    {
#endif
      Piece piece;
      piece.pieceTypeCur = black ? Black : White;    
      black = !black;

      board.PlayPiece(piece, Loc(x, y));
      cout << board.ToString().c_str() << "\n";
      cout << board.MetaToString().c_str() << "\n";
      cout << board.LibsToString().c_str() << "\n";
      cout << "\n";
      cout << "\n";

     cin >> x;
      cin >> y;
#ifndef C
    }
    catch (exception e)
    {      
      cout << "not possible as: " << e.what() << "\n";

      cin >> x;
      cin >> y;
    }
#endif
  }
#endif
  
#ifndef C
  try
  {
#endif
  Board board;
  Loc hardCodedLocation;
  ctor_Board0(&board);

  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 4, 4));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 4, 5));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 4, 6));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 5, 4));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 5, 6));
  Board_PlayPiece(&board, Black, *ctor_Loc1(&hardCodedLocation, 6, 4));
  Board_PlayPiece(&board, White, *ctor_Loc1(&hardCodedLocation, 6, 5));
  Board_PlayPiece(&board, White, *ctor_Loc1(&hardCodedLocation, 6, 6));

  //board.PlayPiece(Black, Loc(7, 4));
  //board.PlayPiece(Black, Loc(7, 6));
  //board.PlayPiece(Black, Loc(8, 5));
  //board.PlayPiece(Black, Loc(8, 6));

#ifdef C
  printf(Board_ToString(&board)); printf("\n");
  printf(Board_MetaToString(&board)); printf("\n");
  printf(Board_LibsToString(&board)); printf("\n");  
#else
  cout << Board_ToString(&board).c_str() << "\n";
  cout << Board_MetaToString(&board).c_str() << "\n";
  cout << Board_LibsToString(&board).c_str() << "\n";  
#endif  
            
  {
    int nothing;
    long t1 = clock(); long totalTime; double time;
    SimulationResults results;
    MonteCarloSimulate(&board, 20452, &results, &nothing, &nothing);
    printf( "%d black score\n", results.scoreInFavourOfBlack );  

    totalTime = (clock() - t1);
	  time = totalTime;    
    printf( "%.2f time for first\n", time );
  
    MonteCarloSimulate(&board, 3452, &results, &nothing, &nothing);
    printf( "%d black score\n", results.scoreInFavourOfBlack );  
  
    MonteCarloSimulate(&board, 3422, &results, &nothing, &nothing);
    printf( "%d black score\n", results.scoreInFavourOfBlack );  
  }
  
  {
    SimulationResults results;
    long t1 = clock(); long totalTime; double time;
    double averageResult = 0.0;

    int cSimulations = 10000;
    double blackWinPercentage = 0;
    int x;

    int blackPlays = 0;
    int whitePlays = 0;

    #ifdef DEBUG_LEVEL_1
      cSimulations -= 5000;
    #endif
    #ifdef DEBUG_LEVEL_2
      cSimulations -= 3000;
    #endif
    #ifdef DEBUG_LEVEL_3 //Diagnostic output
      cSimulations -= 1000;
    #endif

    init_genrand(4324);
    for(x = 0; x < cSimulations; x++)
    {
      MonteCarloSimulate(&board, genrand_int32(), &results, &blackPlays, &whitePlays);
      averageResult += results.scoreInFavourOfBlack;

      if(results.scoreInFavourOfBlack > 0)
        blackWinPercentage++;
    }
  

    totalTime = (clock() - t1);
	  time = (double)totalTime / (double)cSimulations;  

    printf( "%.4f average time (%.2f per second) and %g average result with %g average black win percentage %.2f avg black plays %.2f average white plays\n", 
      time, 
      (1000 / time), 
      averageResult / cSimulations, 
      blackWinPercentage / cSimulations,
      (float)blackPlays / cSimulations,
      (float)whitePlays / cSimulations);
  }
  return 0;

#ifndef C
  } catch(exception e)
  {
    cout << e.what();
  }
#endif
  
  
  return 1;
}
