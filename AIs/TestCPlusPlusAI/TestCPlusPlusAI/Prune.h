#include "Board.h"

#include <vector>

//False means don't place
//Board is copied, and so we change it...
//in the future when we want to not copy it... the algorithm will need to be more complicated
//to remove the need for changing it (complicated stuff needs to be done to see if we can place
bool SimplePrune(Board& board, Pieces type, Location location);

inline bool IsSuicide(Board& board, Pieces type, Location location, int friendly, int enemy, int empty, vector<Piece*> pieceAdjs, Piece* pieceCur);