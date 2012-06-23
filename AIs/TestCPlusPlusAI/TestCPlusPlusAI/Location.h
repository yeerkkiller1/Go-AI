#pragma once

#include <string>
#include <algorithm>

//This is abstracted even though for now it will only be two ints
//in the future it will likely have (static?) helper functions for simple things
//that will be extremely optimized.
class Location
{
public:
  //This is the home for this until the include system is fixed!
  static bool COPY_ALLOWED;

public :
	int x, y;

  //Will give it invalid values, and eventually will be made to crash if not initalized before use
  Location()
  {
    x = -10;
    y = -10;
  }
	Location(int x, int y) : x(x), y(y) {}

  Location( const Location& other ) : x(other.x), y(other.y)
  {
    int breakPoint = 0;
    //Hmm are you sure you want to do this? Plus I am not sure if it works
    //(usually you should not move stuff around, just use the references
    //in the original class and use delta theory to prevent coping)
  }

	bool IsOnBoard();

  friend bool operator== (Location &p1, Location &p2);

	friend bool operator!= (Location &p1, Location &p2); 

  std::string ToString() const
  {
    std::string toReturn = "";    
    toReturn += (char)(x + 48);
    toReturn += ", ";
    toReturn += (char)(y + 48);

    return toReturn;
  }
};