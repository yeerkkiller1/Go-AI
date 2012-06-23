#include "Piece.h"

bool	Piece::IsBlack()
{
	return pieceTypeCur == Black;
}
bool	Piece::IsWhite()
{
	return pieceTypeCur == White;
}
bool	Piece::IsEmpty()
{
	return pieceTypeCur == Empty;
}

Pieces	Piece::Opposite()
{
	if(pieceTypeCur == Black)
		return White;
	else 
	if(pieceTypeCur == White) 
		return Black;
	else 
		throw std::exception("Called opposite of an empty square");
}

bool operator== (const Piece& p1, const Piece& p2)  { return p1.location == p2.location; }
bool operator!= (const Piece& p1, const Piece& p2)  { return p1.location != p2.location; }
bool operator< (const Piece& p1, const Piece& p2)	  { return (p1.location.y < p2.location.y) || 
                                        (p1.location.y == p2.location.y && p1.location.x < p2.location.x); }
bool operator> (const Piece& p1, const Piece& p2)	  { return (p1.location.y > p2.location.y) || 
                                        (p1.location.y == p2.location.y && p1.location.x > p2.location.x); }