#define DEBUG_CHECKS

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Cloo;
using System.IO;
using Cloo.Bindings;

namespace GTPOpenCLTesting
{
    class Program
    {
        static void Main(string[] args)
        {
            List<ComputeDevice> devices = new List<ComputeDevice>();

            Console.WriteLine(OpenCLNet.OpenCL.NumberOfPlatforms.ToString());
            //Console.Read();
            //List<Platform> platforms = new List<Platform>();
            //for(int x = 0; x < OpenCL.NumberOfPlatforms; x++)            
            //    platforms.Add(OpenCL.GetPlatform(x));

            //CreateContext(platforms[0], platforms[0].QueryDevices(DeviceType.ALL)[0]);

            //BuildAndRunOCLSource(File.ReadAllText("OpenCLFunctions.cl"));

            //ComputeContext context = 
            //    ComputeContext(devices, properties, null, IntPtr.Zero);


            //ComputeProgram test = new ComputeProgram(

            //Test();
            InitializeOpenCL();
            RecurseSpeedTest();

            Console.Read();
        }

        public static void Test()
        {
            string source = File.ReadAllText("MonteCarloSimulate.cl");

            //Choose Device
            ComputePlatform platform = ComputePlatform.Platforms[0];

            ComputeDevice device = platform.QueryDevices()[0];

            ComputeContextPropertyList properties =
                new ComputeContextPropertyList(platform);

            //Setup of stuff on our side
            ComputeContext context = new ComputeContext(ComputeDeviceTypes.All,
                properties, null, IntPtr.Zero);

            //Build the program, which gets us the kernel
            ComputeProgram program = new ComputeProgram(context, source);
            program.Build(null, null, null, IntPtr.Zero);
            //can use notify as the 3rd command... if you want this to be non-blocking

            ComputeKernel kernel = program.CreateKernel("MonteCarloSimulate");


            //Create arguments
            int sideSize = 4096;
            int[] inMatrixA = new int[sideSize * sideSize];
            int[] inMatrixB = new int[sideSize * sideSize];
            int[] outMatrixC = new int[sideSize * sideSize];
            Random random = new Random((int)DateTime.Now.Ticks);

            if (sideSize <= 32)
                for (int y = 0; y < sideSize; y++)
                    for (int x = 0; x < sideSize; x++)
                    {
                        inMatrixA[y * sideSize + x] = random.Next(3);
                        inMatrixB[y * sideSize + x] = random.Next(3);
                        outMatrixC[y * sideSize + x] = 0;
                    }


            ComputeBuffer<int> bufferMatrixA = new ComputeBuffer<int>(context,
                ComputeMemoryFlags.UseHostPointer, inMatrixA);

            ComputeBuffer<int> bufferMatrixB = new ComputeBuffer<int>(context,
                ComputeMemoryFlags.UseHostPointer, inMatrixB);

            ComputeBuffer<int> bufferMatrixC = new ComputeBuffer<int>(context,
                ComputeMemoryFlags.UseHostPointer, outMatrixC);

            long localWorkSize = Math.Min(device.MaxComputeUnits, sideSize);


            //Sets arguments
            kernel.SetMemoryArgument(0, bufferMatrixA);
            kernel.SetMemoryArgument(1, bufferMatrixB);
            kernel.SetMemoryArgument(2, bufferMatrixC);
            kernel.SetLocalArgument(3, sideSize * 2);
            kernel.SetValueArgument<int>(4, sideSize);
            //kernel.SetLocalArgument(1, localWorkSize);            

            string offset = " ";
            for (int x = 0; x < sideSize; x++)
                offset += "  ";

            if (sideSize <= 32)
                for (int y = 0; y < sideSize; y++)
                {
                    Console.Write(offset);
                    for (int x = 0; x < sideSize; x++)
                        Console.Write(inMatrixA[y * sideSize + x] + " ");
                    Console.WriteLine();
                }




            //Runs commands
            ComputeCommandQueue commands = new ComputeCommandQueue(context,
                context.Devices[0], ComputeCommandQueueFlags.None);

            long executionTime = DateTime.Now.Ticks;

            //Execute kernel
            //globalWorkSize in increments of localWorkSize (max of device.MaxComputeUnits or kernel.GetWorkGroupSize())
            commands.Execute(kernel, null,
                new long[] { Math.Min(sideSize, 16), Math.Min(sideSize, 16) },
                new long[] { localWorkSize, 1 }, null);

            //globalWorkSize can be any size
            //localWorkSize product much not be greater than device.MaxComputeUnits
            //and it must not be greater than kernel.GetWorkGroupSize()
            //ESSENTIALLY, the program iterates through globalWorkSize
            //in increments of localWorkSize. Both are multidimensional,
            //but this just saves us the time of doing that
            //(1 dimension can be put to multiple if the max dimension lengths
            //are known very easily with remainder).

            //Also, you should probably use this
            //kernel.GetPreferredWorkGroupSizeMultiple(device);

            commands.Finish();

            commands.ReadFromBuffer(bufferMatrixC, ref outMatrixC, true, null);

            commands.Finish();
            executionTime = DateTime.Now.Ticks - executionTime;


            GC.Collect();
            program.Dispose();

            Console.WriteLine();
            if (sideSize <= 32)
                for (int y = 0; y < sideSize; y++)
                {
                    for (int x = 0; x < sideSize; x++)
                        Console.Write(inMatrixB[y * sideSize + x] + " ");
                    Console.Write(" ");
                    for (int x = 0; x < sideSize; x++)
                        Console.Write(outMatrixC[y * sideSize + x] + " ");

                    Console.WriteLine();
                }

            int testY = random.Next(sideSize);
            int testX = random.Next(sideSize);

            int sum = 0;
            for (int q = 0; q < sideSize; q++)
                sum += inMatrixA[q * sideSize + testX] *
                    inMatrixB[testY * sideSize + q];

            Console.WriteLine(sum == outMatrixC[testY * sideSize + testX]);

            Console.WriteLine(executionTime / 10000.0);

        }

        private static void notify(CLProgramHandle programHandle, IntPtr userDataPtr)
        {            
            Console.WriteLine("Program build notification.");
        }

        public static void InitializeOpenCL()
        {
            string source = File.ReadAllText("MonteCarloSimulate.cl");

            //Choose Device
            ComputePlatform platform = ComputePlatform.Platforms[0];

            openCLDevice = platform.QueryDevices()[0];

            ComputeContextPropertyList properties =
                new ComputeContextPropertyList(platform);

            //Setup of stuff on our side
            openCLContext = new ComputeContext(ComputeDeviceTypes.All,
                properties, null, IntPtr.Zero);

            //Build the program, which gets us the kernel
            openCLProgram = new ComputeProgram(openCLContext, source);
            openCLProgram.Build(null, null, null, IntPtr.Zero);
            //can use notify as the 3rd command... if you want this to be non-blocking

            openCLKernel = openCLProgram.CreateKernel("MonteCarloSimulate");


        }
        static ComputeDevice openCLDevice;

        static ComputeKernel openCLKernel;
        static ComputeContext openCLContext;
        static ComputeProgram openCLProgram;

        //This could probably be highly optimized, but there is no point as it will not be a hot point when we are done
        public static int[] twoDtoOneD(int[,] twoD)
        {
            int[] oneD = new int[9 * 9];
            for(int y= 0 ; y < 9 ;y++)
                for (int x = 0; x < 9; x++)
                {
                    oneD[y * 9 + x] = twoD[x, y];
                }
            return oneD;
        }
        public static void CallOpenCL(int[,] libertyGroups, 
            int[,] groupNumbers, int x, int y, 
            int[] surroundingLibs, ref int emptySurroundings,
            int ourSgn, ref int duplicateGroups)
        {
            //Create arguments
            
            //Does not split yet
            //int[,] libertyGroups, 
            //int[,] groupNumbers, 
            //int x, 
            //int y, 
            //int[] surroundingLibs, 
            //ref int emptySurroundings,            
            //ref int duplicateGroups,
            //int ourSgn, 
            
            //We have to map 2 dimension to 1 dimension            

            //Set arguments
            ComputeBuffer<int> libertyGroupsIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, twoDtoOneD(libertyGroups));
            openCLKernel.SetMemoryArgument(0, libertyGroupsIn);

            ComputeBuffer<int> groupNumbersIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, twoDtoOneD(groupNumbers));
            openCLKernel.SetMemoryArgument(1, groupNumbersIn);

            openCLKernel.SetValueArgument<int>(2, x);
            openCLKernel.SetValueArgument<int>(3, y);
            
            ComputeBuffer<int> surroundingLibsIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, surroundingLibs);
            openCLKernel.SetMemoryArgument(4, surroundingLibsIn);

            int[] emptySurroundRef = new int[1];
            ComputeBuffer<int> emptySurroundRefIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, emptySurroundRef);
            openCLKernel.SetMemoryArgument(5, emptySurroundRefIn);

            int[] duplicateGroupsRef = new int[1];
            ComputeBuffer<int> duplicateGroupsRefIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, duplicateGroupsRef);
            openCLKernel.SetMemoryArgument(6, duplicateGroupsRefIn);

            openCLKernel.SetValueArgument<int>(7, ourSgn);            

            //long localWorkSize = Math.Min(openCLDevice.MaxComputeUnits, sideSize);

            //Display input data

            //Runs commands
            ComputeCommandQueue commands = new ComputeCommandQueue(openCLContext,
                openCLContext.Devices[0], ComputeCommandQueueFlags.None);

            long executionTime = DateTime.Now.Ticks;

            //Execute kernel
            //globalWorkSize in increments of localWorkSize (max of device.MaxComputeUnits or kernel.GetWorkGroupSize())
            commands.Execute(openCLKernel, null,
                new long[] { 1 },
                new long[] { 1 }, null);

            //Also, you should probably use this
            //kernel.GetPreferredWorkGroupSizeMultiple(device);

            commands.Finish();

            //int[] surroundingLibs, 
            //ref int emptySurroundings,            
            //ref int duplicateGroups,
            //Read output data
            commands.ReadFromBuffer(surroundingLibsIn, ref surroundingLibs, true, null);
            commands.ReadFromBuffer(emptySurroundRefIn, ref emptySurroundRef, true, null); emptySurroundings = emptySurroundRef[0];
            commands.ReadFromBuffer(duplicateGroupsRefIn, ref duplicateGroupsRef, true, null); duplicateGroups = duplicateGroupsRef[0];

            //We could set blocking to false on reads and then read them all back in then, we could (possiblity) gain some performance
            //by telling it that commands can be executed out of order and then by queuing them up and calling Finish
            commands.Finish();
            executionTime = DateTime.Now.Ticks - executionTime;


            GC.Collect();
         //   openCLProgram.Dispose();            
            //display output data
            
            //Test are done by our caller now

            Console.WriteLine(executionTime / 10000.0);
        }

        /*
        public static bool OpenCLKeepBranch(int piece, int[,] curBoard,
            int[,] libertyGroups, int[,] groupNumbers,
            ref int curGroupCount, int x, int y)
        {
            //Create arguments

            //Does not split yet
            //int[,] libertyGroups, 
            //int[,] groupNumbers, 
            //int x, 
            //int y, 
            //int[] surroundingLibs, 
            //ref int emptySurroundings,            
            //ref int duplicateGroups,
            //int ourSgn, 

            //We have to map 2 dimension to 1 dimension            

            //Set arguments
            ComputeBuffer<int> libertyGroupsIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, twoDtoOneD(libertyGroups));
            openCLKernel.SetMemoryArgument(0, libertyGroupsIn);

            ComputeBuffer<int> groupNumbersIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, twoDtoOneD(groupNumbers));
            openCLKernel.SetMemoryArgument(1, groupNumbersIn);

            openCLKernel.SetValueArgument<int>(2, x);
            openCLKernel.SetValueArgument<int>(3, y);

            ComputeBuffer<int> surroundingLibsIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, surroundingLibs);
            openCLKernel.SetMemoryArgument(4, surroundingLibsIn);

            int[] emptySurroundRef = new int[1];
            ComputeBuffer<int> emptySurroundRefIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, emptySurroundRef);
            openCLKernel.SetMemoryArgument(5, emptySurroundRefIn);

            int[] duplicateGroupsRef = new int[1];
            ComputeBuffer<int> duplicateGroupsRefIn = new ComputeBuffer<int>(openCLContext, ComputeMemoryFlags.UseHostPointer, duplicateGroupsRef);
            openCLKernel.SetMemoryArgument(6, duplicateGroupsRefIn);

            openCLKernel.SetValueArgument<int>(7, ourSgn);

            //long localWorkSize = Math.Min(openCLDevice.MaxComputeUnits, sideSize);

            //Display input data

            //Runs commands
            ComputeCommandQueue commands = new ComputeCommandQueue(openCLContext,
                openCLContext.Devices[0], ComputeCommandQueueFlags.None);

            long executionTime = DateTime.Now.Ticks;

            //Execute kernel
            //globalWorkSize in increments of localWorkSize (max of device.MaxComputeUnits or kernel.GetWorkGroupSize())
            commands.Execute(openCLKernel, null,
                new long[] { 1 },
                new long[] { 1 }, null);

            //Also, you should probably use this
            //kernel.GetPreferredWorkGroupSizeMultiple(device);

            commands.Finish();

            //int[] surroundingLibs, 
            //ref int emptySurroundings,            
            //ref int duplicateGroups,
            //Read output data
            commands.ReadFromBuffer(surroundingLibsIn, ref surroundingLibs, true, null);
            commands.ReadFromBuffer(emptySurroundRefIn, ref emptySurroundRef, true, null); emptySurroundings = emptySurroundRef[0];
            commands.ReadFromBuffer(duplicateGroupsRefIn, ref duplicateGroupsRef, true, null); duplicateGroups = duplicateGroupsRef[0];

            //We could set blocking to false on reads and then read them all back in then, we could (possiblity) gain some performance
            //by telling it that commands can be executed out of order and then by queuing them up and calling Finish
            commands.Finish();
            executionTime = DateTime.Now.Ticks - executionTime;


            GC.Collect();
            //   openCLProgram.Dispose();            
            //display output data

            //Test are done by our caller now

            Console.WriteLine(executionTime / 10000.0);
        }

*/
        static void RecurseSpeedTest()
        {
            int boardSampleCount = 1;
            int monteCarloCount = 1;
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


            localOpenCL.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            2, 2, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            6, 2, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            2, 4, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            4, 4, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            3, 5, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            2, 6, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            4, 6, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            6, 6, ref blackCaptured, ref whiteCaptured);

            localOpenCL.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            4, 5, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            3, 6, ref blackCaptured, ref whiteCaptured);

            localOpenCL.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            3, 2, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            2, 7, ref blackCaptured, ref whiteCaptured);

            localOpenCL.MetaPlayPiece(1, board, libboard, groboard, ref curGroupCount,
                            5, 6, ref blackCaptured, ref whiteCaptured);
            localOpenCL.MetaPlayPiece(2, board, libboard, groboard, ref curGroupCount,
                            1, 6, ref blackCaptured, ref whiteCaptured);


            localOpenCL player = new localOpenCL();
            double bestWinRate = 0;
            //Ent_vertex vertex = player.GetBest(true, true, board, 1, 0, out bestWinRate);
            
            for (int x = 0; x < boardSampleCount; x++)
            {
                for (int j = 0; j < 9; j++)
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

                        if (!localOpenCL.KeepBranch(1, cboard, clibboard, cgroboard,
                            ref curGroupCount, j, k))
                        {
                            boardRates[j, k] = -100;
                            continue;
                        }


                        localOpenCL.MetaPlayPiece(1, cboard, clibboard, cgroboard, ref ccurGroupCount,
                            j, k, ref cblackCaptured, ref cwhiteCaptured);
                        boardRates[j, k] += localOpenCL.MonteCarloForBlackMetadata(false,
                            cboard, 0, 0, monteCarloCount, ref averageScore);

                        //boardRates[j, k] = averageScore;

                        boardRates[j, k] *= localOpenCL.KeepBranch(1, board, libboard,
                            groboard, ref curGroupCount, j, k) ? 1 : -1;
                    }
            }
            for (int j = 0; j < 9; j++)
                for (int k = 0; k < 9; k++)
                    boardRates[j, k] /= boardSampleCount;

            //averagePlainBoard += localOpenCL.MonteCarloForBlack(true, boardSamples[x], 0, 0);
            //averagePlainBoard /= boardSampleCount;

            speedTestOne = DateTime.Now.Ticks - speedTestOne;

        }
    }

    public class localOpenCL
    {
        static double monteCarloPassAmount = 0;
        static double monteCarloNotPassAmount = 0;

        public static double komi = 0; //SHOULD NOT BE STATIC

        public static Random random = new Random();

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
            while (curBoard[xPlace, yPlace] != 0 || !KeepBranch(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, xPlace, yPlace))
            {
                xPlace = random.Next(9);
                yPlace = random.Next(9);
            }

            curBoard[xPlace, yPlace] = curPiece == 1 ? 9 : 8; //Just for debugging, doesn't hurt anything though

            //PLACE PIECE                
            MetaPlayPiece(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, xPlace, yPlace, ref blackCaptured, ref whiteCaptured);


            return SimMetadataRecurse(!curBlack, curBoard, libertyGroups, groupNumbers, ref curGroupCount, curBlackNetCapture, ref blackCaptured, ref whiteCaptured);
        }

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
            bool oldVal = OldKeepBranch(piece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y);
            return oldVal;
        }
        public static bool OldKeepBranch(int piece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, int x, int y)
        {
            int oppositePiece = piece == 1 ? 2 : 1;
            int ourSgn = piece == 1 ? 1 : -1;

            int emptySurroundings = 0;

            int duplicateGroups = 0;

            //Add up adjacent liberty groups
            int[] surroundingLibs = new int[4];

            if (curBoard[x, y] <= int.MinValue + 100)//Already capped, either super secure or certain death (usually)                
                return false;

            FillInSurroundingLibsandEmpty1(libertyGroups, groupNumbers, x, y, surroundingLibs, ref emptySurroundings, ourSgn, ref duplicateGroups); //emptySurroundings = 0;

            int[] testSurLibs = new int[4];
            int testEmpty = 0;
            int testDup = 0;
            Program.CallOpenCL(libertyGroups, groupNumbers, x, y, testSurLibs, ref testEmpty, ourSgn, ref testDup);

#if DEBUG_CHECKS
            for (int a = 0; a < 4; a++)
                if (testSurLibs[a] != surroundingLibs[a])
                    throw new Exception("openCL code different surrounding liberties");

            if(testEmpty != emptySurroundings)
                throw new Exception("openCL code different empty surrounding spaces");

            if(testDup != duplicateGroups)
                throw new Exception("openCL code different duplicate groups");
#endif

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

        public static void MetaPlayPiece(int curPiece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, int x, int y, ref int blackCaptured, ref int whiteCaptured)
        {
#if DEBUG_CHECKS
                if (curBoard[x,y] != 9 && curBoard[x,y] != 8 && !IsEmpty(curBoard, x, y)) //can't place on full squares
                {
                throw new Exception("can't place on full squares");
                }
#endif

            int[,] originalBoardState2 = new int[9, 9];
            Array.Copy(curBoard, originalBoardState2, 9 * 9);

            int[,] originalBoardLib2 = new int[9, 9];
            Array.Copy(libertyGroups, originalBoardLib2, 9 * 9);

            int[,] originalBoardGroup2 = new int[9, 9];
            Array.Copy(groupNumbers, originalBoardGroup2, 9 * 9);

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

            KeepBranch(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y);

            int count = 0, contVal = 0;

            //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
            while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
            {
                if (contVal == 1)
                    continue;
                if (libertyGroups[x, y] * ourSgn == -1) //It has no more liberties and so it dies now
                {
                    int takenPiece = curBoard[x, y] == 1 ? int.MinValue + 1 : int.MinValue;

                    //Remove other group
                    capped += removeGroupMeta(curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y, oppositePiece, curPiece, takenPiece);
                    emptySurrounding++; //Now there is an empty here
                }
                else if (libertyGroups[x, y] * ourSgn < 0) //We just decrement its number of liberties, unless of course it borders us more than once
                {
                    if (ScreenDuplicates(x, y, count, groupNumbers))
                        SetGroupNumber(libertyGroups, x, y, libertyGroups[x, y] + ourSgn, libertyGroups[x, y]);
                }
                else if (libertyGroups[x, y] * ourSgn > 0) //Add it to our new liberty grouping
                {
                    if (ScreenDuplicates(x, y, count, groupNumbers))
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
            }

            //count = 0; contVal = 0; these should naturally reset in PlayLoop

            //We also need to check for liberty overlap on the groups we are combining   

            int combineOverlapAmount = 0;
            //Remove our emptySurroundings that are extra
            if (curCombineGroups == 1)
            {
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0) //Around cur piece
                {
                    if (contVal == 1)
                        continue;
                    bool libertyAccountedFor = false;
                    int subX = x, subY = y, subCount = 0;

                    //Could also just check libertyGroups
                    if (IsEmpty(curBoard, x, y)) //All liberties we will get                    
                        //We will recycle contVal, as it is only really used right after it is set
                        while ((contVal = PlayLoop(ref subX, ref subY, ref subCount)) != 0) //Everything around liberty
                        {
                            if (contVal == 1 || libertyAccountedFor)
                                continue;

                            //If we will be sharing a liberty with this group, and it is the same type as us
                            if (curBoard[subX, subY] == curPiece) //libertyGroups[x, y] * ourSgn > 0                            
                                for (int b = 0; b < curCombineGroups; b++) //If it is beside a group we will add to us anyway, then don't add it twice                                     
                                    if (groupNumbers[xGroups[b], yGroups[b]] == groupNumbers[subX, subY])
                                    {
                                        libertyAccountedFor = true;
                                        break;
                                    }
                        }

                    if (libertyAccountedFor)
                        emptySurrounding--;
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

            if ((newGroupLiberties == 0 || newGroupLiberties == 1) && capped == 0) //Uh... that was a dumb move?
            {
                throw new Exception("bad move");


                //curBoard[x, y] = 9;
                //int temp = 0;
                ////CreateLibertyGroups(curBoard, libertyGroups, curBoard, ref temp);
                ////Actually... this will be triggered when we cap a move, but are then in a bad place
                //curBoard[x, y] = 0;

                //MetaPlayPiece(curPiece, originalBoardState2, originalBoardLib2, originalBoardGroup2, ref curGroupCount, x, y, ref blackCaptured, ref whiteCaptured);

                //KeepBranch(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y);

                //showboard(curBoard);
                ////MetaPlayPiece(curPiece, originalBoardState, originalBoardLib, originalBoardGroup, ref curGroupCount, x, y, ref blackCaptured, ref whiteCaptured);
                //KeepBranch(curPiece, curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y);
            }

            curBoard[x, y] = curPiece;

#if DEBUG_CHECKS
                //tempemptylibcheck(curBoard, libertyGroups, groupNumbers, ref curGroupCount);
#endif
            //MetaPlayPiece(curPiece, originalBoardState, originalBoardLib, originalBoardGroup, ref curGroupCount, x, y, ref blackCaptured, ref whiteCaptured);

            if (capped > 0)
            {
                if (oppositePiece == 1) //We capture opposite pieces when we play!
                    blackCaptured += capped;
                else
                    whiteCaptured += capped;

                //MetaPlayPiece(curPiece, originalBoardState2, originalBoardLib2, originalBoardGroup2, ref curGroupCount, x, y, ref blackCaptured, ref whiteCaptured);
            }


#if DEBUG_CHECKS

                int[,] originalBoardState = new int[9, 9];
                Array.Copy(curBoard, originalBoardState, 9 * 9);

                int[,] originalBoardLib = new int[9, 9];
                Array.Copy(libertyGroups, originalBoardLib, 9 * 9);

                int[,] originalBoardGroup = new int[9, 9];
                Array.Copy(groupNumbers, originalBoardGroup, 9 * 9);               

                //MetaPlayPiece(curPiece, originalBoardState, originalBoardLib, originalBoardGroup, ref curGroupCount, x, y, ref blackCaptured, ref whiteCaptured);

                int groupCount = 0;
                CreateLibertyGroups(originalBoardState, originalBoardLib, originalBoardGroup, ref groupCount);

                for (int xp = 0; xp < 9; xp++)
                    for (int yp = 0; yp < 9; yp++)
                    {
                        if (libertyGroups[xp, yp] == 0 && !IsEmpty(curBoard, xp, yp)) //If it has 0 liberties it should be empty?
                        {
                throw new Exception("liberty disconnect");
                            //Although we might be given an initial board state where it is, but I guess we just need to catch that early
                        }
                        if (libertyGroups[xp, yp] != 0 && IsEmpty(curBoard, xp, yp)) //If it is empty is should have 0 liberties
                        {
                throw new Exception("liberty disconnect");
                        }
                        if (libertyGroups[xp, yp] != originalBoardLib[xp, yp]) //Not good, our liberty groups are wrong
                        {
                throw new Exception("liberty disconnect");
                            curBoard[x, y] = 9;
                            int temp = 0;
                            //CreateLibertyGroups(curBoard, libertyGroups, groupNumbers, ref temp);


                            MetaPlayPiece(curPiece, originalBoardState2, originalBoardLib2, originalBoardGroup2, ref curGroupCount, x, y, ref blackCaptured, ref whiteCaptured);
                            
                        }
                    }

#endif
        }
        public static int CheckNewLibertyCount(int curPiece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers,
            int curGroupCount, int x, int y)
        {
#if DEBUG_CHECKS                
                int[,] ccurBoard = new int[9, 9]; int[,] clibertyGroups = new int[9, 9]; int[,] cgroupNumbers = new int[9, 9];
                Array.Copy(curBoard, ccurBoard, 9 * 9); Array.Copy(libertyGroups, clibertyGroups, 9 * 9); Array.Copy(groupNumbers, cgroupNumbers, 9 * 9);
                curBoard = ccurBoard; libertyGroups = clibertyGroups; groupNumbers = cgroupNumbers;
#endif

            //We will try to do this without changing the source data and without copying it... but we may have to
            //to do some calculations for now. In the future all copy operations should be scrapper (as there
            //is theoretically a faster way, at least I think there is).
            int ourSgn = curPiece == 1 ? 1 : -1;

            int oppositePiece = curPiece == 1 ? 2 : 1;

            int capped = 0;
            int newGroupLiberties = 0;
            //Recalculate adjacent liberty groups

            //The adjacent groups which needs to be combined
            int[] xGroups = { -1, -1, -1, -1 };
            int[] yGroups = { -1, -1, -1, -1 };
            int curCombineGroups = 0;

            int emptySurrounding = 0;

            int count = 0, contVal = 0;

            //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
            while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
            {
                if (contVal == 1)
                    continue;
                if (libertyGroups[x, y] * ourSgn == -1) //It has no more liberties and so it dies now
                {
                    int takenPiece = curBoard[x, y] == 1 ? int.MinValue + 1 : int.MinValue;

                    //COPY OPERATION (this should buffer the caller from any changes we make, but of course it is less efficient       
#if !DEBUG_CHECKS
                    int[,] ccurBoard = new int[9, 9]; int[,] clibertyGroups = new int[9, 9]; int[,] cgroupNumbers = new int[9, 9];
#endif
                    Array.Copy(curBoard, ccurBoard, 9 * 9); Array.Copy(libertyGroups, clibertyGroups, 9 * 9); Array.Copy(groupNumbers, cgroupNumbers, 9 * 9);
                    curBoard = ccurBoard; libertyGroups = clibertyGroups; groupNumbers = cgroupNumbers;

                    //Remove other group
                    capped += removeGroupMeta(curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y, oppositePiece, curPiece, takenPiece);
                    emptySurrounding++; //Now there is an empty here
                }
                else if (libertyGroups[x, y] * ourSgn < 0) //We just decrement its number of liberties, unless of course it borders us more than once
                {
                    //not needed
                    //if (ScreenDuplicates(x, y, count, groupNumbers))
                    //    SetGroupNumber(libertyGroups, x, y, libertyGroups[x, y] + ourSgn, libertyGroups[x, y]);
                }
                else if (libertyGroups[x, y] * ourSgn > 0) //Add it to our new liberty grouping
                {
                    if (ScreenDuplicates(x, y, count, groupNumbers))
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
            }

            //count = 0; contVal = 0; these should naturally reset in PlayLoop

            //We also need to check for liberty overlap on the groups we are combining   

            int combineOverlapAmount = 0;
            //Remove our emptySurroundings that are extra
            if (curCombineGroups == 1)
            {
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0) //Around cur piece
                {
                    if (contVal == 1)
                        continue;
                    bool libertyAccountedFor = false;
                    int subX = x, subY = y, subCount = 0;

                    //Could also just check libertyGroups
                    if (IsEmpty(curBoard, x, y)) //All liberties we will get                    
                        //We will recycle contVal, as it is only really used right after it is set
                        while ((contVal = PlayLoop(ref subX, ref subY, ref subCount)) != 0) //Everything around liberty
                        {
                            if (contVal == 1 || libertyAccountedFor)
                                continue;

                            //If we will be sharing a liberty with this group, and it is the same type as us
                            if (curBoard[subX, subY] == curPiece) //libertyGroups[x, y] * ourSgn > 0                            
                                for (int b = 0; b < curCombineGroups; b++) //If it is beside a group we will add to us anyway, then don't add it twice                                     
                                    if (groupNumbers[xGroups[b], yGroups[b]] == groupNumbers[subX, subY])
                                    {
                                        libertyAccountedFor = true;
                                        break;
                                    }
                        }

                    if (libertyAccountedFor)
                        emptySurrounding--;
                }
            }
            //If there are more than 2 liberty groups, it is probably just faster to just combine the group
            //and recalculate all of its liberties from scratch (faster though, cause it is just one group)
            else if (curCombineGroups >= 2)
            {
                //Not sure whether making a metadata map or just linked arrays is faster?
                //(data static, position data implicit VS data dynamic, position data stored)
                //also, static positioning allows much faster searching

                int prevPiece = curBoard[x, y]; //So we can undo it
                curBoard[x, y] = curPiece; //Place the piece

                //It may be possible to make a metaMap and then
                int[,] newMetaMap = new int[9, 9];
                Array.Copy(curBoard, newMetaMap, 9 * 9);
                newGroupLiberties = RecurseMarkLibs(curPiece, newMetaMap, x, y); //Fortunately, this function doesn't change anything anyway

                curBoard[x, y] = prevPiece;



#if DEBUG_CHECKS //Insures we do not corrupt the given data
                for (int k = 0; k < 9; k++)
                    for (int j = 0; j < 9; j++)
                    {
                        if (curBoard[k, j] != ccurBoard[k, j] ||
                            libertyGroups[k, j] != clibertyGroups[k, j] ||
                            groupNumbers[k, j] != cgroupNumbers[k, j])
                        {
                            //Bad, we changed a value in CheckNewLiberyCount which should never happen
                        }
                    }
#endif

                return newGroupLiberties * ourSgn;
            }

            if (curCombineGroups < 2) //We redo everything if it is 2 or more
            {
                //Take all of the groups and combine them, now that they liberties are correct as removed groups have been accounted for
                for (int b = 0; b < curCombineGroups; b++)
                    newGroupLiberties += libertyGroups[xGroups[b], yGroups[b]];

                newGroupLiberties -= curCombineGroups * ourSgn; //Subtract as we are losing 1 liberty per group we touch
                newGroupLiberties -= combineOverlapAmount * ourSgn;
                newGroupLiberties += emptySurrounding * ourSgn;


#if DEBUG_CHECKS //Insures we do not corrupt the given data
                for (int k = 0; k < 9; k++)
                    for (int j = 0; j < 9; j++)
                    {
                        if (curBoard[k, j] != ccurBoard[k, j] ||
                            libertyGroups[k, j] != clibertyGroups[k, j] ||
                            groupNumbers[k, j] != cgroupNumbers[k, j])
                        {
                            //Bad, we changed a value in CheckNewLiberyCount which should never happen
                        }
                    }
#endif

                return newGroupLiberties;
            }

            throw new Exception("this point cannot be reached (or maybe it can)");
        }

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


    }

    public class exportedOpenCL
    {        
        public static void FillInSurroundingLibsandEmpty(int[,] libertyGroups, int[,] groupNumbers, int x, int y, 
            int[] surroundingLibs, ref int emptySurroundings,
            int ourSgn, ref int duplicateGroups)
        {
            int[] groupsAround = { 0, 0, 0, 0 }; //This could probably be put in shared memory or something, 
            //but not now the bulk initialization is just so much faster, so it will be left like this                                                

            int tempPos = 0;
            int groupsAroundCount = 0;

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

        /*
        public static bool KeepBranch(int piece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers, ref int curGroupCount, int x, int y)
        {
            int oppositePiece = piece == 1 ? 2 : 1;
            int ourSgn = piece == 1 ? 1 : -1;

            int emptySurroundings = 0;

            int duplicateGroups = 0;

            //Add up adjacent liberty groups
            int[] surroundingLibs = new int[4];

            if (curBoard[x, y] <= int.MinValue + 100)//Already capped, either super secure or certain death (usually)                
                return false;

            FillInSurroundingLibsandEmpty(
            libertyGroups,
            groupNumbers,
            x,
            y,
            surroundingLibs,
            ref emptySurroundings,
            ourSgn, ref duplicateGroups
            );

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
                            //This is a hot point, no duplicate groups but about 10% we don't have enough liberties

                            int enemyGroups = 0, friendlyGroups = 0;
                            for (int m = 0; m < 4; m++)
                            {
                                if (surroundingLibs[m] * ourSgn > 0)
                                    enemyGroups++;
                                else if (surroundingLibs[m] * ourSgn > 0)
                                    friendlyGroups++;
                            }

                            if (newLibs * ourSgn <= 1)
                                return false;

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
        //Requires CheckNewLibertyCount
                    //PlayLoop
                    //ScreenDuplicates
                    //removeGroupMeta
                        //removeGroupMetaRecurs
                        //(PlayLoop)                    
                    //IsEmpty
                    //RecurseMarkLibs (and recursive
                        //(PlayLoop)

        public static int CheckNewLibertyCount(int curPiece, int[,] curBoard, int[,] libertyGroups, int[,] groupNumbers,
            int curGroupCount, int x, int y)
        {
#if DEBUG_CHECKS
            int[,] ccurBoard = new int[9, 9]; int[,] clibertyGroups = new int[9, 9]; int[,] cgroupNumbers = new int[9, 9];
            Array.Copy(curBoard, ccurBoard, 9 * 9); Array.Copy(libertyGroups, clibertyGroups, 9 * 9); Array.Copy(groupNumbers, cgroupNumbers, 9 * 9);
            curBoard = ccurBoard; libertyGroups = clibertyGroups; groupNumbers = cgroupNumbers;
#endif

            //We will try to do this without changing the source data and without copying it... but we may have to
            //to do some calculations for now. In the future all copy operations should be scrapper (as there
            //is theoretically a faster way, at least I think there is).
            int ourSgn = curPiece == 1 ? 1 : -1;

            int oppositePiece = curPiece == 1 ? 2 : 1;

            int capped = 0;
            int newGroupLiberties = 0;
            //Recalculate adjacent liberty groups

            //The adjacent groups which needs to be combined
            int[] xGroups = { -1, -1, -1, -1 };
            int[] yGroups = { -1, -1, -1, -1 };
            int curCombineGroups = 0;

            int emptySurrounding = 0;

            int count = 0, contVal = 0;

            //Remove enemy liberties and kill enemy groups (recalculating affected liberties)   
            while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0)
            {
                if (contVal == 1)
                    continue;
                if (libertyGroups[x, y] * ourSgn == -1) //It has no more liberties and so it dies now
                {
                    int takenPiece = curBoard[x, y] == 1 ? int.MinValue + 1 : int.MinValue;

                    //COPY OPERATION (this should buffer the caller from any changes we make, but of course it is less efficient       
#if !DEBUG_CHECKS
                    int[,] ccurBoard = new int[9, 9]; int[,] clibertyGroups = new int[9, 9]; int[,] cgroupNumbers = new int[9, 9];
#endif
                    Array.Copy(curBoard, ccurBoard, 9 * 9); Array.Copy(libertyGroups, clibertyGroups, 9 * 9); Array.Copy(groupNumbers, cgroupNumbers, 9 * 9);
                    curBoard = ccurBoard; libertyGroups = clibertyGroups; groupNumbers = cgroupNumbers;

                    //Remove other group
                    capped += removeGroupMeta(curBoard, libertyGroups, groupNumbers, ref curGroupCount, x, y, oppositePiece, curPiece, takenPiece);
                    emptySurrounding++; //Now there is an empty here
                }
                else if (libertyGroups[x, y] * ourSgn < 0) //We just decrement its number of liberties, unless of course it borders us more than once
                {
                    //not needed
                    //if (ScreenDuplicates(x, y, count, groupNumbers))
                    //    SetGroupNumber(libertyGroups, x, y, libertyGroups[x, y] + ourSgn, libertyGroups[x, y]);
                }
                else if (libertyGroups[x, y] * ourSgn > 0) //Add it to our new liberty grouping
                {
                    if (ScreenDuplicates(x, y, count, groupNumbers))
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
            }

            //count = 0; contVal = 0; these should naturally reset in PlayLoop

            //We also need to check for liberty overlap on the groups we are combining   

            int combineOverlapAmount = 0;
            //Remove our emptySurroundings that are extra
            if (curCombineGroups == 1)
            {
                while ((contVal = PlayLoop(ref x, ref y, ref count)) != 0) //Around cur piece
                {
                    if (contVal == 1)
                        continue;
                    bool libertyAccountedFor = false;
                    int subX = x, subY = y, subCount = 0;

                    //Could also just check libertyGroups
                    if (IsEmpty(curBoard, x, y)) //All liberties we will get                    
                        //We will recycle contVal, as it is only really used right after it is set
                        while ((contVal = PlayLoop(ref subX, ref subY, ref subCount)) != 0) //Everything around liberty
                        {
                            if (contVal == 1 || libertyAccountedFor)
                                continue;

                            //If we will be sharing a liberty with this group, and it is the same type as us
                            if (curBoard[subX, subY] == curPiece) //libertyGroups[x, y] * ourSgn > 0                            
                                for (int b = 0; b < curCombineGroups; b++) //If it is beside a group we will add to us anyway, then don't add it twice                                     
                                    if (groupNumbers[xGroups[b], yGroups[b]] == groupNumbers[subX, subY])
                                    {
                                        libertyAccountedFor = true;
                                        break;
                                    }
                        }

                    if (libertyAccountedFor)
                        emptySurrounding--;
                }
            }
            //If there are more than 2 liberty groups, it is probably just faster to just combine the group
            //and recalculate all of its liberties from scratch (faster though, cause it is just one group)
            else if (curCombineGroups >= 2)
            {
                //Not sure whether making a metadata map or just linked arrays is faster?
                //(data static, position data implicit VS data dynamic, position data stored)
                //also, static positioning allows much faster searching

                int prevPiece = curBoard[x, y]; //So we can undo it
                curBoard[x, y] = curPiece; //Place the piece

                //It may be possible to make a metaMap and then
                int[,] newMetaMap = new int[9, 9];
                Array.Copy(curBoard, newMetaMap, 9 * 9);
                newGroupLiberties = RecurseMarkLibs(curPiece, newMetaMap, x, y); //Fortunately, this function doesn't change anything anyway

                curBoard[x, y] = prevPiece;



#if DEBUG_CHECKS //Insures we do not corrupt the given data
                for (int k = 0; k < 9; k++)
                    for (int j = 0; j < 9; j++)
                    {
                        if (curBoard[k, j] != ccurBoard[k, j] ||
                            libertyGroups[k, j] != clibertyGroups[k, j] ||
                            groupNumbers[k, j] != cgroupNumbers[k, j])
                        {
                            //Bad, we changed a value in CheckNewLiberyCount which should never happen
                        }
                    }
#endif

                return newGroupLiberties * ourSgn;
            }

            if (curCombineGroups < 2) //We redo everything if it is 2 or more
            {
                //Take all of the groups and combine them, now that they liberties are correct as removed groups have been accounted for
                for (int b = 0; b < curCombineGroups; b++)
                    newGroupLiberties += libertyGroups[xGroups[b], yGroups[b]];

                newGroupLiberties -= curCombineGroups * ourSgn; //Subtract as we are losing 1 liberty per group we touch
                newGroupLiberties -= combineOverlapAmount * ourSgn;
                newGroupLiberties += emptySurrounding * ourSgn;


#if DEBUG_CHECKS //Insures we do not corrupt the given data
                for (int k = 0; k < 9; k++)
                    for (int j = 0; j < 9; j++)
                    {
                        if (curBoard[k, j] != ccurBoard[k, j] ||
                            libertyGroups[k, j] != clibertyGroups[k, j] ||
                            groupNumbers[k, j] != cgroupNumbers[k, j])
                        {
                            //Bad, we changed a value in CheckNewLiberyCount which should never happen
                        }
                    }
#endif

                return newGroupLiberties;
            }

            throw new Exception("this point cannot be reached (or maybe it can)");
        }
        */
    }
}
