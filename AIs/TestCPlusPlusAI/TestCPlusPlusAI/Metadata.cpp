#include "Metadata.h"
#include "q.h"

//#include <stdlib.h>
#include <algorithm>

//using namespace std;

//PIECE
bool	Piece::IsBlack()
{
	return curPiece == Black;
}
bool	Piece::IsWhite()
{
	return curPiece == White;
}
bool	Piece::IsEmpty()
{
	return curPiece == Empty;
}

Pieces	Piece::Opposite()
{
	if(curPiece == Black)
		return White;
	else 
	if(curPiece == White) 
		return Black;
	else 
		throw exception("Called opposite of an empty square");
}

bool operator== (Piece p1, Piece p2)  { return p1.location == p2.location; }
bool operator!= (Piece p1, Piece p2)	{ return p1.location != p2.location; }

//LOCATION
bool	Location::IsOnBoard	()
{
	return (x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0);
}

bool operator== (Location &p1, Location &p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

bool operator!= (Location &p1, Location &p2)
{
  return p1.x != p2.x || p1.y != p2.y;
}

//BOARD
Piece	Board::GetPiece		(Location location)
{
	if(!location.IsOnBoard())
		throw exception("placing off board");

	return board[location.x][location.y];
}
Piece tempPiece;
void * Board::GetPieceVoid (Location location, Board board)
{
  tempPiece = board.GetPiece(location);
	return (void *)(&tempPiece);
}

void	Board::SetPiece		(Location location, Piece piece)
{
	if(!location.IsOnBoard())
		throw exception("placing off board");

  piece.location = location;
	board[location.x][location.y] = piece;

  if(piece.curPiece == Black)  
    board[location.x][location.y].hadBlack = true;
  else if(piece.curPiece == White)  
    board[location.x][location.y].hadWhite = true;
}

void  Board::RemovePiece  (Location location)
{
  throw exception ("not implemented yet, as doing this is much less efficient than just removing an entire group.");
}

void  Board::RemoveGroup (libertyGroup groupToRemove)
{
  Board* m_this = this;

  Pieces curPiece = groupToRemove[0].curPiece;

  for_each(begin(groupToRemove), end(groupToRemove), [m_this, &curPiece] (Piece pieceToRemove)
  {
    if(curPiece != pieceToRemove.curPiece)
    {
      throw exception ("group removing not uniform");
    }

    //Remove it, and increase all the groups which gain a liberty        
    Location location = pieceToRemove.location;

    vector<libertyGroup> surrGroups = SurroundingSquares<libertyGroup>(
      &GetLibertyGroupVoid, location, m_this);
    
    Piece ourPiece = m_this->GetPiece(pieceToRemove.location);  

    //All the surrounding groups which have been accounted for
    vector<libertyGroup> surrAdded; //For now just keep track of starts to identify groups
    //(we will not be changing any adjacent groups so it will be fine for now)

    int libsRemoved = 0;

    //Adjacent liberty increase
    for_each(begin(surrGroups), end(surrGroups), [&surrAdded, m_this, &ourPiece, &libsRemoved] (libertyGroup& group) {              
      if(group.size() > 0 && group[0].curPiece != ourPiece.curPiece)
      {        
        bool contains = false;
        for_each(begin(surrAdded), end(surrAdded), [&group, &contains](libertyGroup surrGroup){
          if(surrGroup[0].location == group[0].location)
          {
            contains = true; //Should really break after this          
          }
        });

        //Our lib has not been accounted for, so we add to its liberty count
        if(!contains)
        {
          surrAdded.push_back(group);        
        
          m_this->ChangeLiberties(group, 1);
        }        
      }      
    }); //End going through surroundings

    //Keep track of pieces removed
    if(ourPiece.curPiece == Black)
      m_this->blackTakenOff++;
    else
      m_this->whiteTakenOff++;
  
    //Reset the square
    m_this->board[location.x][location.y].curPiece = Empty;
    m_this->libertyBoard[location.x][location.y].clear();
    m_this->libertyBoard[location.x][location.y].liberties = 0;    
  });
}

void  Board::ChangeLiberties (libertyGroup group, int delta)
{
  Board* m_this = this;
  for_each(begin(group), end(group), [&group, m_this, delta] (Piece& piece) {
    m_this->libertyBoard[piece.location.x][piece.location.y].liberties += delta;
  });
}

libertyGroup	Board::GetLibertyGroup		(Location location)
{
  if(!location.IsOnBoard())
		throw exception("liberty group off board");

  libertyGroup tempTest = libertyBoard[location.x][location.y];
  if(tempTest.size() > 0)
  {
    Piece first = tempTest[0];
  }

	return libertyBoard[location.x][location.y];
}
libertyGroup temp;
void * Board::GetLibertyGroupVoid (Location location, Board board)
{
  //This will not work, as it gets destroyed because you are killing it...
  //wtf are you smoking!
  temp = board.GetLibertyGroup(location);
	return (void *)(&temp);
}

void	Board::PlayPiece	(Piece piece, Location location)
{
	if(piece.curPiece == Empty)
		throw exception("placing empty piece");

	if(GetPiece(location).curPiece != Empty)
		throw exception("placing over piece");

  //Redundant
	if(!location.IsOnBoard())
		throw exception("placing off board");
	
	//Recalculate liberties before placing piece

	//libertyGroup curLibGroup = libertyBoard[location.x][location.y];

  piece.location = location;
  if(curTurn != piece.curPiece)
  {
    int why = 0; //?why, it may be needed but you are likely just doing something wrong
  }
  else
  {
    //Advance the turn
    curTurn = piece.curPiece == Black ? White : Black;
  }
  

	vector<libertyGroup> groups = SurroundingSquares<libertyGroup>(&GetLibertyGroupVoid, location, this);

	libertyGroup friendly; //vectors of size 0

  Board* m_this = this;

  libertyGroup originalPlacement;
  originalPlacement.push_back(piece);
  int liberties = 0;

  vector<Piece> surrounding = SurroundingSquares<Piece>(&GetPieceVoid, piece.location, m_this);
  for_each(begin(surrounding), end(surrounding), [&originalPlacement] (Piece& piece) {
    if(piece.curPiece == Empty)    
      originalPlacement.liberties++;
  });

  board[location.x][location.y] = piece;
  board[location.x][location.y].hadBlack |= piece.IsBlack();
  board[location.x][location.y].hadWhite |= piece.IsWhite();


  friendly = originalPlacement;
  
  //Merge all friendly groups into 1 big group (keeping liberty count),
  //and subtract from enemy liberties
	for_each(begin(groups), end(groups), [&piece, &friendly, &m_this] (libertyGroup& group) {
		//What does this even do?
		//Remove this operator overload and use better functions!
		if(group.size() > 0)
		{
      Piece curFirstPiece = group[0];

      int test = _ITERATOR_DEBUG_LEVEL;

			//Friendly
			if(group[0].curPiece == piece.curPiece)	       
			{			
        bool alreadyFound = false;
        //If the first has been merged, it is likely the whole group has already been merged
        for_each(begin(friendly), end(friendly), [&group, &alreadyFound] (Piece& piece) {
          if(group[0].location == piece.location)
            alreadyFound = true;
        });

        if(!alreadyFound)
        {
				  //Add to friendly, while maintaining its correct liberty count
          //friendly.liberties--; //When we border a friendly, it loses one liberty
          //Actually not, it will just not get accounted for when we check over the libs again
				  if(friendly.size() == 0)
				  {
					  //Liberties do not need to be recalculated, just move it
					  friendly = group;          
				  }
				  else
				  {				
					  vector<Piece> originalEmptySquares; 
					  //Dynamically generated as usually we should not reach here (hopefully)
          
            //Fill originalEmptySquares
            for_each(begin(friendly), end(friendly), [&originalEmptySquares, m_this] (Piece& piece) {
              vector<Piece> surrounding = SurroundingSquares<Piece>(&GetPieceVoid, piece.location, m_this);
              for_each(begin(surrounding), end(surrounding), [&originalEmptySquares] (Piece& piece) {
                if(piece.curPiece == Empty)
                  originalEmptySquares.push_back(piece);
              });
            });

            //Go through group empty squares, add 1 liberty for all new ones found      
            auto libOrgEnd = end(group);
            for(auto libToAdd = begin(group); libToAdd != libOrgEnd; libToAdd++)
            {
              vector<Piece> newLibs = SurroundingSquares<Piece>(&GetPieceVoid, libToAdd->location, m_this);            
              for_each(begin(newLibs), end(newLibs), [&originalEmptySquares, &friendly] (Piece& newLib) {              
                if(newLib.curPiece == Empty)
                {
                  bool contains = false;
                  for(auto origEmpty = begin(originalEmptySquares); origEmpty != end(originalEmptySquares); origEmpty++)
                  {
                    if(origEmpty->location == newLib.location)
                    {
                      contains = true;
                      break;
                    }
                  }

                  //Doesn't contain it, so we add to liberty count
                  if(!contains)
                  {
                    friendly.liberties++;
                    originalEmptySquares.push_back(newLib);
                  }
                }
              });
            }

            //Combine friendly and group
            auto origEnd = end(group);
            for(auto groupToAdd = begin(group); groupToAdd != origEnd; groupToAdd++)
            {
              friendly.push_back(*groupToAdd);
              if(groupToAdd[0].curPiece != piece.curPiece)
              {
                throw exception("friendly group not all friendly");
              }
				    }
          }
        }
			}
			else if (group[0].curPiece == piece.Opposite())
			{
				//If it dies remove it, else just take off one liberty        
        if(group.liberties == 1)           
          m_this->RemoveGroup(group);        
        else
          m_this->ChangeLiberties(group, -1);        
			}
			else
			{
				throw exception("group contains empty squares");
			}			
		}
	});
  
  //An unfortunate update that is needed
  for(auto friendPiece = begin(friendly); friendPiece != end(friendly); friendPiece++)
  {
    libertyBoard[(*friendPiece).location.x][(*friendPiece).location.y] = friendly;

    if(friendPiece->curPiece != piece.curPiece)
    {
      throw exception("friendly piece not friendly");
    }
	}    
}

/*
//All the private functions and members
class Metadata::impl
{
public:
  impl()
  {
  }

private:

};

//Defines how we create/init Metadata (not much will go on there)
//and then passes stuff onto impl (basically everything will be there).
Metadata::Metadata() : pimpl( new impl () ) { }
Metadata::~Metadata() { }

*/