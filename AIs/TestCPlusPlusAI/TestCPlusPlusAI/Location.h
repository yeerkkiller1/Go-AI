#pragma once

//This is abstracted even though for now it will only be two ints
//in the future it will likely have (static?) helper functions for simple things
//that will be extremely optimized.
class Location
{
public :
	int x, y;

  //Will give it invalid values, and eventually will be made to crash if not initalized before use
  Location()
  {
    x = -10;
    y = -10;
  }
	Location(int x, int y) : x(x), y(y) {}

  Location( const Location& other )
  {
    //Hmm are you sure you want to do this? Plus I am not sure if it works
    //(usually you should not move stuff around, just use the references
    //in the original class and use delta theory to prevent coping)
  }

	bool IsOnBoard();

  friend bool operator== (Location &p1, Location &p2);

	friend bool operator!= (Location &p1, Location &p2); 
};