#pragma once


#include "Prune.h"

typedef struct SimulationResults SimulationResults;

struct SimulationResults
{
  int scoreInFavourOfBlack;  
};

void MonteCarloSimulate(Board* boardIn, int seed, SimulationResults* simulationResults,
                        int* pBlackPlays, int* pWhitePlays);

void ctor_SimulateResults(SimulationResults* m, Board board);
