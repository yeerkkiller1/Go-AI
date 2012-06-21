using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace C_Sharp_Preprocessor
{
    class Program
    {
        static void Main(string[] args)
        {
        }
    }
}


//Regular call structure:
//GetAndPlayMove
//  GetBest
//      (Branch back to GetBest)
//          MonteCarloForBlackMetadata
//              SimulateRandomGameWithMetadata
//              CreateLibertyGroups
//                  SimMetadataRecurse
//                      (choose random move)
//                      KeepBranch
//                          FillInSurroundingLibsandEmpty1
//                          CheckNewLibertyCount
//                      MetaPlayPiece
//  Play

//Used throughout
//SetGroupNumber