#pragma once


#include "Prune.h"

typedef struct SimulationResults SimulationResults;

struct SimulationResults
{
  int scoreInFavourOfBlack;  
  int blackPlays;
  int whitePlays;
};

void MonteCarloSimulate
(
  Board* boardIn,
  int seed,
  SimulationResults* simulationResults,
  unsigned long* mt,
  int* pmti
);

void ctor_SimulateResults(SimulationResults* m, Board board);

#ifdef PROFILE
  int* GetProfileStream();
  int* GetProfileStreamPos();
#endif