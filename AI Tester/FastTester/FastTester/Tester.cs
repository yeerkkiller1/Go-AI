using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GTPLibrary;

namespace FastTester
{
    public static class Tester
    {
        /// <summary>
        /// Outputs it results to the console.
        /// </summary>
        public static void SimpleMontecarloTest(SimpleGoPlayer goPlayer)
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

            TestDotNetGoPlayer.monteCarloCount = monteCarloCount;

            int[,] board = new int[9, 9];
            int[,] libboard = new int[9, 9];
            int[,] groboard = new int[9, 9];
            int curGroupCount = 0;
            int blackCaptured = 0;
            int whiteCaptured = 0;

            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            2, 2, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            6, 2, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            2, 4, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            4, 4, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            3, 5, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            2, 6, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            4, 6, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            6, 6, ref blackCaptured, ref whiteCaptured);

            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            4, 5, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            3, 6, ref blackCaptured, ref whiteCaptured);

            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            3, 2, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            2, 7, ref blackCaptured, ref whiteCaptured);

            TestDotNetGoPlayer.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            5, 6, ref blackCaptured, ref whiteCaptured);
            TestDotNetGoPlayer.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            1, 6, ref blackCaptured, ref whiteCaptured);


            TestDotNetGoPlayer player = new TestDotNetGoPlayer();
            double bestWinRate = 0;
            Ent_vertex vertex = player.GetBest(true, true, board, 1, 0, out bestWinRate);

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

            Console.WriteLine((9 * 9 * TestDotNetGoPlayer.monteCarloCount) / ((speedTestOne / 10000.0) / 1000.0));

            Console.Read();
        

        }
    }
}
