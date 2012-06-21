#include "Piece.h"
#include "Board.h"

#include "libertyGroup.h"
#include "q.h"

#include <algorithm>

using namespace std;


Piece*	Board::GetPiece		(Location location)
{
	if(!location.IsOnBoard())
		throw std::exception("placing off board");

	return &(board[location.x][location.y]);
}
//Just allows GetPiece to be passed as a function pointer to a template (as the return parameter cannot
//be templated apparently, this gets around that).
void * Board::GetPieceVoid (Location location, Board* board)
{  
	return (void *)(board->GetPiece(location));
}
void  Board::RemovePiece  (Location location)
{
  throw std::exception ("not implemented yet, as doing this is much less efficient than just removing an entire group.");
}

//Really just gets liberties from the piece
libertyGroup*	Board::GetLibertyGroup		(Location location)
{
  if(!location.IsOnBoard())
		throw std::exception("liberty group off board");

	return (board[location.x][location.y].libGroup);
}
void * Board::GetLibertyGroupVoid (Location location, Board* board)
{    
	return (void *)(board->GetLibertyGroup(location));
}


void  Board::RemoveGroup (libertyGroup* libGrpToRemove)
{
  Pieces pieceTypeCur = libGrpToRemove[0].owner->pieceTypeCur; //That way we always know what we should be removing

  for_each(begin(*libGrpToRemove), end(*libGrpToRemove), [&pieceTypeCur, this] (Piece* pieceToRemove)
  {
    if(pieceTypeCur != pieceToRemove->pieceTypeCur)
    {
      throw std::exception ("group to remove not uniform");
    }

    //Remove it, and increase all the groups which gain a liberty        
    Location locToRem = pieceToRemove->location;

    vector<libertyGroup*> libGrpSurrs = SurroundingSquares<libertyGroup>(
      &GetLibertyGroupVoid, locToRem, this);
    
    //All the surrounding groups which have been accounted for
    set<libertyGroup*> libGrpAdjAdded; //For now just keep track of starts to identify groups
    //(we will not be changing any adjacent groups so it will be fine for now)

    int libsRemoved = 0;
    
    //Adjacent liberty increase
    for_each(begin(libGrpSurrs), end(libGrpSurrs), [&libGrpAdjAdded, &pieceTypeCur, this] (libertyGroup* libGrpSurr) {
      if(libGrpSurr->size() > 0 && libGrpSurr[0].owner->pieceTypeCur != pieceTypeCur)
        //Add libs to all adjacent groups (that are not us, cause if they are they will be removed anyway!
      {
        //Not sure if this find will work as its searching for a pointer (which only create once
        //so it should be fine)
        if(libGrpAdjAdded.find(libGrpSurr) == libGrpAdjAdded.end())
        //Our lib has not been accounted for, so we add to its liberty count
        {
          libGrpAdjAdded.insert(libGrpSurr);
          libGrpSurr->liberties.insert(libGrpSurr->owner);
        }        
      }      
    }); //End going through surroundings

    //Keep track of pieces removed
    if(pieceTypeCur == Black)
      this->blackTakenOff++;
    else
      this->whiteTakenOff++;      

    //Reset the square    
    this->board[locToRem.x][locToRem.y].pieceTypeCur = Empty;

    //Reset our liberty group, unless we are the owner then we stick around as we are needed!
    if(this->board[locToRem.x][locToRem.y].libGroup != &(this->board[locToRem.x][locToRem.y].ownedLibertyGroup))
    {
      this->board[locToRem.x][locToRem.y].libGroup = &(this->board[locToRem.x][locToRem.y].ownedLibertyGroup);
      this->board[locToRem.x][locToRem.y].libGroup->clear(); //Clean it up so its accurate
      this->board[locToRem.x][locToRem.y].libGroup->liberties.clear();
    }
  });
    
  //This was explicitedly not cleared in the loop, so we clear it now as it is no longer needed
  //(and the owner is already pointing to the correct libertyGroup, its own, and it now has
  //the correct liberty information).
  libGrpToRemove->clear();
  libGrpToRemove->liberties.clear();
}


void	Board::PlayPiece	(Pieces type, Location location)
{
	if(type == Empty)
		throw std::exception("placing empty piece");

	if(GetPiece(location)->pieceTypeCur != Empty)
		throw std::exception("placing over piece");

  //Redundant
	if(!location.IsOnBoard())
		throw std::exception("placing off board");
	

  //Play actual piece
  Piece* curPiece = GetPiece(location);
  curPiece->pieceTypeCur = curTurn;  
  curPiece->hadBlack |= curPiece->IsBlack(); //We set these on place
  curPiece->hadWhite |= curPiece->IsWhite();

  //Advance turn
  if(curTurn != type)  
    int why = 0; //?why, it may be needed but you are likely just doing something wrong  
  else    
    curTurn = type == Black ? White : Black;  


  //May flip-flip around a lot when combining, as when combining
  //we set this to the largest, and then add the other
  if(curPiece->libGroup != &curPiece->ownedLibertyGroup)
  {
    throw exception ("empty piece should have its libGroup as its ownedLibertyGroup");
  }

  //Make cur liberty group, and fill it with our liberties
	libertyGroup* curFriendly = curPiece->libGroup;
  curFriendly->insert(curPiece);

  vector<Piece*> pieceSurrs = SurroundingSquares<Piece>(&GetPieceVoid, location, this);
  for_each(begin(pieceSurrs), end(pieceSurrs), [&curFriendly] (Piece* pieceSurr) {
    if(pieceSurr->pieceTypeCur == Empty)    
      curFriendly->liberties.insert(pieceSurr);
  });


	vector<libertyGroup*> libGrpSurrs = SurroundingSquares<libertyGroup>(&GetLibertyGroupVoid, location, this);


  //Merge all friendly groups into 1 big group (keeping liberties),
  //and subtracting from enemy liberties
	for_each(begin(libGrpSurrs), end(libGrpSurrs), [&curFriendly, &curPiece, type, this] (libertyGroup* group) {
		//What does this even do?
		//Remove this operator overload and use better functions!
		if(group->size() > 0)
		{      
			//Friendly
			if(group->owner->pieceTypeCur == type)	       
			{		
        libertyGroup * src;
        libertyGroup * dst;
        
        //group into curFriendly
        if(group->size() < curFriendly->size())
        {
          src = group;
          dst = curFriendly;
        }
        else //curFriendly into group
        {
          src = group;
          dst = curFriendly;
        }

        //Go through all of src, first settings all pieces so they instead point
        //to the dst (except for the owner, he is handled last), adding the pieces 
        //to the dst and then afterwards, adding the liberties        

        for_each(begin(*src), end(*src), [&dst] (Piece * pieceSrc)
        {          
          dst->insert(pieceSrc);
          //If its not the owner
          if(pieceSrc->libGroup != &pieceSrc->ownedLibertyGroup)          
            pieceSrc->libGroup = dst;            
          
          dst->insert(pieceSrc);
        });

        for_each(begin(src->liberties), end(src->liberties), [&dst] (Piece * pieceSrcLib)
        {
          //Make sure this actually screens duplicates (it should because its a set... but maybe not)
          dst->liberties.insert(pieceSrcLib);
        });

        //Clean up src
        src->liberties.clear();
        src->clear();
        src->owner->libGroup = dst;
			}
			else if (group->owner->Opposite() == type)
			{
				//If it dies remove it, else just take off one liberty        
        if(group->liberties.size() == 1)           
          this->RemoveGroup(group);        
        else
        {
          //This is all you need to do to replace a liberty
          group->liberties.erase(group->liberties.find(curPiece));
        }
			}
			else
			{
				throw std::exception("group contains empty squares");
			}			
		}
	});   
}