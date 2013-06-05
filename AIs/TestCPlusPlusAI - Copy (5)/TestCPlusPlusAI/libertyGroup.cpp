#include "Board.h"
#include "libertyGroup.h"

#ifndef OPEN_CL
  #include <string.h>
#endif

void ctor_libertyGroup0(libertyGroup* m, libertyGroup* other)
{
  //The Pieces still need to be remapped
  *other = *m;
}

void ctor_libertyGroup1(libertyGroup* m)
{
  ctor_FastSetPiece0(&m->group);
  ctor_FastSetPiece0(&m->liberties); 
}