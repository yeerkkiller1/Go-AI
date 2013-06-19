using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Threading;

using CommandLineHelpers;

namespace BuildAI
{
    class Program
    {
        static FileRecords fileRecords;
        static string DoBuild(string compilerArgs, string buildArgs)
        {
            Process buildProcess = Hlp.MakeCmdProcess(buildDirectory);

            //RedirectProcessToScreen(buildProcess);

            buildProcess.StandardInput.WriteLine(
 "\"" + clBuildPath + "\" " + compilerArgs);

            buildProcess.StandardInput.WriteLine("cl " + buildArgs);

            buildProcess.StandardInput.WriteLine("exit");

            string buildOutput =  buildProcess.StandardOutput.ReadToEnd();            

            return buildOutput;
        }

        
        static string userName = "quentin";

        static string maintainProjectPath = Hlp.Local(@"C:\Users\"+userName+@"\Dropbox\Programming\MaintainProject\MaintainProject\bin\Debug\MaintainProject.exe");
        static string clBuildPath = Hlp.Local(@"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat");
        static string openCLBuildPath = Hlp.Local(@"C:\Users\" + userName + @"\Dropbox\Go AI\New C++ AI Build\OpenCL Compiler\Debug\OpenCLC2.exe");

        //Src directory
        static string sourceDirectory = Hlp.Local(@"C:\Users\" + userName + @"\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\");
        static string buildDirectory = Hlp.Local(@"C:\Users\" + userName + @"\Dropbox\Go AI\AIs\TestCPlusPlusAI\Debug\");

        static string[] args;
        static bool ArgsContains(string arg)
        {
            return args.Contains(arg) || args.Contains("-" + arg) || args.Contains("--" + arg);
        }

        //Quotes the input, does not insure it has not already been quoted
        static string Quote(string input)
        {
            return "\"" + input + "\"";
        }

        //Gets all parts surrouned by start and end
        static List<string> GetSpecialParts(string input, bool invertSpecialParts)
        {
            return GetSpecialParts(input, invertSpecialParts, '{', '}');
        }
        static List<string> GetSpecialParts(string input, bool invertSpecialParts, char start, char end)
        {
            List<string> specialParts = new List<string>();

            string curSpecialPart = "";
            bool inSpecialPart = false;

            foreach (char ch in input)
            {
                if (ch == start)
                {
                    if (!invertSpecialParts && inSpecialPart)
                        throw new Exception("Special part nested in GetSpecialParts call. Only call this if you control the output!");

                    inSpecialPart = !inSpecialPart;
                    if (invertSpecialParts)
                    {
                        specialParts.Add(curSpecialPart);
                        curSpecialPart = "";
                    }
                }
                else if (ch == end)
                {
                    if (!invertSpecialParts && !inSpecialPart)
                        throw new Exception("Too many end braces in GetSpecialParts call. Only call this if you control the output!");

                    inSpecialPart = !inSpecialPart;

                    specialParts.Add(curSpecialPart);
                    curSpecialPart = "";
                }
                else if (inSpecialPart != invertSpecialParts)
                {
                    curSpecialPart += ch;
                }
            }

            if (invertSpecialParts)
            {
                specialParts.Add(curSpecialPart);
                curSpecialPart = "";
            }
            return specialParts;
        }

        static string lastChangesMade = "";
        static void Main(string[] _args)
        {
            args = _args;
            fileRecords = new FileRecords(Hlp.Local(@"C:\Users\" + userName + @"\Dropbox\Go AI\BuildAI\BuildAI\OptimizationHistory.txt"));
            while (true)
            {
                Console.WriteLine("Enter changes made and hit enter...");
                string userInputTMP = Console.ReadLine();
                if (userInputTMP != "")
                    lastChangesMade = userInputTMP;


                Console.WriteLine("");
                Console.WriteLine("---------------------------------------");
                Console.WriteLine("            Starting Tests");
                Console.WriteLine("---------------------------------------");
                Console.WriteLine("");
                RunAllTests();
                Console.WriteLine("");
                Console.WriteLine("---------------------------------------");
                Console.WriteLine("              Done Tests");
                Console.WriteLine("---------------------------------------");
                Console.WriteLine("");
            }
        }

        static void RunAllTests()
        {
            string newFiles = Hlp.RunProcess(maintainProjectPath, "\"" + sourceDirectory + "\"", sourceDirectory);

            if (newFiles != "")
            {
                Console.WriteLine("Project files removed or added:");
                Hlp.PrintWithIndent(newFiles, 4);

                if (Hlp.GetUserResponse("UPDATE or IGNORE new files?", "UPDATE", "IGNORE") == "UPDATE")
                    Hlp.RunProcess(maintainProjectPath, "!UPDATE! \"" + sourceDirectory + "\"", sourceDirectory);
                else
                    Hlp.RunProcess(maintainProjectPath, "!IGNORE! \"" + sourceDirectory + "\"", sourceDirectory);
            }

            string[] includedFiles = File.ReadAllLines(sourceDirectory + Directory.GetParent(sourceDirectory).Name + ".included");

            Hlp.Local(includedFiles);

            if (includedFiles.Length < 1 || !includedFiles[0].EndsWith("EntryPoint.cpp"))
            {
                Console.WriteLine("EntryPoint.cpp must be the first item in your .include file (in " + sourceDirectory + ").");
                if (includedFiles.Length < 1)
                    Console.WriteLine("Your .include file is empty or does not exist! (Either MaintainProject.exe is broken or you have no .cpps in " + sourceDirectory + ".");
                else
                    Console.WriteLine("The first file is " + includedFiles[0] + ".");
            }

            string includeFileParameters = "/Tc \"" + String.Join("\" /Tc \"", includedFiles) + "\"";

            string combineFileParameters = "\"" + String.Join("\" \"", includedFiles) + "\" ";

            //BuildRealOpenCL(combineFileParameters);

            using (CheckPoint checkPoint = new CheckPoint("Optimized build"))
                BuildSource(includedFiles, includeFileParameters, "/Ox /Ob2", "Optimized");

            //string combineFileParameters;
            using (CheckPoint checkPoint = new CheckPoint("Combine source"))
                CombineSource(includedFiles, out combineFileParameters);

            using (CheckPoint checkPoint = new CheckPoint("Build combined"))
                BuildCombinedSource(includedFiles, combineFileParameters);

            //using (CheckPoint checkPoint = new CheckPoint("Build and run test OpenCL"))
                //BuildAndTestOpenCL(combineFileParameters);

            //using (CheckPoint checkPoint = new CheckPoint("Build real OpenCL"))
                //BuildRealOpenCL(combineFileParameters);

            Console.WriteLine("\n BUILDS SUCCEEDED! BEGINNING VERIFICATION! \n");

            //Now verify running all code

            string optimizedResult;
            string optimizedScreened;

            using (CheckPoint checkPoint = new CheckPoint("Run optimized"))
                RunOptimized(out optimizedResult, out optimizedScreened);

            using (CheckPoint checkPoint = new CheckPoint("Build and run normal"))
                RunWithParams(includedFiles, includeFileParameters, optimizedScreened, "Normal", "");

            using (CheckPoint checkPoint = new CheckPoint("Run combined"))
                RunCombined(optimizedScreened);

            using (CheckPoint checkPoint = new CheckPoint("Build and run profiled"))
                RunWithParams(includedFiles, includeFileParameters, optimizedScreened, "Profiled", "/DPROFILE /F30000000");

            using (CheckPoint checkPoint = new CheckPoint("Build and run debugged"))
                RunWithParams(includedFiles, includeFileParameters, optimizedScreened, "Debugged", "/DDEBUG");

            //using (CheckPoint checkPoint = new CheckPoint("Run real openCL"))
                //RunRealOpenCL(optimizedScreened, combineFileParameters);            
        }
        
        private static int BuildSource(string[] includedFiles, string includeFileParameters, string buildArgs, string buildName)
        {
            //Build
            do
            {                
                DateTime timeStamp = File.GetLastWriteTime(buildDirectory + "EntryPoint.exe");

                string buildResult = DoBuild("x64", includeFileParameters + " " + buildArgs);

                //Success! (probably...)
                if (timeStamp != File.GetLastWriteTime(buildDirectory + "EntryPoint.exe"))
                    return 0;

                //Try a bunch, as long as our error is changing
                for (int x = 0; x < 10; x++)
                {
                    string prevResult = buildResult;
                    Thread.Sleep(100);

                    buildResult = DoBuild("x64", includeFileParameters + " " + buildArgs);
                    if (timeStamp != File.GetLastWriteTime(buildDirectory + "EntryPoint.exe"))
                        return x + 2;

                    if (prevResult == buildResult) //We are getting nowhere
                        break;
                }

                Console.WriteLine();
                Console.WriteLine(buildResult);
                if (Hlp.GetUserResponse(buildName + " build appears to have failed, try again (YES or NO)?", "YES", "NO") == "NO")
                    break;
            } while (true);

            return 0;
        }

        private static void CombineSource(string[] includedFiles, out string combineFileParameters)
        {
            //Combining source            
            do
            {
                combineFileParameters = "\"" + String.Join("\" \"", includedFiles) + "\" ";
                DateTime combiningTimeStamp = File.GetLastWriteTime(sourceDirectory + "COMBINED_SOURCE.cpp");
                string combiningResult = Hlp.RunProcess(openCLBuildPath, combineFileParameters + " -include_entrypoint -nobuild -writesource", sourceDirectory);

                //Success! (probably...)
                if (combiningTimeStamp != File.GetLastWriteTime(sourceDirectory + "COMBINED_SOURCE.cpp"))
                    break;

                Console.WriteLine();
                Console.WriteLine(combiningResult);

                if (Hlp.GetUserResponse("Combining source appears to have failed, try again (YES or NO)?", "YES", "NO") == "NO")
                    break;
            } while (true);
        }

        private static void BuildCombinedSource(string[] includedFiles, string combineFileParameters)
        {
            //Building combined source
            do
            {
                DateTime combinedTimeStamp = File.GetLastWriteTime(buildDirectory + "COMBINED_SOURCE.exe");

                string combinedResult = DoBuild("x64", Quote(sourceDirectory + "COMBINED_SOURCE.cpp") + " /Ox /Ob2");

                //Success! (probably...)
                if (combinedTimeStamp != File.GetLastWriteTime(buildDirectory + "COMBINED_SOURCE.exe"))
                    break;

                Console.WriteLine();
                Console.WriteLine(combinedResult);

                if (Hlp.GetUserResponse("Building combined source appears to have failed, try again (YES or NO)?", "YES", "NO") == "NO")
                {
                    break;
                }
                else
                {
                    CombineSource(includedFiles, out combineFileParameters);
                }
            } while (true);
        }

        private static void BuildAndTestOpenCL(string combineFileParameters)
        {
            //Build openCL
            do
            {
                string buildOpenCL = Hlp.RunProcess(openCLBuildPath, combineFileParameters + "-testrun", sourceDirectory);

                //Success! (probably...)
                if (buildOpenCL == "")
                    break;

                Console.WriteLine();
                Console.WriteLine(buildOpenCL);

                if (Hlp.GetUserResponse("Building OpenCL appears to have failed, try again (YES or NO)?", "YES", "NO") == "NO")
                    break;
            } while (true);
        }

        private static void BuildRealOpenCL(string combineFileParameters)
        {
            //Build openCL
            do
            {
                string buildOpenCL = Hlp.RunProcess(openCLBuildPath, combineFileParameters, sourceDirectory);

                //Success! (probably...)
                if (buildOpenCL == "")
                    break;

                Console.WriteLine();
                Console.WriteLine(buildOpenCL);

                if (Hlp.GetUserResponse("Building OpenCL appears to have failed, try again (YES or NO)?", "YES", "NO") == "NO")
                    break;
            } while (true);
        }

        private static void RunRealOpenCL(string optimizedScreened, string combineFileParameters)
        {
            //Run openCL
            do
            {
                string openCLResult = Hlp.RunProcess(openCLBuildPath, combineFileParameters + 
                    "-run -incount 1 -outcount 10000 -globalsize 1", sourceDirectory);
                string openCLScreened = String.Join("", GetSpecialParts(openCLResult, true));

                if (openCLScreened == optimizedScreened)
                {
                    LogResults(openCLResult, "OpenCL");
                    Console.WriteLine("OpenCL results: " + String.Join(", ", GetSpecialParts(openCLScreened, false)));
                    break;
                }

                Console.WriteLine("Optimized:");
                Hlp.PrintWithIndent(optimizedScreened, 4);
                Console.WriteLine("OpenCL:");
                Hlp.PrintWithIndent(openCLScreened, 4);

                Console.WriteLine("Discrepancy between optimized and openCL results (results above).");
                if (Hlp.GetUserResponse("Try again (YES or NO)?", "YES", "NO") == "NO")
                    break;

            } while (true);
        }

        private static void LogResults(string results, string name)
        {
            List<string> resultValues = GetSpecialParts(results, false, '(', ')');
            List<string> resultNames = GetSpecialParts(results, false, '<', '>');

            resultValues.InsertRange(0, GetSpecialParts(results, false, '{', '}'));

            while (resultValues.Count > resultNames.Count)
            {
                resultNames.Insert(0, "Timing Information");
            }

            resultValues.Insert(0, lastChangesMade);
            resultNames.Insert(0, "Last changes made          ");

            fileRecords.WriteResults(name, resultValues.ToArray(), resultNames.ToArray());
        }

        private static void RunOptimized(out string optimizedResult, out string optimizedScreened)
        {
            //Running optimized source (baseline)
            optimizedResult = Hlp.RunProcess(buildDirectory + "EntryPoint.exe", "", buildDirectory);
            optimizedScreened = String.Join("", GetSpecialParts(optimizedResult, true));

            LogResults(optimizedResult, "Optimized");
            Console.WriteLine("Optimized results: " + String.Join(", ", GetSpecialParts(optimizedResult, false)));
        }

        private static void RunCombined(string optimizedScreened)
        {
            do
            {
                string combinedResult = Hlp.RunProcess(buildDirectory + "COMBINED_SOURCE.exe", "", buildDirectory);
                string combinedScreened = String.Join("", GetSpecialParts(combinedResult, true));

                if (combinedScreened == optimizedScreened)
                {
                    LogResults(combinedResult, "Combined");
                    Console.WriteLine("Combined results: " + String.Join(", ", GetSpecialParts(combinedResult, false)));
                    break;
                }

                Console.WriteLine("Optimized:");
                Hlp.PrintWithIndent(optimizedScreened, 4);
                Console.WriteLine("Combined:");
                Hlp.PrintWithIndent(combinedScreened, 4);

                Console.WriteLine("Discrepancy between optimized and combined results (results above).");
                if (Hlp.GetUserResponse("Try again (YES or NO)?", "YES", "NO") == "NO")
                    break;
            } while (true);
        }        

        private static void RunWithParams(string[] includedFiles, string includeFileParameters, string optimizedScreened,
            string runName, string buildParams)
        {
            do
            {
                int buildTries = BuildSource(includedFiles, includeFileParameters, buildParams, runName);
                string nonOptimizedResult = Hlp.RunProcess(buildDirectory + "EntryPoint.exe", "", buildDirectory);
                string nonoptimizedScreened = String.Join("", GetSpecialParts(nonOptimizedResult, true));

                if (nonoptimizedScreened == optimizedScreened)
                {
                    LogResults(nonOptimizedResult, runName);
                    Console.WriteLine(runName + " results: " + String.Join(", ", GetSpecialParts(nonOptimizedResult, false)) + " (" + buildTries + " build retries)");
                    break;
                }                

                Console.WriteLine("Optimized:");
                Hlp.PrintWithIndent(optimizedScreened, 4);
                Console.WriteLine(runName + ":");
                Hlp.PrintWithIndent(nonoptimizedScreened, 4);

                Console.WriteLine("Discrepancy between optimized and " + runName + " results (results above).");
                if (Hlp.GetUserResponse("Try again (YES or NO)?", "YES", "NO") == "NO")
                    break;
            } while (true);
        }
    }

    public class FileRecords
    {
        private string baseFilePath;

        List<string> recordNames = new List<string>();
        //The spaces which we end records on
        List<int> recordSpacing = new List<int>();

        public FileRecords(string _baseFilePath)
        {
            baseFilePath = _baseFilePath;
            
            if (File.Exists(baseFilePath))
            {
                string[] lines = File.ReadAllLines(baseFilePath);

                if (lines.Length > 0)
                {
                    string headerLine = lines[0];

                    int lastSpacePos = -1;
                    int curSpacePos = headerLine.IndexOf('|', lastSpacePos + 1);

                    while (curSpacePos >= 0)
                    {
                        recordNames.Add(headerLine.Substring(lastSpacePos + 1, curSpacePos - lastSpacePos - 1).Trim());
                        recordSpacing.Add(curSpacePos);

                        lastSpacePos = curSpacePos;
                        curSpacePos = headerLine.IndexOf('|', lastSpacePos + 1);                        
                    }                    
                }
            }
            else
            {
                File.Create(baseFilePath);
            }
        }
        
        public void AddNewRecordName(string recordName, int spaceAllocated)
        {
            recordNames.Add(recordName);

            string headerName = recordName + Hlp.Space(spaceAllocated - recordName.Length - 1) + "|";

            int lastSpacePos = recordSpacing.Count > 0 ? recordSpacing[recordSpacing.Count - 1] : 0;
            recordSpacing.Add(lastSpacePos + spaceAllocated);

            string[] lines = File.ReadAllLines(baseFilePath);

            if (lines.Length > 0)
            {
                lines[0] += headerName;
                File.WriteAllLines(baseFilePath, lines);
            }
            else
            {
                File.AppendAllText(baseFilePath, headerName);
            }
        }

        public bool AdvanceToRecord(string recordName, ref int pos)
        {
            while (pos < recordNames.Count)
            {
                if (recordNames[pos] == recordName.Trim())
                    return true;
                pos++;
            }

            return false;
        }
        public void WriteResults(string recordName, string[] records, string[] recordNames)
        {
            int pos = 0;

            string outputLine = "";

            if (!AdvanceToRecord("Record Type", ref pos))
                AddNewRecordName("Record Type", 25);

            outputLine += recordName;
            outputLine += Hlp.Space(recordSpacing[pos] - outputLine.Length);

            for (int x = 0; x < records.Length; x++)
            {
                if (!AdvanceToRecord(recordNames[x], ref pos))
                    AddNewRecordName(recordNames[x], Math.Max(recordNames[x].Length + 3, records[x].Length * 2));

                outputLine += records[x];
                outputLine += Hlp.Space(recordSpacing[pos] - outputLine.Length);
            }

            File.AppendAllText(baseFilePath, outputLine + "\r\n");
        }
    }
}
