#include "Metadata.h"

#include <stdlib.h>
#include <algorithm>

class SimulationResults
{
public:
  int scoreInFavourOfBlack;

  SimulationResults(Board board);  
}; //class

SimulationResults MonteCarloSimulate(Board initial, int seed);

//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board board, Piece piece, Location location);

//Just for testing, add to pieces, clear piece before you call
void DestructiveNonThreadSafeMakeLibertyGroup(Board& board, Piece curPiece);