//#define DEBUG_CHECKS

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GTPLibrary;
using System.Threading;

namespace ProfilableProgram
{
    class Program
    {
        static void Main(string[] args)
        {
            RecurseSpeedTest();
            //GTPInterface test = new GTPInterface();
        }

        //DO THIS NEXT!
        //Make the functions not static and use speedModifier to set monteCarloCount (inside the player)
        //Then go to FastTester and abstract SimpleMontecarloTest so it can accept any goPlayer
        //remember to reset goPlayer inside of it too and stuffs.


        //Original (possibility random play, certainly very poor):
        //  60/s (with accidental debug code left)
        //  140/s (with debug code removed, minor unlikely infinite recursion bug)

        //Overhaul
        // 50/s (with KeepBranch very unoptimized, and spread is broken)
        // 60/s (KeepBranch unoptimized, spread less broken?)
        // 175/s (KeepBranch more optimized, spread appears non broken)
        // 225/s (KeepBranch more optimized, more debug checks removed)
        //  KeepBranch -        34.408 (84.376 with children)
        //  PlayLoop -          32.007 (32.007 with children)
        //  CheckNewLiberty -   10.727 (20.953 with children)
        // 275/s (PlayLoop removed  partially from KeepBranch)
        //  KeepBranch -        48.875 (77.780 with children)
        //  PlayLoop -          13.237 (13.237 with children)
        //  CheckNewLiberty -   14.748 (28.905 with children)

        //75.388/75.333 Non PlayLoop FillInSurroundingLibsandEmpty
        //82.152/82.003 PlayLoop FillInSurroundingLibsAndEmpty

        //1300/s (accidental debug code (same as before) removed)
        //  MetaPlayPiece -     10.330 (63.117 with children)
        //  KeepBranch -        7.2350 (27.561 with children)
        //  PlayLoop -          16.804 (16.804 with children)
        //  CheckNewLiberty -   3.8080 (12.364 with children)

        static void RecurseSpeedTest()
        {
            int boardSampleCount = 1;
            int monteCarloCount = 5;
            int tryPlayCount = 1; //We try to play this amount of pieces, but with overlap we may play less

            Random random = new Random();

            int[][,] boardSamples = new int[boardSampleCount][,];
            for (int x = 0; x < boardSampleCount; x++)
            {
                boardSamples[x] = new int[9, 9];
                for (int y = 0; y < tryPlayCount; y++)
                {
                    int xPos = random.Next(9); int yPos = random.Next(9);
                    if (boardSamples[x][xPos, yPos] == 0)
                        boardSamples[x][xPos, yPos] = (y % 2) + 1;
                }
            }

            int[][,] forcedOutput = new int[boardSampleCount][,];

            long speedTestOne = DateTime.Now.Ticks;
            
            double averagePlainBoard = 0;
            double[,] boardRates = new double[9, 9];
            
            int[,] board = new int[9, 9];
            int[,] libboard = new int[9, 9];
            int[,] groboard = new int[9, 9];
            int curGroupCount = 0;
            int blackCaptured = 0;
            int whiteCaptured = 0;

            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            2, 2, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            4, 4, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            4, 5, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            4, 6, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            5, 4, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            5, 6, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            6, 4, ref blackCaptured, ref whiteCaptured);

            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            6, 5, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            6, 6, ref blackCaptured, ref whiteCaptured);            

            TestDotNetGoPlayer player = new TestDotNetGoPlayer();
            double bestWinRate = 0;
            Ent_vertex vertex = player.GetBest(true, true, board, 1, 0, out bestWinRate);

            double averageScoreTest = 0;
            double rate = 0;

            int totalSims = 100;

            for (int x = 0; x < totalSims; x++)
                rate += TestDotNetGoPlayer.MonteCarloForBlackMetadata(false,
                                board, 0, 0, monteCarloCount, ref averageScoreTest);

            rate /= totalSims;
            averageScoreTest /= totalSims;

            //Just make sure they are the same (as it is a deterministic test anyway)
            for (int x = 0; x < boardSampleCount; x++)
            {
                for(int j = 0; j < 9; j ++)
                    for (int k = 0; k < 9; k++)
                    {
                        int[,] cboard = new int[9, 9]; Array.Copy(board, cboard, 9 * 9);
                        int[,] clibboard = new int[9, 9]; Array.Copy(libboard, clibboard, 9 * 9);
                        int[,] cgroboard = new int[9, 9]; Array.Copy(groboard, cgroboard, 9 * 9);
                        int ccurGroupCount = 0;
                        int cblackCaptured = 0;
                        int cwhiteCaptured = 0;

                        double averageScore = 0;

                        if (board[j, k] > 0)
                        {
                            boardRates[j, k] = board[j, k] + 0.01;
                            continue;
                        }

                        if (!TestDotNetGoPlayer.KeepBranch(1, cboard, clibboard, cgroboard,
                            ref curGroupCount, j, k))
                        {
                            boardRates[j, k] = -100;
                            continue;
                        }


                        TestDotNetGoPlayer.MetaPlayPiece(1, cboard, clibboard, cgroboard, ref ccurGroupCount,
                            j, k, ref cblackCaptured, ref cwhiteCaptured);
                        boardRates[j, k] += TestDotNetGoPlayer.MonteCarloForBlackMetadata(false, 
                            cboard, 0, 0, monteCarloCount, ref averageScore);                        

                        //boardRates[j, k] = averageScore;

                        boardRates[j, k] *= TestDotNetGoPlayer.KeepBranch(1, board, libboard,
                            groboard, ref curGroupCount, j, k) ? 1 : -1;
                    }
            }
            for (int j = 0; j < 9; j++)
                for (int k = 0; k < 9; k++)
                    boardRates[j, k] /= boardSampleCount;
            
                //averagePlainBoard += TestDotNetGoPlayer.MonteCarloForBlack(true, boardSamples[x], 0, 0);
            //averagePlainBoard /= boardSampleCount;

            speedTestOne = DateTime.Now.Ticks - speedTestOne;

            //Console.WriteLine((9 * 9 * TestDotNetGoPlayer.monteCarloCount) / ((speedTestOne / 10000.0) / 1000.0));

            Console.Read();
        }


        public class TestDotNetGoPlayer : SimpleGoPlayer
        {
            static double monteCarloPassAmount = 0;
            static double monteCarloNotPassAmount = 0;

            int blackTaken, whiteTaken;

            public int[,] board = new int[9, 9]; // 0 empty, 1 - black, 2 - white
            //-10 is black territory, -20 is white territory

            public static double komi = 0; //SHOULD NOT BE STATIC

            public TestDotNetGoPlayer()
            {
                blackTaken = 0;
                whiteTaken = 0;
            }

            //public TestDotNetGoPlayer
            public override Ent_vertex GetAndPlayMove(string color)
            {
                //Go a few ply deep, then min/max with montecarlo algorithm to find best
                Ent_vertex vertex;
                double bestWinRate;
                vertex = GetBest(color == "b", color == "b", board, 1, 0, out bestWinRate);

                Play(new Ent_move(new Ent_color(color), vertex));

                return vertex;
            }

            //Ent_vertex is only really used on the last return
            public Ent_vertex GetBest(bool curBlack, bool blackUs, int[,] curBoard, int plyDeep, int curPly, out double bestWinRate)
            {
                if (plyDeep == curPly) //We do not go deeper, just do monte-carlo and then return
                {
                    //ATTENTION, we should really keep track of capped before now, or atleast actually calculate them now!
                    //However if we do right here it will change our parent, and we will either have to keep copying 
                    //curBoard or reverse the changes here

                    //removeCapturesFinal(curBoard, libertyGroups, ref blackCaptured, ref whiteCaptured);

                    int blackCaptured = 0, whiteCaptured = 0;
                    double averageScore = 0;

                    bestWinRate = MonteCarloForBlackMetadata(curBlack, curBoard, blackCaptured, whiteCaptured, 100, ref averageScore);

                    if (!blackUs)
                    {
                        bestWinRate = 1 - bestWinRate;
                        averageScore *= -1;
                    }

                    //Bonus for having a better average score
                    if (averageScore > 10)
                        bestWinRate *= 1.2;
                    else if (averageScore > 5)
                        bestWinRate *= 1.1;
                    else if (averageScore < -10)
                        bestWinRate *= 0.6;
                    else if (averageScore < -5)
                        bestWinRate *= 0.8;
                    else if (averageScore < 0)
                        bestWinRate *= 0.9;

                    return null;
                }

                //Search deeper

                //From curBoard split up... do all possibility for now                        
                double[,] bestWinRates = new double[9, 9];

                int[,] tempNewBoard = new int[9, 9];
                Array.Copy(curBoard, tempNewBoard, 9 * 9);

                int curColor = curBlack ? 1 : 2;
                int ourColor = blackUs ? 1 : 2;

                for (int x = 0; x < 9; x++)
                    for (int y = 0; y < 9; y++)
                    {
                        if (tempNewBoard[x, y] != 0)
                            continue;

                        tempNewBoard[x, y] = curColor; //Place it

                        //Test it
                        GetBest(!curBlack, blackUs, tempNewBoard, plyDeep, curPly + 1, out bestWinRates[x, y]);

                        //Remove it
                        tempNewBoard[x, y] = 0;
                    }

                if (curBlack == blackUs) //We just choose the best
                {
                    Ent_vertex bestVertex = null;
                    double bestRate = 0;

                    for (int x = 0; x < 9; x++)
                        for (int y = 0; y < 9; y++)
                            if (bestWinRates[x, y] > bestRate)
                            {
                                bestRate = bestWinRates[x, y];
                                bestVertex = new Ent_vertex(x, y);
                            }

                    bestWinRate = bestRate;
                    return bestVertex;
                }
                else //We choose the worst (as it is not our turn, and the enemy chooses their best (our worst))
                {
                    Ent_vertex bestVertex = null;
                    double bestRate = double.MaxValue;

                    for (int x = 0; x < 9; x++)
                        for (int y = 0; y < 9; y++)
                            if (bestWinRates[x, y] < bestRate && bestWinRates[x, y] != 0)
                            {
                                bestRate = bestWinRates[x, y];
                                bestVertex = new Ent_vertex(x, y);
                            }

                    bestWinRate = bestRate;
                    return bestVertex;
                }

            }


            static Random random = new Random();
            //Do everything recursively for now, to see speed-boost when it is not recursive
            //Returns score for black (so negative means loss)
            
            public static double MonteCarloForBlackMetadata(bool curBlack, int[,] curBoard,
                int blackCaptured, int whiteCaptured, int totalGames, ref double averageScore)
            {
                int blackWins = 0;

                averageScore = 0;

                for (int x = 0; x < totalGames; x++)
                {
                    int[,] tempNewBoard = new int[9, 9];
                    Array.Copy(curBoard, tempNewBoard, 9 * 9);

                    int innerBlackCap = blackCaptured;
                    int innerWhiteCaptured = whiteCaptured;

                    double curScore = SimulateRandomGameWithMetadata(curBlack, tempNewBoard, 0, ref innerBlackCap, ref innerWhiteCaptured);
                    averageScore += curScore;
                    if (curScore > 0) //Black wins
                    {
                        blackWins++;
                    }
                    //Ignore ties for now
                }

                averageScore /= totalGames;

#if DEBUG_CHECKS
                if (monteCarloPassAmount / monteCarloNotPassAmount > 0.08)
                {
                    throw new Exception("why are is the monte carlo simulation passing so much!");
                    //Why are we passing so much!
                }
#endif

                return (double)blackWins / (double)totalGames;
            }

            //As I expected, PlayLoop is the hottest point in the whole program. If you trust the ANTS performance profiler
            //40% of all time is spent calling this... HOWEVER! You will need to do some adding and comparisons anyway,
            //so while removing this will remove that ugly 40% time it will likely barely speed of the program, and just
            //redistribute the code elsewhere. Instead reduce calls to this (by reducing calls, not replacing it!),
            //and it will get faster, and you will find new places that seem to spend all their time in this,
            //etc, etc, until the real processing is revealed (or not).
            /// <summary>
            /// 0 means hard stop loop, 1 means skip loop and 2 means do loop normally
            /// </summary>                                    
            public static int PlayLoop(ref int x, ref int y, ref int count)
            {
                switch (count)
                {
                    case 0:
                        x++; count++;
                        if (x < 9)
                            return 2;
                        else
                            return 1;
                    case 1:
                        x -= 2; count++;
                        if (x >= 0)
                            return 2;
                        else
                            return 1;
                    case 2:
                        x++; y++; count++;
                        if (y < 9)
                            return 2;
                        else
                            return 1;
                    case 3:
                        y -= 2; count++;
                        if (y >= 0)
                            return 2;
                        else
                            return 1;
                    default:
                        y++; count = 0;
                        return 0;
                }
            }
            //Keeping track of groups and then checking based on that may be faster than this
            //(this will screen multiple empties too, but usually that won't be a problem)
            public static bool ScreenDuplicates(int x, int y, int count, int[,] groupNumbers)
            {
                switch (count)
                {
                    //One off, as count is incremented asap
                    case 1:
                        return ((x - 2 < 0 || groupNumbers[x, y] != groupNumbers[x - 2, y]) &&
                            (y + 1 >= 9 || groupNumbers[x, y] != groupNumbers[x - 1, y + 1]) &&
                            (y - 1 < 0 || groupNumbers[x, y] != groupNumbers[x - 1, y - 1]));
                    case 2:
                        return ((y + 1 >= 9 || groupNumbers[x, y] != groupNumbers[x + 1, y + 1]) &&
                            (y - 1 < 0 || groupNumbers[x, y] != groupNumbers[x + 1, y - 1]));
                    case 3:
                        return (y - 2 < 0 || groupNumbers[x, y] != groupNumbers[x, y - 2]);
                    default:
                        return true;
                }
            }

            public static double SimulateRandomGameWithMetadata(bool curBlack, int[,] curBoard, int curBlackNetCapture, ref int blackCaptured, ref int whiteCaptured)
            {
                //Calculate liberty groups, then in the future we will just keep track of them instead of recalculating them
                int[,] libertyGroups = new int[9, 9]; //|v| = liberty group size, sgn(v) == +, means black, sgn(v) == - means white
                int[,] groupNumbers = new int[9, 9]; //numbered starting at 1, unique number for every group with a max equal to curGroupCount
                int curGroupCount = 0;
                CreateLibertyGroups(curBoard, libertyGroups, groupNumbers, ref curGroupCount);

                return SimMetadataRecurse(curBlack, curBoard, libertyGroups, groupNumbers, ref curGroupCount, curBlackNetCapture, ref blackCaptured, ref whiteCaptured);
            }
            public static double SimMetadataRecurse(bool curBlack, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, int curBlackNetCapture, ref int blackCaptured, ref int whiteCaptured)
            {
                int curPiece = curBlack ? 1 : 2;
                int oppositePiece = curBlack ? 2 : 1;

                bool emptyBlack = false, emptyWhite = false; //Make sure there is an empty square

                for (int x = 0; x < 9 && (!emptyBlack || !emptyWhite); x++)
                    for (int y = 0; y < 9 && (!emptyBlack || !emptyWhite); y++)
                    {
                        //Measure to detect if we should place a piece here, this is key as if this is very good we can
                        //end the analysis early, but if this is has flaws all subsequent analysis will leave vunerabilities
                        if (curBoard[x, y] == 0)
                        {
                            if (emptyBlack) //Then there is not an emptyWhite
                            {
                                if (KeepBranch(2, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y))
                                    emptyWhite = true;
                            }
                            else if (emptyWhite) //Then there is not an emptyBlack
                            {
                                if (KeepBranch(1, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y))
                                    emptyBlack = true;
                            }
                            else
                            {
                                if (KeepBranch(1, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y))
                                    emptyBlack = true;

                                if (KeepBranch(2, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y))
                                    emptyWhite = true;
                            }
                        }
                    }

                //removeCaptures(curBoard, ref blackCaptured, ref whiteCaptured);

                if (!emptyBlack && !emptyWhite) //No where to place for either side, so GAME IS OVER
                {
                    double finalScore = 0;
                    //blackCaptured = 0;
                    //whiteCaptured = 0;

                    removeCapturesFinal(curBoard, libertyGroups, groupNumbers, ref curGroupCount, ref blackCaptured, ref whiteCaptured);

                    //Sort of a stronger version of removeCapturesFinal, but does more guessing
                    guessRemoveDead(curBoard, libertyGroups, groupNumbers, ref curGroupCount, ref blackCaptured, ref whiteCaptured);

                    finalScore += whiteCaptured - blackCaptured;

                    int blackTer, whiteTer;
                    territoryScore(curBoard, out blackTer, out whiteTer);

                    finalScore += blackTer - whiteTer;
                    finalScore += curBlackNetCapture;

                    finalScore += komi;

                    return finalScore;
                }

                //PASS
                if ((curBlack && !emptyBlack) || (!curBlack && !emptyWhite))
                {
                    monteCarloPassAmount++;
                    //ATTENTION! Passes are rare and should not occur more than a few plays before the game ends
                    return SimMetadataRecurse(!curBlack, curBoard, libertyGroups, groupNumbers, ref curGroupCount, curBlackNetCapture, ref blackCaptured, ref whiteCaptured);
                }
                else
                    monteCarloNotPassAmount++;

                int xPlace = random.Next(9);
                int yPlace = random.Next(9);
                //The check if it is good may be unneccessary and will likely be a large hotspot?

                int cycles = 0;
                while (curBoard[xPlace, yPlace] != 0 || !KeepBranch(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, xPlace, yPlace))
                {
                    xPlace = random.Next(9);
                    yPlace = random.Next(9);

                    cycles++;

                    if (cycles > 100)
                    {
                        monteCarloPassAmount++;
                        //ATTENTION! Passes are rare and should not occur more than a few plays before the game ends
                        return SimMetadataRecurse(!curBlack, curBoard, libertyGroups, groupNumbers, ref curGroupCount, curBlackNetCapture, ref blackCaptured, ref whiteCaptured);
                    }
                }

                curBoard[xPlace, yPlace] = curPiece == 1 ? 9 : 8; //Just for debugging, doesn't hurt anything though

                //PLACE PIECE                
                MetaPlayPiece(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, xPlace, yPlace, ref blackCaptured, ref whiteCaptured);


                return SimMetadataRecurse(!curBlack, curBoard, libertyGroups, groupNumbers, ref curGroupCount, curBlackNetCapture, ref blackCaptured, ref whiteCaptured);
            }
            public static void tempemptylibcheck(int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount)
            {
                for (int xp = 0; xp < 9; xp++)
                    for (int yp = 0; yp < 9; yp++)
                    {
                        if (curBoard[xp, yp] < int.MinValue + 100 && libertyGroups[xp, yp] != 0)
                        {
                            throw new Exception("taken piece and liberty group not properly reflecting that.");
                        }
                    }
            }

            public static bool ScreenDuplicatesNew(int x, int y, int count, int[,] groupNumbers)
            {
                if (count == 0)
                {
                    return ((x - 2 < 0 || groupNumbers[x, y] != groupNumbers[x - 2, y]) &&
                     (y + 1 >= 9 || groupNumbers[x, y] != groupNumbers[x - 1, y + 1]) &&
                     (y - 1 < 0 || groupNumbers[x, y] != groupNumbers[x - 1, y - 1]));
                }
                else if (count == 1)
                {
                    return ((y + 1 >= 9 || groupNumbers[x, y] != groupNumbers[x + 1, y + 1]) &&
                            (y - 1 < 0 || groupNumbers[x, y] != groupNumbers[x + 1, y - 1]));
                }
                else if (count == 2)
                {
                    return (y - 2 < 0 || groupNumbers[x, y] != groupNumbers[x, y - 2]);
                }
                else
                {
                    return true;
                }

            }
            public static bool OutOfBounds(int x, int y, int iterNum)
            {
                if (iterNum == 0)
                {
                    return (x >= 9);
                }
                else if (iterNum == 1)
                {
                    return (x < 0);
                }
                else if (iterNum == 2)
                {
                    return (y >= 9);
                }
                else
                {
                    return (y < 0);
                }
            }
            //Remove all PlayLoops in this
            public static void MetaPlayPiece(int curPiece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, int x, int y, ref int blackCaptured, ref int whiteCaptured)
            {
                int ourSgn = curPiece == 1 ? 1 : -1;

                int oppositePiece = curPiece == 1 ? 2 : 1;

                int capped = 0;
                int newGroupLiberties = 0;
                //Recalculate adjacent liberty groups

                curBoard[x, y] = curPiece == 1 ? 9 : 8; //Just for debugging, that way I can visually see where we are placing a piece
                //curBoard[x, y] = curPiece;

                //The adjacent groups which needs to be combined
                int[] xGroups = { -1, -1, -1, -1 };
                int[] yGroups = { -1, -1, -1, -1 };
                int curCombineGroups = 0;

                int emptySurrounding = 0;

                //KeepBranch(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y);

                //int count = 0, contVal = 0;

                int[] xOff = { 1, -1, 0, 0 };
                int[] yOff = { 0, 0, 1, -1 };

                //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
                for(int f = 0; f < 4; f++)
                {
                    x += xOff[f];
                    y += yOff[f];

                    //1
                    if (OutOfBounds(x, y, f))
                    {
                        x -= xOff[f];
                        y -= yOff[f];
                        continue;
                    }
                    
                    if (libertyGroups[x, y] * ourSgn == -1) //It has no more liberties and so it dies now
                    {
                        int takenPiece = curBoard[x, y] == 1 ? int.MinValue + 1 : int.MinValue;

                        //Remove other group
                        capped += removeGroupMeta(curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y, oppositePiece, curPiece, takenPiece);
                        emptySurrounding++; //Now there is an empty here
                    }
                    else if (libertyGroups[x, y] * ourSgn < 0) //We just decrement its number of liberties, unless of course it borders us more than once
                    {
                        if (ScreenDuplicatesNew(x, y, f, groupNumbers))
                            SetGroupNumber(libertyGroups, x, y, libertyGroups[x, y] + ourSgn, libertyGroups[x, y]);
                    }
                    else if (libertyGroups[x, y] * ourSgn > 0) //Add it to our new liberty grouping
                    {
                        //0.6
                        if (ScreenDuplicatesNew(x, y, f, groupNumbers))
                        {
                            xGroups[curCombineGroups] = x;
                            yGroups[curCombineGroups] = y;
                            curCombineGroups++;
                        }
                    }
                    else
                    {
                        emptySurrounding++;
                    }

                    x -= xOff[f];
                    y -= yOff[f];

                }

                //count = 0; contVal = 0; these should naturally reset in PlayLoop

                //We also need to check for liberty overlap on the groups we are combining   

                int combineOverlapAmount = 0;
                //Remove our emptySurroundings that are extra
                if (curCombineGroups == 1)
                {
                    for (int f = 0; f < 4; f++)
                    {
                        x += xOff[f];
                        y += yOff[f];

                        //1
                        if (OutOfBounds(x, y, f))
                        {
                            x -= xOff[f];
                            y -= yOff[f];
                            continue;
                        }
                        bool libertyAccountedFor = false;
                        int subX = x, subY = y, subCount = 0;

                        //Could also just check libertyGroups
                        if (IsEmpty(curBoard, x, y)) //All liberties we will get                    
                            //We will recycle contVal, as it is only really used right after it is set
                            for (int subf = 0; subf < 4 && !libertyAccountedFor; subf++)
                            {
                                x += xOff[subf];
                                y += yOff[subf];

                                //1
                                if (OutOfBounds(x, y, subf))
                                {
                                    x -= xOff[subf];
                                    y -= yOff[subf];
                                    continue;
                                }                                

                                //If we will be sharing a liberty with this group, and it is the same type as us
                                if (curBoard[subX, subY] == curPiece) //libertyGroups[x, y] * ourSgn > 0                            
                                    for (int b = 0; b < curCombineGroups; b++) //If it is beside a group we will add to us anyway, then don't add it twice                                     
                                        if (groupNumbers[xGroups[b], yGroups[b]] == groupNumbers[subX, subY])
                                        {
                                            libertyAccountedFor = true;
                                            break;
                                        }

                                x -= xOff[subf];
                                y -= yOff[subf];
                            }

                        if (libertyAccountedFor)
                            emptySurrounding--;

                        x -= xOff[f];
                        y -= yOff[f];
                         
                    }
                }
                //If there are more than 2 liberty groups, it is probably just faster to just combine the group
                //and recalculate all of its liberties from scratch (faster though, cause it is just one group)
                else if (curCombineGroups >= 2)
                {
                    //Not sure whether making a metadata map or just linked arrays is faster?
                    //(data static, position data implicit VS data dynamic, position data stored)
                    //also, static positioning allows much faster searching

                    curBoard[x, y] = curPiece; //Place the piece

                    //It may be possible to make a metaMap and then
                    int[,] newMetaMap = new int[9, 9];
                    Array.Copy(curBoard, newMetaMap, 9 * 9);
                    newGroupLiberties = RecurseMarkLibs(curPiece, newMetaMap, x, y);

                    //Set everything around us with the new group number
                    curGroupCount++; //We should probably recycle (a previous group number), but its okay                    
                    for (int b = 0; b < curCombineGroups; b++)
                        SetGroupNumber(groupNumbers, xGroups[b], yGroups[b], curGroupCount, groupNumbers[xGroups[b], yGroups[b]]);
                    groupNumbers[x, y] = curGroupCount;

                    curGroupCount++; //We increment it twice, because we use group to follow the paths around
                    SetGroupNumberFromOther(libertyGroups, groupNumbers, x, y, newGroupLiberties * ourSgn, curGroupCount - 1, curGroupCount);
                }

                if (curCombineGroups < 2) //We redo everything if it is 2 or more
                {
                    //Take all of the groups and combine them, now that they liberties are correct as removed groups have been accounted for
                    for (int b = 0; b < curCombineGroups; b++)
                        newGroupLiberties += libertyGroups[xGroups[b], yGroups[b]];

                    newGroupLiberties -= curCombineGroups * ourSgn; //Subtract as we are losing 1 liberty per group we touch
                    newGroupLiberties -= combineOverlapAmount * ourSgn;
                    newGroupLiberties += emptySurrounding * ourSgn;

                    curGroupCount++;
                    int newGroupNumber = curGroupCount;
                    //Now, we combine all of the libertyGroups
                    libertyGroups[x, y] = newGroupLiberties;
                    groupNumbers[x, y] = newGroupNumber;
                    //curBoard[x, y] = curPiece; (done later now, for debugging purposes)
                    for (int b = 0; b < curCombineGroups; b++) //Renumbers the liberties and the groups
                    {
                        if (newGroupLiberties != libertyGroups[xGroups[b], yGroups[b]]) //It is entirely possible to place beside a group, and not change its liberties
                            SetGroupNumber(libertyGroups, xGroups[b], yGroups[b], newGroupLiberties, libertyGroups[xGroups[b], yGroups[b]]);
                        SetGroupNumber(groupNumbers, xGroups[b], yGroups[b], newGroupNumber, groupNumbers[xGroups[b], yGroups[b]]);
                    }
                }

                

                curBoard[x, y] = curPiece;


                if (capped > 0)
                {
                    if (oppositePiece == 1) //We capture opposite pieces when we play!
                        blackCaptured += capped;
                    else
                        whiteCaptured += capped;
                }

            }

            //Essentially MetaPlayPiece, but stops before playing it and just checks, may get poor performance now... but... idk
            public static int CheckNewLibertyCount(int curPiece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers,
                int curGroupCount, int x, int y)
            {
int ourSgn = curPiece == 1 ? 1 : -1;

                int oppositePiece = curPiece == 1 ? 2 : 1;

                int capped = 0;
                int newGroupLiberties = 0;
                //Recalculate adjacent liberty groups

                //curBoard[x, y] = curPiece == 1 ? 9 : 8; //Just for debugging, that way I can visually see where we are placing a piece
                //curBoard[x, y] = curPiece;

                //The adjacent groups which needs to be combined
                int[] xGroups = { -1, -1, -1, -1 };
                int[] yGroups = { -1, -1, -1, -1 };
                int curCombineGroups = 0;

                int emptySurrounding = 0;

                //KeepBranch(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y);

                //int count = 0, contVal = 0;

                int[] xOff = { 1, -1, 0, 0 };
                int[] yOff = { 0, 0, 1, -1 };

                //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
                for(int f = 0; f < 4; f++)
                {
                    x += xOff[f];
                    y += yOff[f];

                    //1
                    if (OutOfBounds(x, y, f))
                    {
                        x -= xOff[f];
                        y -= yOff[f];
                        continue;
                    }
                    
                    if (libertyGroups[x, y] * ourSgn == -1) //It has no more liberties and so it dies now
                    {
                        int takenPiece = curBoard[x, y] == 1 ? int.MinValue + 1 : int.MinValue;

                        //COPY OPERATION (this should buffer the caller from any changes we make, but of course it is less efficient  
                        int[,] ccurBoard = new int[9, 9]; int[,] clibertyGroups = new int[9, 9]; int[,] cgroupNumbers = new int[9, 9];
                        Array.Copy(curBoard, ccurBoard, 9 * 9); Array.Copy(libertyGroups, clibertyGroups, 9 * 9); Array.Copy(groupNumbers, cgroupNumbers, 9 * 9);
                        curBoard = ccurBoard; libertyGroups = clibertyGroups; groupNumbers = cgroupNumbers;

                        //Remove other group
                        capped += removeGroupMeta(curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y, oppositePiece, curPiece, takenPiece);
                        emptySurrounding++; //Now there is an empty here
                    }
                    else if (libertyGroups[x, y] * ourSgn < 0) //We just decrement its number of liberties, unless of course it borders us more than once
                    {
                        if (ScreenDuplicatesNew(x, y, f, groupNumbers))
                            SetGroupNumber(libertyGroups, x, y, libertyGroups[x, y] + ourSgn, libertyGroups[x, y]);
                    }
                    else if (libertyGroups[x, y] * ourSgn > 0) //Add it to our new liberty grouping
                    {
                        //0.6
                        if (ScreenDuplicatesNew(x, y, f, groupNumbers))
                        {
                            xGroups[curCombineGroups] = x;
                            yGroups[curCombineGroups] = y;
                            curCombineGroups++;
                        }
                    }
                    else
                    {
                        emptySurrounding++;
                    }

                    x -= xOff[f];
                    y -= yOff[f];

                }                

                //count = 0; contVal = 0; these should naturally reset in PlayLoop

                //We also need to check for liberty overlap on the groups we are combining   

                int combineOverlapAmount = 0;
                //Remove our emptySurroundings that are extra
                if (curCombineGroups == 1)
                {
                    for (int f = 0; f < 4; f++)
                    {
                        x += xOff[f];
                        y += yOff[f];

                        //1
                        if (OutOfBounds(x, y, f))
                        {
                            x -= xOff[f];
                            y -= yOff[f];
                            continue;
                        }
                        bool libertyAccountedFor = false;
                        int subX = x, subY = y, subCount = 0;

                        //Could also just check libertyGroups
                        if (IsEmpty(curBoard, x, y)) //All liberties we will get                    
                            //We will recycle contVal, as it is only really used right after it is set
                            for (int subf = 0; subf < 4 && !libertyAccountedFor; subf++)
                            {
                                x += xOff[subf];
                                y += yOff[subf];

                                //1
                                if (OutOfBounds(x, y, subf))
                                {
                                    x -= xOff[subf];
                                    y -= yOff[subf];
                                    continue;
                                }                                

                                //If we will be sharing a liberty with this group, and it is the same type as us
                                if (curBoard[subX, subY] == curPiece) //libertyGroups[x, y] * ourSgn > 0                            
                                    for (int b = 0; b < curCombineGroups; b++) //If it is beside a group we will add to us anyway, then don't add it twice                                     
                                        if (groupNumbers[xGroups[b], yGroups[b]] == groupNumbers[subX, subY])
                                        {
                                            libertyAccountedFor = true;
                                            break;
                                        }

                                x -= xOff[subf];
                                y -= yOff[subf];
                            }

                        if (libertyAccountedFor)
                            emptySurrounding--;

                        x -= xOff[f];
                        y -= yOff[f];
                         
                    }
                }
                //If there are more than 2 liberty groups, it is probably just faster to just combine the group
                //and recalculate all of its liberties from scratch (faster though, cause it is just one group)
                else if (curCombineGroups >= 2)
                {
                    //Not sure whether making a metadata map or just linked arrays is faster?
                    //(data static, position data implicit VS data dynamic, position data stored)
                    //also, static positioning allows much faster searching

                  //  curBoard[x, y] = curPiece; //Place the piece

                    //It may be possible to make a metaMap and then
                    int[,] newMetaMap = new int[9, 9];
                    Array.Copy(curBoard, newMetaMap, 9 * 9);
                    newGroupLiberties = RecurseMarkLibs(curPiece, newMetaMap, x, y);

                    return newGroupLiberties;
                }

                if (curCombineGroups < 2) //We redo everything if it is 2 or more
                {
                    //Take all of the groups and combine them, now that they liberties are correct as removed groups have been accounted for
                    for (int b = 0; b < curCombineGroups; b++)
                        newGroupLiberties += libertyGroups[xGroups[b], yGroups[b]];

                    newGroupLiberties -= curCombineGroups * ourSgn; //Subtract as we are losing 1 liberty per group we touch
                    newGroupLiberties -= combineOverlapAmount * ourSgn;
                    newGroupLiberties += emptySurrounding * ourSgn;

                    return newGroupLiberties;
                }

                return 0; //We never get here
            }

            //Sort of opposite of taking empty squares and using them to add liberties to groups... this just
            //follows a group and marks liberties around it. This should be slightly faster for small groups.

            //Follows the piece on metaBoard, marking liberties on metaBoard (and previous paths followed) and counting all marked.
            //metaBoard is originally just a copy of the curBoard
            public static int RecurseMarkLibs(int piece, int[,] metaBoard, int x, int y)
            {
                int liberties = 0;
                int count = 0, contVal = 0;

                metaBoard[x, y] = -1; //Mark it so we don't visit it again

                //I am sure there is a way to combine these loops and optimize it to minize checking squares
                //for liberties and pieces...but w/e, it will probably not be a problem

                //First mark all liberties, then visit all neighbours
                //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
                {
                    if (contVal == 1)
                        continue;

                    if (IsEmpty(metaBoard, x, y))
                    {
                        liberties++;
                        metaBoard[x, y] = -1; //Mark it so we don't count it again
                    }
                }

                //Visit other squares (we interestingly enough, keep changing the same x and y values all over the stack, but whenever we return
                //from a function, they are the same as they were before the function was called, and so we can likely optimize this by
                //using static x and y, instead of passing refs.... but w/e).
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
                {
                    if (contVal == 1)
                        continue;

                    if (metaBoard[x, y] == piece)
                        liberties += RecurseMarkLibs(piece, metaBoard, x, y);
                }

                return liberties;
            }

            public static int RecurseMaxOneLib(int piece, int[,] metaBoard, int x, int y)
            {
                int liberties = 0;
                int count = 0, contVal = 0;

                metaBoard[x, y] = -1; //Mark it so we don't visit it again

                //I am sure there is a way to combine these loops and optimize it to minize checking squares
                //for liberties and pieces...but w/e, it will probably not be a problem

                //First mark all liberties, then visit all neighbours
                //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
                {
                    if (contVal == 1)
                        continue;

                    if (IsEmpty(metaBoard, x, y))
                    {
                        liberties++;

                        if (liberties > 1)
                            return liberties;
                        metaBoard[x, y] = -1; //Mark it so we don't count it again
                    }
                }

                //Visit other squares (we interestingly enough, keep changing the same x and y values all over the stack, but whenever we return
                //from a function, they are the same as they were before the function was called, and so we can likely optimize this by
                //using static x and y, instead of passing refs.... but w/e).
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
                {
                    if (contVal == 1)
                        continue;

                    if (metaBoard[x, y] == piece)
                        liberties += RecurseMarkLibs(piece, metaBoard, x, y);

                    if (liberties > 1)
                        return liberties;
                }

                return liberties;
            }

            public static bool IsEmpty(int[,] curBoard, int x, int y)
            {
#if DEBUG_CHECKS
                //These are empty flags this doesn't catch, these should never be true here because they are purely for debugging
                if (curBoard[x, y] == 8 || curBoard[x, y] == 9 || curBoard[x, y] == -9 || curBoard[x, y] == -8)
                {
                throw new Exception("improper use of IsEmpty");
                }

#endif
                return curBoard[x, y] == 0 || curBoard[x, y] < int.MinValue + 100;
            }            

            //This is the hottest method, and there will likely be many copies of this for different purposes of different speed
            //and of different quality

            //24.286
            //24.706
            //23.691
            //23.600
            //23.955
            //23.569 (x and y no longer changed)
            //static int[] groupsAround = { 0, 0, 0, 0 };
            static int groupsAroundCount = 0;
            public static void FillInSurroundingLibsandEmpty1(int[,] libertyGroups, int[,] groupNumbers, int x, int y, int[] surroundingLibs, ref int emptySurroundings,
                int ourSgn, ref int duplicateGroups)
            {
                int[] groupsAround = { 0, 0, 0, 0 }; //This could probably be put in shared memory or something, 
                //but not now the bulk initialization is just so much faster, so it will be left like this                                                

                int tempPos = 0;
                groupsAroundCount = 0;

                if (x + 1 < 9)
                {
                    //x++;
                    if (libertyGroups[x + 1, y] == 0)
                    {
                        surroundingLibs[0] = 0;
                        emptySurroundings++;
                    }
                    else
                    {
                        //ScreenDuplicates,no duplicates on the first one                        
                        groupsAround[groupsAroundCount++] = groupNumbers[x + 1, y];
                        surroundingLibs[0] = libertyGroups[x + 1, y] * ourSgn;
                    }
                    //x--;
                }
                else
                    surroundingLibs[0] = int.MinValue + 10000; //SPECIAL EDGE OF BOARD SENTINEL

                if (x - 1 >= 0)
                {
                    //x--;
                    if (libertyGroups[x - 1, y] == 0)
                    {
                        surroundingLibs[1] = 0;
                        emptySurroundings++;
                    }
                    else
                    {
                        //ScreenDuplicates
                        if (groupsAroundCount == 0 || groupsAround[0] != groupNumbers[x - 1, y]) //Only one duplicate to check for (groupsAroundCount max of 1)
                        {
                            groupsAround[groupsAroundCount++] = groupNumbers[x - 1, y];
                            surroundingLibs[1] = libertyGroups[x - 1, y] * ourSgn;
                        }
                        else //DUPLICATE
                        {
                            surroundingLibs[1] = int.MinValue + 20000 + 100 * ourSgn; //DUPLICATE SENTINEL
                            duplicateGroups++;
                        }
                    }
                    //x++;
                }
                else
                    surroundingLibs[1] = int.MinValue + 10000; //SPECIAL EDGE OF BOARD SENTINEL

                if (y + 1 < 9)
                {
                    //y++;
                    if (libertyGroups[x, y + 1] == 0)
                    {
                        surroundingLibs[2] = 0;
                        emptySurroundings++;
                    }
                    else
                    {
                        //ScreenDuplicates
                        for (tempPos = 0; tempPos < groupsAroundCount; tempPos++)
                            if (groupsAround[tempPos] == groupNumbers[x, y + 1])
                                break;
                        if (tempPos == groupsAroundCount)
                        //Dynamic checking gets faster at this points
                        {
                            groupsAround[groupsAroundCount++] = groupNumbers[x, y + 1];
                            surroundingLibs[2] = libertyGroups[x, y + 1] * ourSgn;
                        }
                        else //DUPLICATE
                        {
                            surroundingLibs[2] = int.MinValue + 20000 + 100 * ourSgn; //DUPLICATE SENTINEL
                            duplicateGroups++;
                        }
                    }
                    //y--;
                }
                else
                    surroundingLibs[2] = int.MinValue + 10000; //SPECIAL EDGE OF BOARD SENTINEL

                if (y - 1 >= 0)
                {
                    //y--;
                    if (libertyGroups[x, y - 1] == 0)
                    {
                        surroundingLibs[3] = 0;
                        emptySurroundings++;
                    }
                    else
                    {
                        //ScreenDuplicates
                        for (tempPos = 0; tempPos < groupsAroundCount; tempPos++)
                            if (groupsAround[tempPos] == groupNumbers[x, y - 1])
                                break;
                        if (tempPos == groupsAroundCount)
                        {
                            groupsAround[groupsAroundCount++] = groupNumbers[x, y - 1];
                            surroundingLibs[3] = libertyGroups[x, y - 1] * ourSgn;
                        }
                        else //DUPLICATE
                        {
                            surroundingLibs[3] = int.MinValue + 20000 + 100 * ourSgn; //DUPLICATE SENTINEL
                            duplicateGroups++;
                        }
                    }
                    //y++;
                }
                else
                    surroundingLibs[3] = int.MinValue + 10000; //SPECIAL EDGE OF BOARD SENTINEL
            }
            
            public static bool KeepBranch(int piece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, int x, int y)
            {
                //Just not a good play, this will probably be expensive... it basically places the piece and accounts for every
                //possible situation (as is basically MetaPlayPiece), but it should not modify any arrays

                int oppositePiece = piece == 1 ? 2 : 1;
                int ourSgn = piece == 1 ? 1 : -1;

                int emptySurroundings = 0;

                int duplicateGroups = 0;

                //Add up adjacent liberty groups
                int[] surroundingLibs = new int[4];

                if (curBoard[x, y] <= int.MinValue + 100)//Already capped, either super secure or certain death (usually)                
                    return false;

                FillInSurroundingLibsandEmpty1(libertyGroups, groupNumbers, x, y, surroundingLibs, ref emptySurroundings, ourSgn, ref duplicateGroups); //emptySurroundings = 0;
                //FillInSurroundingLibsandEmpty2(libertyGroups, groupNumbers, x, y, surroundingLibs, ref emptySurroundings, ourSgn); emptySurroundings = 0;
                //FillInSurroundingLibsandEmpty2(libertyGroups, groupNumbers, x, y, surroundingLibs, ref emptySurroundings, ourSgn); emptySurroundings = 0;
                //FillInSurroundingLibsandEmpty1(libertyGroups, groupNumbers, x, y, surroundingLibs, ref emptySurroundings, ourSgn);

#if DEBUG_CHECKS
                int contVal, count = 0;
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
                {
                    if (contVal == 1)
                        continue;


                    if (libertyGroups[x, y] < (int.MinValue + 100)) 
                        //Should never happen, I though we screened strange value out of libertyGroups
                    {
                    throw new Exception("liberty groups contains strange values");
                    }
                }
#endif

                //We cannot be suiciding if any surrounding groups have 3 or more liberties, as we will take away 1 leaving atleast 2
                if (surroundingLibs[0] < 3 &&
                    surroundingLibs[1] < 3 &&
                    surroundingLibs[2] < 3 &&
                    surroundingLibs[3] < 3 &&
                    emptySurroundings < 2) //emptySurroundings is not reduced when 
                {
                    //If we can take something it is okay, this check can likely be done early only...but it will likely usually be true
                    if (surroundingLibs[0] != -1 && surroundingLibs[1] != -1 && surroundingLibs[2] != -1 && surroundingLibs[3] != -1)
                    {
                        //If they are all 1 or less, we HAVE to take something or else it will be a suicide
                        //(and we aren't taking something as we checked that, so it is bad
                        if (emptySurroundings < 2 && surroundingLibs[0] < 2 && surroundingLibs[1] < 2 && surroundingLibs[2] < 2 && surroundingLibs[3] < 2)
                        {
                            //Oops, we can also have empty squares around us
                            return false;
                        }
                        else //We have surrounding groups which have 2 liberties, so placing may or may not result in 3 liberties
                        {
                            //About 10% of the time the things is we share a liberty with a group we are beside
                            //and this makes it so placing becomes a bad idea

                            //About 1% of the time we have duplicateGroups >= 2, and 99% of these cases result in not enough liberties

                            if (duplicateGroups >= 2)
                            {
                                //This check is not really needed as it almost always returns false... but this should not be a hot point anyway
                                int newLibs = CheckNewLibertyCount(piece, curBoard, libertyGroups, groupNumbers, curGroupCount, x, y);

                                if (newLibs * ourSgn <= 1)
                                    return false;
                            }
                            else
                            {
                                int newLibs = CheckNewLibertyCount(piece, curBoard, libertyGroups, groupNumbers, curGroupCount, x, y);
                                //Apparently newLibs is never == 0?


                                int enemyGroups = 0, friendlyGroups = 0;
                                for (int m = 0; m < 4; m++)
                                {
                                    if (surroundingLibs[m] * ourSgn > 0)
                                        enemyGroups++;
                                    else if (surroundingLibs[m] * ourSgn > 0)
                                        friendlyGroups++;
                                }

                                //For now we will just not CheckNewLibertyCount, this means some of the time we are off
                                //and place in bad spots... but for now it should be okay and not impact the results by too much
                                if (emptySurroundings == 0) //about 40% of the time
                                {
                                    //Usually returns false
                                    if (enemyGroups == 0)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else if (enemyGroups == 1)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else if (enemyGroups == 2)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else if (enemyGroups == 3)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                }
                                else if (emptySurroundings == 1) //about 60% of the time
                                {
                                    //Usually returns false
                                    if (enemyGroups == 0)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else if (enemyGroups == 1)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else if (enemyGroups == 2)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else if (enemyGroups == 3)
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                    else
                                    {
                                        if (newLibs * ourSgn <= 1)
                                            return false;
                                    }
                                }



                                //int newLibs = CheckNewLibertyCount(piece, curBoard, libertyGroups, groupNumbers, curGroupCount, x, y);
                                ////This is a hot point, no duplicate groups but about 10% we don't have enough liberties

                                //if (InsureMoreThanOneLiberty(piece, curBoard, libertyGroups, groupNumbers, curGroupCount, x, y)
                                //    != (newLibs * ourSgn > 1))
                                //{
                                //    InsureMoreThanOneLiberty(piece, curBoard, libertyGroups, groupNumbers, curGroupCount, x, y);
                                //}

                                ////Apparently newLibs is never == 0?

                                //    if (emptySurroundings == 0) //about 40% of the time
                                //    {
                                //        if (newLibs * ourSgn <= 1) //Usually returns false
                                //            return false;
                                //    }
                                //    else if (emptySurroundings == 1) //about 60% of the time
                                //    {
                                //        if (newLibs * ourSgn <= 1) //Usually does not return false
                                //            return false;
                                //    }

                            }
                        }
                    }
                }

                //If any places are in danger of being taken, we should save them (and of course we know it is not suicide, because of the above)
                for (int d = 0; d < 4; d++)
                    if (surroundingLibs[d] == 1)
                        return true;

                //Make sure we are not placing in places with no threats, and that we already have lots of pieces
                if (surroundingLibs[0] >= 0 && surroundingLibs[1] >= 0 && surroundingLibs[2] >= 0 && surroundingLibs[3] >= 0) //No threats
                {
                    //This is not working, so lets be super aggressive... only place when you are next to an enemy
                    //return false;

                    int neighbours = 0;
                    for (int d = 0; d < 4; d++)
                        if (surroundingLibs[d] > 0)
                            neighbours++;

                    if (neighbours >= 3) //So many neighbours, so we have no need to place here (more aggressive moves are likely always better)
                        return false;
                }

                //We could also check for small areas we are secured with two eyes and the enemy cannot form two eyes in.
                return true;
            }

            public static void removeCapturesFinal(int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, ref int blackCaptured, ref int whiteCaptured)
            {
                for (int x = 0; x < 9; x++)
                    for (int y = 0; y < 9; y++)
                        if (curBoard[x, y] > 0)
                        {
                            if (libertyGroups[x, y] == 0)
                            {
                                if (curBoard[x, y] == 1)
                                    blackCaptured++;
                                else
                                    whiteCaptured++;

                                int takenPiece = curBoard[x, y] == 1 ? int.MinValue + 1 : int.MinValue;

                                curBoard[x, y] = takenPiece; //Mark it so we never place there again
                            }
                        }
            }

            //ATTENTION! I am not sure which of finding the liberty groups by following the empty squares and adding to places around them,
            //or following groups and adding the empty squares around them is faster, test both?

            //|v| = liberty group size, sgn(v) == +, means black, sgn(v) == - means white
            //Creates from just a basic board, with 0 for empty, 1 for black and 2 for white (no other pieces). libertyGroups is just overwritten right away too

            //removes special marked empty squares and just makes them empty
            public static void CreateLibertyGroups(int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount)
            {
                curGroupCount = 0;

                Array.Copy(curBoard, libertyGroups, 9 * 9);
                Array.Copy(curBoard, groupNumbers, 9 * 9);

                for (int x = 0; x < 9; x++)
                    for (int y = 0; y < 9; y++)
                    {
                        //libertyGroups[x, y] = 0;
                        //groupNumbers[x, y] = 0;
                        if (curBoard[x, y] == 1 || curBoard[x, y] == 2)
                        {
                            curGroupCount++;
                            SetGroupNumber(libertyGroups, x, y, -(curGroupCount), libertyGroups[x, y]);
                            SetGroupNumber(groupNumbers, x, y, -(curGroupCount), groupNumbers[x, y]);
                        }
                        else if (groupNumbers[x, y] < int.MinValue + 100)
                        {
                            curBoard[x, y] = 0;
                            libertyGroups[x, y] = 0;
                            groupNumbers[x, y] = 0;
                        }
                    }

                int[] libertyCounts = new int[curGroupCount];

                for (int x = 0; x < 9; x++)
                    for (int y = 0; y < 9; y++)
                        if (libertyGroups[x, y] == 0 || libertyGroups[x, y] < int.MinValue + 100) //Every empty square adds liberties to the groups around it   
                        {
                            //OOPS, we can border one group multiple times, so... lets fix that here

                            //    4
                            //   2x1
                            //    3

                            if (x + 1 < 9 && groupNumbers[x + 1, y] != 0)
                            {
                                //If 1 overlaps anything, don't add it (the last one will add it)
                                // 1-2 ... 1-3 ... 1-4
                                if ((x - 1 < 0 || groupNumbers[x + 1, y] != groupNumbers[x - 1, y]) &&
                                    (y + 1 >= 9 || groupNumbers[x + 1, y] != groupNumbers[x, y + 1]) &&
                                    (y - 1 < 0 || groupNumbers[x + 1, y] != groupNumbers[x, y - 1]))
                                    libertyCounts[-groupNumbers[x + 1, y] - 1]++;
                            }

                            if (x - 1 >= 0 && groupNumbers[x - 1, y] != 0)
                            {
                                if ((y + 1 >= 9 || groupNumbers[x - 1, y] != groupNumbers[x, y + 1]) &&
                                    (y - 1 < 0 || groupNumbers[x - 1, y] != groupNumbers[x, y - 1]))
                                    libertyCounts[-groupNumbers[x - 1, y] - 1]++;
                            }

                            if (y + 1 < 9 && groupNumbers[x, y + 1] != 0)
                            {
                                if (y - 1 < 0 || groupNumbers[x, y + 1] != groupNumbers[x, y - 1])
                                    libertyCounts[-groupNumbers[x, y + 1] - 1]++;
                            }

                            if (y - 1 >= 0 && groupNumbers[x, y - 1] != 0)
                                libertyCounts[-groupNumbers[x, y - 1] - 1]++;
                        }

                //Renumber groups, 
                for (int x = 0; x < 9; x++)
                    for (int y = 0; y < 9; y++)
                        if (groupNumbers[x, y] < 0) //It is still numbered, without a count
                        {
                            //groupNumbers[x, y] = -groupNumbers[x, y];

                            int sgn = curBoard[x, y] == 1 ? 1 : -1;
                            int libGroupNum = -groupNumbers[x, y] - 1;
                            if (libertyCounts[libGroupNum] * sgn != libertyGroups[x, y]) //Our count may just happen to equal our previous group number
                            {
                                SetGroupNumberFromOther(libertyGroups, groupNumbers, x, y, libertyCounts[libGroupNum] * sgn,
                                    groupNumbers[x, y], -groupNumbers[x, y]);
                            }
                        }
            }
            //Numbers them, but with negative numbers
            public static void SetGroupNumber(int[,] libertyGroups, int curX, int curY, int newNumber, int colorToNumber)
            {
                if (newNumber == colorToNumber)
                    throw new Exception("you cannot replace something with itself, there is an error in the calling code.");

                libertyGroups[curX, curY] = newNumber;

                if (curX + 1 < 9 && libertyGroups[curX + 1, curY] == colorToNumber)
                    SetGroupNumber(libertyGroups, curX + 1, curY, newNumber, colorToNumber);
                if (curX - 1 >= 0 && libertyGroups[curX - 1, curY] == colorToNumber)
                    SetGroupNumber(libertyGroups, curX - 1, curY, newNumber, colorToNumber);
                if (curY + 1 < 9 && libertyGroups[curX, curY + 1] == colorToNumber)
                    SetGroupNumber(libertyGroups, curX, curY + 1, newNumber, colorToNumber);
                if (curY - 1 >= 0 && libertyGroups[curX, curY - 1] == colorToNumber)
                    SetGroupNumber(libertyGroups, curX, curY - 1, newNumber, colorToNumber);
            }

            public static void SetGroupNumberFromOther(int[,] valueToSet, int[,] valuesToFollow, int curX, int curY, int newValue, int valueToFollow, int newValueToFollow)
            {
                valueToSet[curX, curY] = newValue;
                valuesToFollow[curX, curY] = newValueToFollow;

                if (curX + 1 < 9 && valuesToFollow[curX + 1, curY] == valueToFollow)
                    SetGroupNumberFromOther(valueToSet, valuesToFollow, curX + 1, curY, newValue, valueToFollow, newValueToFollow);
                if (curX - 1 >= 0 && valuesToFollow[curX - 1, curY] == valueToFollow)
                    SetGroupNumberFromOther(valueToSet, valuesToFollow, curX - 1, curY, newValue, valueToFollow, newValueToFollow);
                if (curY + 1 < 9 && valuesToFollow[curX, curY + 1] == valueToFollow)
                    SetGroupNumberFromOther(valueToSet, valuesToFollow, curX, curY + 1, newValue, valueToFollow, newValueToFollow);
                if (curY - 1 >= 0 && valuesToFollow[curX, curY - 1] == valueToFollow)
                    SetGroupNumberFromOther(valueToSet, valuesToFollow, curX, curY - 1, newValue, valueToFollow, newValueToFollow);
            }

            public static int removeGroupMeta(int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, int x, int y, int curPiece, int oppositePiece, int newPiece)
            {
                int[] groupsAdjacent = new int[9 * 9 + 1]; //We can actually find the max, and it will likely be a lot lower than this, but for now this works
                int[] numberAdjacent = new int[9 * 9 + 1];
                int[] xStart = new int[9 * 9 + 1];
                int[] yStart = new int[9 * 9 + 1];

                for (int a = 0; a < 9 * 9 + 1; a++)
                {
                    groupsAdjacent[a] = 0;
                    numberAdjacent[a] = 0;
                    xStart[a] = 0;
                    yStart[a] = 0;
                }

                int amountRemoved = removeGroupMetaRecurs(curBoard, libertyGroups, groupNumbers, x, y, curPiece, oppositePiece, newPiece, 0,
                    groupsAdjacent, numberAdjacent, xStart, yStart);

                //Now add the extra liberties to the groups affected
                int pieceSgn = oppositePiece == 1 ? 1 : -1;

                int pos = 0;
                while (groupsAdjacent[pos] != 0)
                {
                    curGroupCount++;
                    SetGroupNumberFromOther(libertyGroups, groupNumbers, xStart[pos], yStart[pos], pieceSgn * numberAdjacent[pos] +
                        libertyGroups[xStart[pos], yStart[pos]], groupNumbers[xStart[pos], yStart[pos]], curGroupCount);
                    pos++;
                }

                return amountRemoved;
            }

            public static int removeGroupMetaRecurs(int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, int x, int y, int curPiece, int oppositePiece, int newPiece, int curCount,
                int[] groupsAdjacent, int[] numberAdjacent, int[] xStart, int[] yStart)
            {
                //Remove piece
                curBoard[x, y] = newPiece;
                libertyGroups[x, y] = 0; //IMPORTANT! If you forget this it appears to group still exists!
                groupNumbers[x, y] = 0;
                curCount++;

                //tempemptylibcheck(curBoard, libertyGroups);

                //See if there are any new groups around us, and if there is then we should keep track of them
                //(all groups around us are of oppositePiece, of course!)

                int count = 0, contVal = 0;

                //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
                {
                    if (contVal == 1)
                        continue;

                    if (curBoard[x, y] == oppositePiece)
                    {
                        //This screens for duplicates so one piece removed only adds a max of 1 liberty to each group
                        if (ScreenDuplicates(x, y, count, groupNumbers))
                        {
                            int pos = 0;
                            //This is for groups found from entirely other squares
                            while (groupsAdjacent[pos] != 0)
                            {
                                if (groupNumbers[x, y] == groupsAdjacent[pos]) //We already have the group                                                                                
                                    break;
                                pos++;
                            }

                            //Either we found it, or are on a open group space
                            groupsAdjacent[pos] = groupNumbers[x, y];
                            numberAdjacent[pos]++;
                            xStart[pos] = x;
                            yStart[pos] = y;
                        }
                    }
                    else if (curBoard[x, y] == curPiece)
                    {
                        removeGroupMetaRecurs(curBoard, libertyGroups, groupNumbers, x, y, curPiece, oppositePiece, newPiece, curCount,
                        groupsAdjacent, numberAdjacent, xStart, yStart);
                    }
                }

                return curCount;
            }




            //The game is currently strangely played out due to KeepBranch... so we are going to have to do some
            //guessing to remove dead pieces
            public static void guessRemoveDead(int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, ref int blackCaptured, ref int whiteCaptured)
            {
                //return;
                int x, y;

                bool pieceKilled = true;

                //idk if this is the best idea... but maybe?
                while (pieceKilled)
                {
                    for (x = 0; x < 9; x++)
                        for (y = 0; y < 9; y++)
                        {
                            if (libertyGroups[x, y] == 1 || libertyGroups[x, y] == -1)
                            //Well... I guess we should consider pieces with 1 liberty dead
                            //and actually... as we are at the end we can consider it dead
                            //by playing pieces to kill it (treating this as cleanup)
                            {
                                //This move of course ignores many possibilities, such as repeating captures,
                                //the possibility of MANY 1 PIECES TO CAP AT ONCE, so that you cannot cap them all
                                //and some will get away... but it should be OKAY for the monte carlo simulation

                                pieceKilled = true;

                                int curPiece = curBoard[x, y];
                                int opposite = curPiece == 1 ? 2 : 1;

                                int count = 0, contVal = 0;

                                int xToPlace = -1, yToPlace = -1;

                                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
                                {
                                    if (contVal == 1)
                                        continue;

                                    if (IsEmpty(curBoard, x, y))
                                    {
                                        xToPlace = x;
                                        yToPlace = y;
                                    }
                                }

                                if (xToPlace != -1) //Oh, of course... you can be part of a group with 1 liberty and not have any yourself!
                                    MetaPlayPiece(opposite, curBoard, libertyGroups, groupNumbers, ref curGroupCount,
                                        xToPlace, yToPlace, ref blackCaptured, ref whiteCaptured);
                            }
                        }
                    pieceKilled = false;
                }
            }

            //Should only be called when the board is entirely settled, there is a balance though
            //if this is more powerful less simulate needs to be done, but then also this takes longer                    
            public static void territoryScore(int[,] curBoard, out int blackTerCount, out int whiteTerCount)
            {
                //For now this is super simple, assume that all territory that is owned is entirely surrounded
                int[,] tempNewBoard = new int[9, 9];
                Array.Copy(curBoard, tempNewBoard, 9 * 9);

                //-1 means black territory, -2 means white territory          

                int[,] territoryQueue = new int[9 * 9, 3];
                int startQueue = 0;
                int endQueue = 0;

                blackTerCount = 0;
                whiteTerCount = 0;

                int x, y;

                for (x = 0; x < 9; x++)
                    for (y = 0; y < 9; y++)
                        if (tempNewBoard[x, y] > 0)
                        {
                            splitToQueue(x, y + 1, tempNewBoard, ref endQueue, territoryQueue, -tempNewBoard[x, y], ref blackTerCount, ref whiteTerCount);
                            splitToQueue(x, y - 1, tempNewBoard, ref endQueue, territoryQueue, -tempNewBoard[x, y], ref blackTerCount, ref whiteTerCount);
                            splitToQueue(x + 1, y, tempNewBoard, ref endQueue, territoryQueue, -tempNewBoard[x, y], ref blackTerCount, ref whiteTerCount);
                            splitToQueue(x - 1, y, tempNewBoard, ref endQueue, territoryQueue, -tempNewBoard[x, y], ref blackTerCount, ref whiteTerCount);
                        }

                while (startQueue != endQueue)
                {
                    int pieceValue;
                    removeFromQueue(ref startQueue, territoryQueue, out pieceValue, out x, out y);

                    splitToQueue(x, y + 1, tempNewBoard, ref endQueue, territoryQueue, pieceValue, ref blackTerCount, ref whiteTerCount);
                    splitToQueue(x, y - 1, tempNewBoard, ref endQueue, territoryQueue, pieceValue, ref blackTerCount, ref whiteTerCount);
                    splitToQueue(x + 1, y, tempNewBoard, ref endQueue, territoryQueue, pieceValue, ref blackTerCount, ref whiteTerCount);
                    splitToQueue(x - 1, y, tempNewBoard, ref endQueue, territoryQueue, pieceValue, ref blackTerCount, ref whiteTerCount);

                    startQueue = (startQueue + 1) % (9 * 9); //Go forward, and wrap
                }

                //Go through queue, expanding it when neccessary
            }


            public static void splitToQueue(int x, int y, int[,] curBoard,
                ref int endQueue, int[,] territoryQueue, int pieceNumber, ref int blackTerCount, ref int whiteTerCount)
            {
                //Out of bounds
                if (x < 0 || y < 0 || x >= 9 || y >= 9)
                    return;

                if (curBoard[x, y] == 0)
                {
                    curBoard[x, y] = -1; //MARK IT, so we don't visit it again!
                    addToQueue(x, y, pieceNumber, ref endQueue, territoryQueue);

                    if (pieceNumber == -1)
                        blackTerCount++;
                    else
                        whiteTerCount++;
                }

            }

            public static void addToQueue(int x, int y, int pieceNumber, ref int endQueue, int[,] territoryQueue)
            {
                territoryQueue[endQueue, 0] = pieceNumber;
                territoryQueue[endQueue, 1] = x;
                territoryQueue[endQueue, 2] = y;

                endQueue = (endQueue + 1) % (9 * 9); //Go forward, and wrap
            }

            public static void removeFromQueue(ref int startQueue, int[,] territoryQueue, out int pieceValue, out int x, out int y)
            {
                pieceValue = territoryQueue[startQueue, 0];
                x = territoryQueue[startQueue, 1];
                y = territoryQueue[startQueue, 2];

                startQueue = (startQueue + 1) % (9 * 9); //Go forward, and wrap
            }


            public override void Play(GTPLibrary.Ent_move move)
            {
                int curPiece = move.color.ToString() == "b" ? 1 : 2;

                //board[move.vertex.xPos, move.vertex.yPos] = curPiece;

                int[,] libertyGroups = new int[9, 9]; //|v| = liberty group size, sgn(v) == +, means black, sgn(v) == - means white
                int[,] groupNumbers = new int[9, 9]; //numbered starting at 1, unique number for every group with a max equal to curGroupCount
                int curGroupCount = 0;
                CreateLibertyGroups(board, libertyGroups, groupNumbers, ref curGroupCount);

                MetaPlayPiece(curPiece, board, libertyGroups, groupNumbers, ref curGroupCount, move.vertex.xPos, move.vertex.yPos, ref blackTaken, ref whiteTaken);
            }

            public override void ResetAndSetSize(int size)
            {
                for (int x = 0; x < 9; x++)
                    for (int y = 0; y < 9; y++)
                        board[x, y] = 0;
            }

            public override string final_score()
            {
                throw new NotImplementedException();
            }

            public override void quit()
            {
                //Nothing to do
            }

            public override string name
            {
                get { return "DotNetGo OverOptimized"; }
            }

            public override string showboard()
            {
                string boardText = "";
                //throw new NotImplementedException();
                for (int x = 0; x < 9; x++)
                {
                    for (int y = 0; y < 9; y++)
                    {
                        boardText += board[x, y] == 0 ? "O" : (board[x, y] == 1 ? "b" : "w");
                    }
                    boardText += "\n";
                }
                return boardText;
            }

            public static string showboard(int[,] board)
            {
                return showboard(board, true);
            }
            public static string showboard(int[,] board, bool showBW)
            {
                string boardText = "";
                //throw new NotImplementedException();
                for (int y = 0; y < 9; y++)
                {
                    for (int x = 0; x < 9; x++)
                    {
                        if (board[x, y] == 0)
                            boardText += "O";
                        else if (board[x, y] == 1 && showBW)
                            boardText += "b";
                        else if (board[x, y] == 2 && showBW)
                            boardText += "w";
                        else if (board[x, y] > -10 && board[x, y] < 10)
                            boardText += Math.Abs(board[x, y]).ToString();
                        else
                            boardText += "?";
                    }
                    boardText += "\n";
                }
                return boardText;
            }
            public static string showboard(double[,] board)
            {
                string boardText = "";
                //throw new NotImplementedException();
                for (int y = 0; y < 9; y++)
                {
                    for (int x = 0; x < 9; x++)
                        boardText += Math.Round(board[x, y], 3).ToString() + "  ";
                    boardText += "\n\n";
                }
                return boardText;
            }
        }
    }
}
