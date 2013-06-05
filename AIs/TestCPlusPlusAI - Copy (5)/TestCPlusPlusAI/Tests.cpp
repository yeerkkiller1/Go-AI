#include "Tests.h"


#include "FastRandom.h"
#include "Board.h"
#include "MontecarloSimulation.h"

#include "IntStream.h"

void SetupBoard
(
  Board* pBoard
)
{
  Loc hardCodedLocation;
  ctor_Board0(pBoard);
  
  Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 4));
  //Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 5));
  //Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 4, 6));
  //Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 5, 4));
  //Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 5, 6));
  //Board_PlayPiece(pBoard, Black, *ctor_Loc1(&hardCodedLocation, 6, 4));
  //Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 6, 5));
  //Board_PlayPiece(pBoard, White, *ctor_Loc1(&hardCodedLocation, 6, 6));
}

void RunBasicSimulations
(
#ifdef OPEN_CL
__global int* intStream,
#else
int* intStream,
#endif  
  int* pCharPos
)
{
  int streamPos = 0;

  //These are needed for FastRandom function calls
  unsigned long mt[N];
  int mti=N+1;

  
  Board board;
  SetupBoard(&board);
  //
  //
  //Board_ToString(&board, intStream, pCharPos);
  //WriteCharToIntStream(intStream, pCharPos, '\n');
  //
  //Board_MetaToString(&board, intStream, pCharPos);
  //WriteCharToIntStream(intStream, pCharPos, '\n');
  //
  //Board_LibsToString(&board, intStream, pCharPos);
  //WriteCharToIntStream(intStream, pCharPos, '\n');
  

  {
    /*
    SimulationResults results;
    MonteCarloSimulate(&board, 20452, &results, mt, &mti);
    WriteFloatToIntStream(intStream, pCharPos, (float)results.scoreInFavourOfBlack, 0);
    WriteStringToIntStream(intStream, pCharPos, " black score\n");

    MonteCarloSimulate(&board, 3452, &results, mt, &mti);
    WriteFloatToIntStream(intStream, pCharPos, (float)results.scoreInFavourOfBlack, 0);
    WriteStringToIntStream(intStream, pCharPos, " black score\n");
  
    MonteCarloSimulate(&board, 3422, &results, mt, &mti);
    WriteFloatToIntStream(intStream, pCharPos, (float)results.scoreInFavourOfBlack, 0);
    WriteStringToIntStream(intStream, pCharPos, " black score\n");
    */
  }
  
}

void RunSpeedSimulation
(
#ifdef OPEN_CL
__global int* intStream,
#else
int* intStream,
#endif  
  int* pCharPos,
  int cSimulations
)
{
  //These are needed for FastRandom function calls
  unsigned long mt[N];
  int mti=N+1;

  SimulationResults results;
  Board board;
      
  double averageResult = 0.0;

  double blackWinPercentage = 0;
  int x;


  SetupBoard(&board);

  for(x = 0; x < cSimulations; x++)
  {
    MonteCarloSimulate(&board, genrand_int32(mt, &mti), &results, mt, &mti);
    averageResult += results.scoreInFavourOfBlack;

    if(results.scoreInFavourOfBlack > 0)
      blackWinPercentage++;
  }
  
  averageResult /= cSimulations;
  blackWinPercentage /= cSimulations;


  WriteFloatToIntStream(intStream, pCharPos, (float)averageResult, 5);
  WriteStringToIntStream(intStream, pCharPos, " average result with ");
  WriteFloatToIntStream(intStream, pCharPos, (float)blackWinPercentage, 5);
  WriteStringToIntStream(intStream, pCharPos, " average black win percentage\n");
}