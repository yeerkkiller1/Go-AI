#pragma once

#include "Metadata.h"

#include "Prune.h"

#include <stdlib.h>


typedef struct SimulationResults SimulationResults;

struct SimulationResults
{
  int scoreInFavourOfBlack;  
};

void MonteCarloSimulate(Board* initial, int seed, SimulationResults* simResults);

void ctor_SimulateResults(SimulationResults* m, Board board);
