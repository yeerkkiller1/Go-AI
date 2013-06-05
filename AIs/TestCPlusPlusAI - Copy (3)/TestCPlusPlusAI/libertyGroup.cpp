#include "Board.h"
#include "libertyGroup.h"

#include <string.h>

void ctor_libertyGroup(libertyGroup* m, libertyGroup* other)
{
  if(!COPY_ALLOWED)
  {
    int breakHere = 0;
    //Only do this copy if you are going to remap the liberties, as usually 
    //if you are copying to a new board it won't work properly without remapping everything
  }

  //The Pieces still need to be remapped
  memcpy(other, m, sizeof(libertyGroup));
}

void ctor_libertyGroup(libertyGroup* m)
{
  ctor_FastSetPiece(&m->group);
  ctor_FastSetPiece(&m->liberties); 
}