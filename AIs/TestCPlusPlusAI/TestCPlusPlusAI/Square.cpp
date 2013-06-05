#include "Square.h"

void ctor_Square0(Square* m, Board* containingBoard, int x, int y)
{
  m->pieceTypeCur = Empty;
  m->hadBlack = false;
  m->hadWhite = false;

  m->board = containingBoard;

  ctor_FastSetPiece0(m->group, containingBoard);
  ctor_FastSetPiece0(m->liberties, containingBoard);    

  ctor_Loc1(&m->location, x, y);

  m->compressedSetLocation = Loc_CompressPosition(x, y);
}

void ctor_Square1(Square* obj, Board* containingBoard, Pieces initial, int x, int y)
{
  ctor_Square0(obj, containingBoard, x, y);
  obj->pieceTypeCur = initial;    
}

void ctor_SquareCopy(Square* obj, Square* other)
{
  //Copy the memory, this should work?
  *obj = *other;

    //Hmm are you sure you want to do this? Plus I am not sure if it works
    //(usually you should not move stuff around, just use the references
    //in the original class and use delta theory to prevent coping)  
}

bool	Square_IsBlack(Square* m)
{
	return m->pieceTypeCur == Black;
}
bool	Square_IsWhite(Square* m)
{
	return m->pieceTypeCur == White;
}
bool	Square_IsEmpty(Square* m)
{
	return m->pieceTypeCur == Empty;
}

Pieces	Square_Opposite(Square* m)
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

bool Square_Equ(Square* p1,  Square* p2)  { return Loc_Equ(&p1->location, &p2->location); }
bool Square_NotEqu(Square* p1,  Square* p2)  { return Loc_NotEqu(&p1->location, &p2->location); }
bool Square_LessThan(Square* p1,  Square* p2)	  { return (p1->location.y < p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x < p2->location.x); }
bool Square_GreaterThan(Square* p1,  Square* p2)	  { return (p1->location.y > p2->location.y) || 
                                        (p1->location.y == p2->location.y && p1->location.x > p2->location.x); }

#include "IntStream.h"
void Square_ToString(Square* m, 
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
                    int* pCharPos) 
{
  if(m->pieceTypeCur == Black)
    WriteCharToIntStream(intStream, pCharPos, 'B');
  else if(m->pieceTypeCur == White)
    WriteCharToIntStream(intStream, pCharPos, 'W');
  else if(m->pieceTypeCur == Marked)
    WriteCharToIntStream(intStream, pCharPos, 'M');
  else if(m->hadBlack)
    WriteCharToIntStream(intStream, pCharPos, 'b');
  else if(m->hadWhite)
    WriteCharToIntStream(intStream, pCharPos, 'w');
  else
    WriteCharToIntStream(intStream, pCharPos, ' ');
}

void Square_Group(Square* m, __global int* intStream, int* pCharPos) 
{    
  unsigned char* libGrp = m->libGrp->group;
  int z;
  for(z = 0; z < FastSetPiece_length(libGrp); z++)
  { 
    WriteCharToIntStream(intStream, pCharPos, '(');
    Loc_ToString(&FastSetPiece_getAt(m->board, libGrp, z)->location, intStream, pCharPos);
    WriteCharToIntStream(intStream, pCharPos, ')');
    WriteCharToIntStream(intStream, pCharPos, ' ');
  }
}

void Square_LibsInGroup(Square* m, __global int* intStream, int* pCharPos) 
{
  unsigned char* libs = m->libGrp->liberties;
  int z;
  for(z = 0; z < FastSetPiece_length(libs); z++)
  {
    WriteCharToIntStream(intStream, pCharPos, '(');
    Loc_ToString(&FastSetPiece_getAt(m->board, libs, z)->location, intStream, pCharPos);
    WriteCharToIntStream(intStream, pCharPos, ')');
    WriteCharToIntStream(intStream, pCharPos, ' ');
  }
}
