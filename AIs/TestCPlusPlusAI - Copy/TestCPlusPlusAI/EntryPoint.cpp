#include <stdio.h>
#include <stdlib.h>



#include "time.h"

#include <string.h>

#include "q.h"
#include "Metadata.h"

#include "MontecarloSimulation.h"

#include "Location.h"

#include "FastRandom.h"

#include "Constants.h"

#include <stdio.h>

//#include "FastRandom.h"



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
  ctor_Board(&board);

  Board_PlayPiece(&board, Black, *ctor_Loc(&hardCodedLocation, 4, 4));
  Board_PlayPiece(&board, Black, *ctor_Loc(&hardCodedLocation, 4, 5));
  Board_PlayPiece(&board, Black, *ctor_Loc(&hardCodedLocation, 4, 6));
  Board_PlayPiece(&board, Black, *ctor_Loc(&hardCodedLocation, 5, 4));
  Board_PlayPiece(&board, Black, *ctor_Loc(&hardCodedLocation, 5, 6));
  Board_PlayPiece(&board, Black, *ctor_Loc(&hardCodedLocation, 6, 4));
  Board_PlayPiece(&board, White, *ctor_Loc(&hardCodedLocation, 6, 5));
  Board_PlayPiece(&board, White, *ctor_Loc(&hardCodedLocation, 6, 6));

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

  long t1 = clock();
        
  SimulationResults results;
  MonteCarloSimulate(&board, 20452, &results);
  printf( "%g\n", results.scoreInFavourOfBlack );  

  long totalTime = (clock() - t1);
	double time = totalTime;    
  printf( "%.2f", time ); printf( " time for first" ); printf( "\n");
  
  MonteCarloSimulate(&board, 3452, &results);
  printf( "%g\n", results.scoreInFavourOfBlack );  
  
  MonteCarloSimulate(&board, 3422, &results);
  printf( "%g\n", results.scoreInFavourOfBlack );  
  

  t1 = clock();
  double averageResult = 0.0;
  int cSimulations = 1000;
  double blackWinPercentage = 0;
  int x;
  for(x = 0; x < cSimulations; x++)
  {
    MonteCarloSimulate(&board, genrand_int32(), &results);
    averageResult += results.scoreInFavourOfBlack;

    if(results.scoreInFavourOfBlack > 0)
      blackWinPercentage++;
  }

  averageResult /= cSimulations;
  blackWinPercentage /= cSimulations;

  totalTime = (clock() - t1);
	time = (double)totalTime / (double)cSimulations;  

  printf( "%.2f", time );printf( " average time and "  ); 
  
  printf( "%g", averageResult );  
  
  printf( " average result with " );   
  printf( "%g", blackWinPercentage ); 
  printf( " average black win percentage"); printf( "\n");

  return 0;

#ifndef C
  } catch(exception e)
  {
    cout << e.what();
  }
#endif
  
  
  return 1;
	/*
	Piece temp(0);
	for(int x = 0; x < total; x++)
	{
	swap(test[x], test[total - x - 1]);        
	//auto p = make_shared<int>(42);
	}
	*/
}