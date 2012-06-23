#include "Board.h"
#include "libertyGroup.h"

libertyGroup::libertyGroup( const libertyGroup& other ) : owner(other.owner)
{
  if(!Location::COPY_ALLOWED)
    int breakPoint = 0;
  //Hmm are you sure you want to do this? Plus I am not sure if it works
  //(usually you should not move stuff around, just use the references
  //in the original class and use delta theory to prevent coping)
}