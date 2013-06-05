#include "Piece.h"

void ctor_Piece0(Piece* obj)
{
  obj->pieceTypeCur = Empty;
  obj->hadBlack = false;
  obj->hadWhite = false;

  ctor_libertyGroup1(&obj->ownedLibertyGroup);
  obj->libGroup = &obj->ownedLibertyGroup;
}

void ctor_Piece1(Piece* obj, Pieces initial)
{
  ctor_Piece0(obj);
  obj->pieceTypeCur = initial;  
}

void ctor_PieceCopy(Piece* obj, Piece* other)
{
  //Copy the memory, this should work?
  *obj = *other;

    //Hmm are you sure you want to do this? Plus I am not sure if it works
    //(usually you should not move stuff around, just use the references
    //in the original class and use delta theory to prevent coping)  
}

bool	Piece_IsBlack(Piece* m)
{
	return m->pieceTypeCur == Black;
}
bool	Piece_IsWhite(Piece* m)
{
	return m->pieceTypeCur == White;
}
bool	Piece_IsEmpty(Piece* m)
{
	return m->pieceTypeCur == Empty;
}

Pieces	Piece_Opposite(Piece* m)
{
	if(m->pieceTypeCur == Black)
		return White;
	else if(m->pieceTypeCur == White) 
		return Black;
	else 
  {
#ifndef C
		throw std::exception("Called opposite of an empty square");
#endif
    return Empty;
  }
}

bool Piece_Equ(Piece* p1,  Piece* p2)  { return Loc_Equ(&p1->location, &p2->location); }
bool Piece_NotEqu(Piece* p1,  Piece* p2)  { return Loc_NotEqu(&p1->location, &p2->location); }
bool Piece_LessThan(Piece* p1,  Piece* p2)	  { return (p1->location.y < p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x < p2->location.x); }
bool Piece_GreaterThan(Piece* p1,  Piece* p2)	  { return (p1->location.y > p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x > p2->location.x); }

#include "IntStream.h"
void Piece_ToString(Piece* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                    int* pCharPos) 
{
  if(m->pieceTypeCur == Black)
    WriteCharToIntStream(intStream, pCharPos, 'b');
  else if(m->pieceTypeCur == White)
    WriteCharToIntStream(intStream, pCharPos, 'w');
  else if(m->pieceTypeCur == Marked)
    WriteCharToIntStream(intStream, pCharPos, 'M');
  else
    WriteCharToIntStream(intStream, pCharPos, ' ');
}

void Piece_Group(Piece* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                 int* pCharPos) 
{    
  Piece** pieceInGroup;
  for(pieceInGroup = FastSetPiece_begin(&m->libGroup->group);
    pieceInGroup != FastSetPiece_end(&m->libGroup->group); pieceInGroup++)
  { 
    WriteCharToIntStream(intStream, pCharPos, '(');
    Loc_ToString(&(*pieceInGroup)->location, intStream, pCharPos);
    WriteCharToIntStream(intStream, pCharPos, ')');
    WriteCharToIntStream(intStream, pCharPos, ' ');
  }
}

void Piece_LibsInGroup(Piece* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                       int* pCharPos) 
{
  Piece** libInGroup;
  for(libInGroup = FastSetPiece_begin(&m->libGroup->liberties);
    libInGroup != FastSetPiece_end(&m->libGroup->liberties); libInGroup++)
  {
    WriteCharToIntStream(intStream, pCharPos, '(');
    Loc_ToString(&(*libInGroup)->location, intStream, pCharPos);
    WriteCharToIntStream(intStream, pCharPos, ')');
    WriteCharToIntStream(intStream, pCharPos, ' ');
  }
}