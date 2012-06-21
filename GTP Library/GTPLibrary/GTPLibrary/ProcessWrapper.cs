using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading;

namespace GTPLibrary
{
    public class ProcessWrapper
    {
        private Process goProcess;

        private Thread readThread;

        public delegate void lineReceived(string line, string aiName);
        private lineReceived lineFromAI = null;

        public GTPBasics.SendAndRecieve messageOrganizer;

        public string name;

        public int readTimeOut = 60000;

        //_lineFromAI can be null
        public ProcessWrapper(string fileLocation, lineReceived _lineFromAI) : this(fileLocation, "", _lineFromAI) { }
        public ProcessWrapper(string fileLocation, string parameters, lineReceived _lineFromAI)
        {
            ProcessStartInfo startInfo;

            if (parameters == "")
                startInfo = new ProcessStartInfo(fileLocation);
            else
                startInfo = new ProcessStartInfo(fileLocation, parameters);     

            startInfo.UseShellExecute = false;
            startInfo.RedirectStandardInput = true;
            startInfo.RedirectStandardOutput = true;

            goProcess = Process.Start(startInfo);

            name = goProcess.ProcessName;

            lineFromAI = _lineFromAI;

            messageOrganizer = new GTPBasics.SendAndRecieve(simpleMessagePasser);

            readThread = new Thread(new ThreadStart(ReadFromStandardInput));
            readThread.Start();
        }

        string simpleMessagePasser(string input)
        {
            clearOutAllLines();

            //If we use an id we can insure we get the correct response,
            //if we are not getting the correct response we should add this

            sendLine(input);

            int id;
            string response;

            BaseController.GetNextReponse(out id, out response, this);

            return response;
        }

        object readLinesLock = new object();
        Queue<string> readLines = new Queue<string>();

        //ATTENTION! The reading is sort of messed up, we just abort the thread to stop it.
        //Using quit we can do it properly... by stopping all reading right after we read
        //the response to quit... but it doesn't really matter        
        private void ReadFromStandardInput()
        {
            string readLine;

            while ((readLine = goProcess.StandardOutput.ReadLine()) != null)
            {
                lock (readLinesLock)
                {
                    //Wipe out any garbage characters here (I should, but I won't until it is a problem)
                    readLines.Enqueue(readLine);
                }

                if (lineFromAI != null)
                {
                    //Spawn this off, in case the thread does not return to us, that way we can give output
                    //without the danger of crashing the entire read process
                    //Thread sendInputThread = new Thread(new ParameterizedThreadStart(callLineFromAI));
                    //sendInputThread.Start(readLine);
                    lineFromAI(readLine, name); //Nvm, it messes up thread syncronization when this happens, so lets not do it
                }
            }
        }
        private void callLineFromAI(object line)
        {
            lineFromAI((string)line, name);
        }

        public string readNextLine()
        {
            if (goProcess == null)
                throw new Exception("Cannot read lines, process has been terminated.");

            string lineRead = null;

            lock (readLinesLock) //Unwrapped so we can sometimes avoid thread sleeping
                if (readLines.Count > 0)
                    lineRead = readLines.Dequeue();

            int timeWaited = 0;

            while (lineRead == null) //Strange structure for multithreaded use of lock
            {
                lock (readLinesLock)
                    if (readLines.Count > 0)
                        lineRead = readLines.Dequeue();
                Thread.Sleep(10); //VERY IMPORTANT TO SLEEP OUTSIDE OF LOCK!
                timeWaited += 10;

                if (timeWaited > readTimeOut)                
                    throw new Exception("read timeout on " + name);                
            }
            return lineRead;
        }

        public void clearOutAllLines()
        {
            lock (readLinesLock)
            {
                while (readLines.Count > 0)
                    readLines.Dequeue();
            }
        }

        public void stopAllReadingAndQuit()
        {
            if (goProcess == null)
                throw new Exception("Cannot close, process has been terminated.");

            lock (readLinesLock)
            {
                readThread.Abort();
            }

            goProcess.Close();
            goProcess = null;
        }

        public void sendLine(string line)
        {
            if (goProcess == null)
                throw new Exception("Cannot send lines, process has been terminated.");

            goProcess.StandardInput.Write(line + "\n");
        }

        public static implicit operator GTPBasics.SendAndRecieve(ProcessWrapper wrapper)
        {
            return wrapper.messageOrganizer;
        }
    }
}
