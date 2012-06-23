#include "Metadata.h"

#include "Prune.h"

#include <stdlib.h>
#include <algorithm>

class SimulationResults
{
public:
  int scoreInFavourOfBlack;

  SimulationResults(Board& board);  
}; //class

SimulationResults MonteCarloSimulate(Board initial, int seed);