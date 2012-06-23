#include "Piece.h"
#include "Board.h"

#include "libertyGroup.h"
#include "q.h"

#include <algorithm>

#include <stdio.h>
#include <iostream>

using namespace std;

Board::Board():whiteTakenOff(0), blackTakenOff(0), curTurn(Black)  
{ 
  //While we don't need to allocate anything, we need to hook some pointers up
  for(int y = 0; y < BOARD_SIZE; y++)
    for(int x = 0; x < BOARD_SIZE; x++)
    {
      board[x][y].location = Location(x,y);
      board[x][y].libGroup = &(board[x][y].ownedLibertyGroup);
      board[x][y].libGroup->owner = &board[x][y];
    }
}

Board::Board( const Board& other )
{
  //Don't know why this is needed?  
  whiteTakenOff = other.whiteTakenOff;
  blackTakenOff = other.blackTakenOff;
  curTurn = other.curTurn;

  Location::COPY_ALLOWED = true;
  //While we don't need to allocate anything, we need to hook some pointers up
  for(int y = 0; y < BOARD_SIZE; y++)
  {
    for(int x = 0; x < BOARD_SIZE; x++)
    {
      const Piece& otherPiece = other.board[x][y];
      Piece& curPiece = board[x][y];

      //location
      curPiece.location = Location(x, y);
      curPiece.pieceTypeCur = otherPiece.pieceTypeCur;
      curPiece.hadBlack = otherPiece.hadBlack;
      curPiece.hadWhite = otherPiece.hadWhite;
            
      //fix up all ownedLibertyGroup (as every group is owned)
      //and then hook up libGroup to the appropriate ownedLibertyGroup

      curPiece.ownedLibertyGroup.owner = &curPiece;
    }
  }

  for(int y = 0; y < BOARD_SIZE; y++)
  {
    for(int x = 0; x < BOARD_SIZE; x++)
    {
      const Piece& otherPiece = other.board[x][y];
      Piece& curPiece = board[x][y];
      Location prevOwnerPos = other.board[x][y].libGroup->owner->location;
      curPiece.ownedLibertyGroup.clear();    
      curPiece.ownedLibertyGroup.liberties.clear();      

      for(auto pieceInGroup = otherPiece.ownedLibertyGroup.begin();
        pieceInGroup != otherPiece.ownedLibertyGroup.end(); pieceInGroup++)
      {              
          curPiece.ownedLibertyGroup.insert(&board[(*pieceInGroup)->location.x][(*pieceInGroup)->location.y]);
      }

      std::for_each(begin(otherPiece.ownedLibertyGroup.liberties), end(otherPiece.ownedLibertyGroup.liberties),
        [&curPiece, this](Piece * libInGroup){
          curPiece.ownedLibertyGroup.liberties.insert(&(this->board)[libInGroup->location.x][libInGroup->location.y]);
      });
    }
  }

  for(int y = 0; y < BOARD_SIZE; y++)
  {
    for(int x = 0; x < BOARD_SIZE; x++)
    {
      Location prevOwnerPos = other.board[x][y].libGroup->owner->location;

      //libGroup      
      board[x][y].libGroup = &board[prevOwnerPos.x][prevOwnerPos.y].ownedLibertyGroup;
      //board[x][y].libGroup->owner = &board[prevOwnerPos.x][prevOwnerPos.y];      

      //Wait... this will actually be REALLY HARD! DON'T DO THIS!
    }
  }
#ifdef DEBUG_LEVEL_2
  for(int y = 0; y < BOARD_SIZE; y++)
  {
    for(int x = 0; x < BOARD_SIZE; x++)
    {      
      if(board[x][y].ToString() != other.board[x][y].ToString() ||
        board[x][y].Group() != other.board[x][y].Group() ||
        board[x][y].LibsInGroup() != other.board[x][y].LibsInGroup())
      {
        throw exception("copy of board failed!");
      }
    }
  }
#endif
    
  Location::COPY_ALLOWED = false;
}

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
  curPiece->pieceTypeCur = type;  
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

  //Fill our liberties up, and take away ourself as a liberty from surrounding squares
  vector<Piece*> pieceSurrs = SurroundingSquares<Piece>(&GetPieceVoid, location, this);
  for_each(begin(pieceSurrs), end(pieceSurrs), [&curFriendly, &curPiece] (Piece* pieceSurr) {
    if(pieceSurr->pieceTypeCur == Empty)    
      curFriendly->liberties.insert(pieceSurr);
    else
    {
      //It could have been previously removed
      //if(pieceSurr->libGroup->liberties.find(curPiece) != pieceSurr->libGroup->liberties.end())
        pieceSurr->libGroup->liberties.erase(curPiece);
    }
  });
  //If we make the group dead, we handle it later (unless it is friendly, then prune failed and we broke the rules)


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

        for(auto pieceSrc = src->begin(); pieceSrc != src->end(); pieceSrc++)
        {
          dst->insert(*pieceSrc);
          //If its not the owner
          if((*pieceSrc)->libGroup != &(*pieceSrc)->ownedLibertyGroup)          
            (*pieceSrc)->libGroup = dst;                                
        }

        for(auto pieceSrcLib = src->liberties.begin(); 
          pieceSrcLib != src->liberties.end(); pieceSrcLib++)
        {
          dst->liberties.insert(*pieceSrcLib);
        }

        //Clean up src
        src->liberties.clear();
        src->clear();
        src->owner->libGroup = dst;

        int spotForDebugging = 0;
      }
      else if (group->owner->Opposite() == type)
			{
				//If it died remove it (we already took away the liberty from it before
        if(group->liberties.size() == 0)           
          this->RemoveGroup(group);                
			}
			else
			{
				throw std::exception("group contains empty squares");
			}			
		}
	});   
}