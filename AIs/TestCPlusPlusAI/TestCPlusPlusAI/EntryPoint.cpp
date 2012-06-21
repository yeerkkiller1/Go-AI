#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <cstdlib>

#include <vector>

#include <chrono>
//#include <string>
#include <memory>
#include <iostream>

#include <boost/utility.hpp>



#include "q.h"
#include "Metadata.h"

#include "MontecarloSimulation.h"

//#include "FastRandom.h"

using namespace std;
using namespace chrono;

//setup a build console to use new C++ 11 stuff (not sure if it will already)
//and full optimization

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
    try
    {
      Piece piece;
      piece.curPiece = black ? Black : White;    
      black = !black;

      board.PlayPiece(piece, Location(x, y));
      cout << board.ToString().c_str() << endl;
      cout << board.MetaToString().c_str() << endl;
      cout << board.LibsToString().c_str() << endl;
      cout << endl;
      cout << endl;

     cin >> x;
      cin >> y;
    }
    catch (exception e)
    {      
      cout << "not possible as: " << e.what() << endl;

      cin >> x;
      cin >> y;
    }
  }
#endif

  try
  {
  Board board;

  board.PlayPiece(Piece(Black), Location(4, 4));
  board.PlayPiece(Piece(Black), Location(4, 5));
  board.PlayPiece(Piece(Black), Location(4, 6));
  board.PlayPiece(Piece(Black), Location(5, 4));
  board.PlayPiece(Piece(Black), Location(5, 6));
  board.PlayPiece(Piece(Black), Location(6, 4));
  board.PlayPiece(Piece(White), Location(6, 5));
  board.PlayPiece(Piece(White), Location(6, 6));

  cout << board.ToString().c_str() << endl;
  cout << board.MetaToString().c_str() << endl;
  cout << board.LibsToString().c_str() << endl;  

  auto t1 = high_resolution_clock::now(); 
    
  SimulationResults results = MonteCarloSimulate(board, 20452);
  cout << results.scoreInFavourOfBlack << endl;

  auto totalTime = (high_resolution_clock::now() - t1);
	double time = totalTime.count() / 10000.0;  
  cout << time << endl;

  results = MonteCarloSimulate(board, 3452);
  cout << results.scoreInFavourOfBlack << endl;

  results = MonteCarloSimulate(board, 3422);
  cout << results.scoreInFavourOfBlack << endl;
    
  totalTime = (high_resolution_clock::now() - t1);
	time = totalTime.count() / 10000.0;  
  cout << time;

  return 0;
  
  }
  catch (exception e)
  {
    cout << e.what();
  }
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