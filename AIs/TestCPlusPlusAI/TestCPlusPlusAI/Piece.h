#pragma once

class Piece; //Forward declartion to make libertyGroup happy
enum Pieces { Black, White, Empty, Marked};

#include "Location.h"
#include "libertyGroup.h"

#include <string>
#include <algorithm>
using namespace std;

//You should really not copy these, as they contain all the information on the 
//spot on the board and stuff (SHOULD REALLY BE CALLED SQUARE!)
class Piece
{
public:
	bool IsBlack();
	bool IsWhite();
	bool IsEmpty();	

  //MUST BE FILLED IN BY BOARD!
  Location location;  

  libertyGroup * libGroup;
  //This is NOT our liberties, this instead is the group we own, that way if we need one we
  //can get one right away.
  libertyGroup ownedLibertyGroup;

	Pieces Opposite();

  Piece() 
    : pieceTypeCur(Empty), hadBlack(false), hadWhite(false){}
	Piece(Pieces initial) 
    : pieceTypeCur(initial), hadBlack(false), hadWhite(false)
  {}
	~Piece(){}

  Piece( const Piece& other ) : libGroup(other.libGroup), ownedLibertyGroup(other.ownedLibertyGroup)
  {
    if(!Location::COPY_ALLOWED)
      int breakPoint = 0;
    //Hmm are you sure you want to do this? Plus I am not sure if it works
    //(usually you should not move stuff around, just use the references
    //in the original class and use delta theory to prevent coping)
  }  

	friend bool operator== (const Piece& p1, const Piece& p2);	
  friend bool operator!= (const Piece& p1, const Piece& p2);	
  friend bool operator < (const Piece& p1, const Piece& p2);  	
  friend bool operator > (const Piece& p1, const Piece& p2);

	Pieces pieceTypeCur;

  //Initially false, set when we place and not unset after we remove 
  //(so if hadBlack and pieceTypeCur == Empty, it has a black captured :D )
  bool hadBlack;
  bool hadWhite;

  std::string ToString() const
  {
    if(pieceTypeCur == Black)
      return "b";
    else if(pieceTypeCur == White)
      return "w";
    else if(pieceTypeCur == Marked)
      return "M";
    else
      return " ";
  }

  std::string Group() const
  {
    std::string curText = "";

    for(auto pieceInGroup = libGroup->begin();
      pieceInGroup != libGroup->end(); pieceInGroup++)
    {
      curText += "(";
      curText += (*pieceInGroup)->location.ToString();
      curText += ") ";
    }

    return curText;
  }

  std::string LibsInGroup() const
  {
    std::string curText = "";
    for_each(begin(libGroup->liberties), end(libGroup->liberties), [&curText] (Piece * libInGroup){
      curText += "(";
      curText += libInGroup->location.ToString();
      curText += ") ";
    });

    return curText;
  }
};