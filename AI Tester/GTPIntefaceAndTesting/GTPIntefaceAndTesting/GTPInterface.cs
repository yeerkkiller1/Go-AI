using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using GTPLibrary;
using System.IO;
using System.Reflection;

namespace GTPIntefaceAndTesting
{
    public partial class GTPInterface : Form
    {
        List<SimpleGoPlayer> goAis = new List<SimpleGoPlayer>();

        StreamWriter writer = File.CreateText("logfile.txt");

        Dictionary<string, double> gamesPlayed = new Dictionary<string, double>();
        Dictionary<string, double> blackWinSum = new Dictionary<string, double>();
        Dictionary<string, double> blackScoreSum = new Dictionary<string, double>();

        //We already coded some board controls, so we might as well use this to display the board
        //TestDotNetGoPlayer goodBoardController = new TestDotNetGoPlayer();

        public GTPInterface()
        {
            InitializeComponent();

            //RecurseSpeedTest();
            //return;

            DoubleBuffered = true;

            gamesPlayed.Add("test", 5);

            CreateAI();
            ResetAI();

            foreach (SimpleGoPlayer goAi in goAis)
                goAi.ResetAndSetSize(9);

            Thread testThread = new Thread(new ThreadStart(TestAI));
            testThread.Start();            
        }

        /// <summary>
        /// Dynamically loads all ai which implement SimpleGoPlayer, that are in 
        /// C:\Users\QuentinBrooks\Dropbox\Go AI\AIs (put all release builds here)
        /// </summary>

        void CreateAI()
        {
            QuitAIs(goAis);

            goAis.Clear();

            string path = Directory.GetParent(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)).FullName;
            if (Environment.OSVersion.Version.Major >= 6)
            {
                path = Directory.GetParent(path).ToString();
            }

            path += @"\Dropbox\Go AI\AIs\";

            ProcessWrapper gnuGoAI = new ProcessWrapper(
               path + @"gnugo-3.8\gnugo.exe",
               "--mode gtp", new ProcessWrapper.lineReceived(ReadOutput));

            ProcessWrapper amigo = new ProcessWrapper(
                path + @"amigogtp-1.7\amigogtp.exe",
                new ProcessWrapper.lineReceived(ReadOutput));

            ProcessWrapper pachi = new ProcessWrapper(
                path + @"pachi-5.00\pachi.exe",
                new ProcessWrapper.lineReceived(ReadOutput));

            //goAis.Add(new ProcessBasedPlayer(gnuGoAI)); //takes long turns
            //goAis.Add(new ProcessBasedPlayer(pachi)); //takes long turns
            goAis.Add((new ProcessBasedPlayer(amigo))); //fast turns (but really sucks)
            //goAis.Add(new TestDotNetGoPlayer());

            DynamicallyLoadAI();

            Controller.time_settings(1, 3, 5, pachi);
        }

        void DynamicallyLoadAI()
        {
            string[] dllFiles = Directory.GetFiles(@"C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\",
                "*.dll", SearchOption.AllDirectories);

            //Apparently we CAN load this too
            string[] exeFiles = Directory.GetFiles(@"C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\",
                "*.exe", SearchOption.AllDirectories);

            List<string> filesToTry = new List<string>();
            filesToTry.AddRange(dllFiles);
            filesToTry.AddRange(exeFiles);

            foreach (string file in filesToTry)
            {
                try
                {
                    Assembly assembly = Assembly.LoadFile(file);
                    Type[] types = assembly.GetTypes();

                    //Add all the types
                    foreach (Type type in types)
                    {
                        if (typeof(SimpleGoPlayer).IsAssignableFrom(type))
                        {
                            try
                            {
                                SimpleGoPlayer simpleGoPlayer = (SimpleGoPlayer)Activator.CreateInstance(type);
                                foreach (SimpleGoPlayer otherAi in goAis)                                
                                    if (otherAi.GetType().FullName == simpleGoPlayer.GetType().FullName)
                                        //This is an example of how not to use the exception system :D
                                        throw new Exception("Duplicate");
                                goAis.Add(simpleGoPlayer);                                 
                            }
                            catch(Exception e) //This may also not work, but there may be types that do work!
                            {
                            
                            }
                        }
                    }
                }
                catch //Its okay if it fails, that is probably likely
                {

                }
            }
        }        

        void ResetAI()
        {
            foreach (SimpleGoPlayer goAi in goAis)
            {
                try
                {
                    goAi.ResetAndSetSize(9);
                }
                catch { }
            }

            moveNumber = 1;
            for (int x = 0; x < 9; x++)
                for (int y = 0; y < 9; y++)
                    board[x, y] = 0;
        }

        #region AI Testing
        Random random = new Random();
        void TestAI()
        {                                 
            while (true) //game loop
            {                
                int aiOne = random.Next(goAis.Count);
                int aiTwo = random.Next(goAis.Count - 1);
                if (aiTwo >= aiOne)
                    aiTwo++;

                List<SimpleGoPlayer> curAIs = new List<SimpleGoPlayer>();
                aiOne = 0; //TEMP HARDCODED! REPLACE THIS!
                aiTwo = 1;
                curAIs.Add(goAis[aiOne]);
                curAIs.Add(goAis[aiTwo]);

                RunGame(curAIs);

                ResetAI();

                SafeRefresh();

                writer.Flush();                
            } //game while end

            writer.Flush();
        }

        string blackAi, whiteAi;
        int turnNumber = 0;

        bool lastPassed = false;
        void RunGame(List<SimpleGoPlayer> ais)
        {
            foreach (SimpleGoPlayer ai in ais)
                ai.ResetAndSetSize(9);

            lastPassed = false;

            Output(ais[0].name + "b\t" + ais[1].name + "w\t");

            blackAi = ais[0].name;
            whiteAi = ais[1].name;

            while (true) //Turn loop
            {
                try
                {
                    writer.Flush();

                    Ent_vertex vertex = ais[0].GetAndPlayMove("b"); //0 is b, 1 is w... just because it has to be this way                    
                    if (!PlayMove(ais, new Ent_move(new Ent_color("b"), vertex), 0))
                    {
                        EndGame(ais[0], ais[1]);
                        break;
                    }

                    vertex = ais[1].GetAndPlayMove("w"); //0 is b, 1 is w... just because it has to be this way                    
                    if (!PlayMove(ais, new Ent_move(new Ent_color("w"), vertex), 1))
                    {
                        EndGame(ais[0], ais[1]);
                        break;
                    }

                } //main try catch end
                catch (Exception e)
                {
                    writer.WriteLine(e.ToString());
                    QuitAIs(ais);

                    CreateAI();

                    break;
                }
            } //Play while end

            writer.Flush();
        }

        //Having the ai generate a move without playing it, and then playing on all ais would be the best
        //but... all ai have to be able to play a move but all don't need to generate one without playing it
        //so this is more likely to work for now.
        bool PlayMove(List<SimpleGoPlayer> ais, Ent_move move, int aiExemption)
        {
            if (move.vertex == null) //resign
            {
                Output(move.color.ToString() + " resigned");
                Output("\n");

                return false;
            }

            if (move.vertex.pass)
            {
                Output("PASS" + "\t");

                if (lastPassed)                                   
                    return false;                

                lastPassed = true;
                return true;
            }

            board[move.vertex.xPos, move.vertex.yPos] = move.color.ToString() == "b" ? moveNumber : -moveNumber;
            moveNumber++;

            Output(move.vertex.ToString() + "\t");

            for (int x = 0; x < ais.Count; x++)
            {
                if (x != aiExemption)
                {
                    ais[x].Play(move);                                       
                    lastPassed = false;
                }
            }

            
            //goodBoardController.Play(move); //It knows if the pieces are empty
            board[move.vertex.xPos, move.vertex.yPos] = ++turnNumber * (move.color.color == 1 ? 1 : -1);

            SafeRefresh();

            return true;
        }

        void QuitAIs(List<SimpleGoPlayer> ais)
        {
            for (int x = 0; x < goAis.Count; x++)
            {
                try
                {
                    goAis[x].quit();
                }
                catch { }
            }
        }

        string curLine = "";
        void Output(string textToOutput)
        {
            writer.Write(textToOutput);
            curLine += textToOutput;
            if(textToOutput.Contains("\n"))
            {
                ReadOutput(curLine, "AI Tester");
                curLine = "";
            }
        }

        void EndGame(SimpleGoPlayer blackAI, SimpleGoPlayer whiteAI)
        {
            Output("\nGAME done, final score: ");

            string finalScore = "";

            try
            {
                finalScore = blackAI.final_score();
            }
            catch
            {
                try
                {
                    finalScore = whiteAI.final_score();
                }
                catch
                {
                    Output("neither side can score!");
                }
            }

            if (finalScore != "")
            {
                char winner = finalScore.ToLower()[0];
                double score = Convert.ToDouble(finalScore.Substring(2));

                string winnerName = winner == 'w' ? whiteAi : blackAi;
                string loserName = winner == 'b' ? whiteAi : blackAi;

                Output(winnerName + "\t" + score.ToString()); 
                
                string gameKey = blackAi + " vs " + whiteAi;
                                
                AddToDict(gamesPlayed, gameKey, 1);
                AddToDict(blackWinSum, gameKey, 1);
                AddToDict(blackScoreSum, gameKey, winner == 'b' ? score : -score);
            }

            Output("\n");
        }

        void AddToDict(Dictionary<string, double> dict, string key, double numToAdd)
        {
            if (!dict.ContainsKey(key))
                dict.Add(key, 0);
            dict[key] += numToAdd;
        }
        #endregion

        Semaphore lineDisplayLocker = new Semaphore(1, 100);
        object lineDisplayLock = new object();
        int curLinePos = 0;
        string[] linesDisplayed = new string[40];

        #region Board Interface and AI line output

        //This calls refresh too many times! But it is okay for now...        
        void ReadOutput(string lineRecieved, string aiName)
        {
            string[] delimitter = { "\n" };

            string[] lines = lineRecieved.Split(delimitter, StringSplitOptions.None);

            if (lines.Length > 1)
            {
                foreach (string line in lines)
                    ReadOutput(line, aiName);
                return;
            }

            lineRecieved = lineRecieved.Replace('\t', ' ');

            //There are extra lines every response, and some responses
            //are just confirmations (no response)
            if (lineRecieved.Trim() == "" || lineRecieved.Trim() == "=")
                return;

            //if (aiName == "AI Tester")
            {                
                lock (lineDisplayLock)
                {                    
                    linesDisplayed[curLinePos] = aiName + "--> " + lineRecieved;
                    curLinePos = (curLinePos - 1 + linesDisplayed.Length) % linesDisplayed.Length;
                }
            }

            SafeRefresh();
        }

        void SafeRefresh()
        {
            if (InvokeRequired)
            {
                BeginInvoke(new MethodInvoker(SafeRefresh));
                return;
            }
            else
            {
                CheckForIllegalCrossThreadCalls = false;
                Refresh();
            }
        }
        
        static Font font = new Font("Courier New", 10);
        static Font pieceFont = new Font("Arial", 20);
        private void computerOutput_Paint(object sender, PaintEventArgs e)
        {
            for (int x = 0; x < linesDisplayed.Length; x++)
            {
                int pos = (x + curLinePos) % linesDisplayed.Length;
                if (linesDisplayed[pos] != null)
                    e.Graphics.DrawString(linesDisplayed[pos], font, Brushes.Black, 5, (float)(x * 12));
            }
        }

        public int[,] board = new int[9, 9];
        //0 - empty, negative black, positive white
        int moveNumber = 1;        


        private void nineBoard_Paint(object sender, PaintEventArgs e)
        {
            float dimensionHeight = (float)((nineBoard.Height - 100) / 9.0);
            float dimensionWidth = (float)(nineBoard.Width / 9.0);

            for (int x = 0; x < 10; x++)
            {
                e.Graphics.DrawLine(Pens.Black, x * dimensionWidth, 0, x * dimensionWidth, dimensionHeight * 9);
                e.Graphics.DrawString(letterCoordinates[x].ToString(), new Font("New Courier", 12), Brushes.Black,
                    (float)(x + 0.5) * dimensionWidth, 0);

                
                    e.Graphics.DrawLine(Pens.Black, 0, x * dimensionHeight, dimensionWidth * 10, x * dimensionHeight);
                    if (x < 9)
                    e.Graphics.DrawString((9 - x).ToString(), new Font("New Courier", 12), Brushes.Black,
                        0, (float)(x + 0.5) * dimensionHeight);
                
            }

            DrawX(e.Graphics, (int)(2.5 * dimensionWidth), (int)(2.5 * dimensionHeight), (int)(dimensionHeight * 0.25));
            DrawX(e.Graphics, (int)(4.5 * dimensionWidth), (int)(2.5 * dimensionHeight), (int)(dimensionHeight * 0.25));
            DrawX(e.Graphics, (int)(6.5 * dimensionWidth), (int)(2.5 * dimensionHeight), (int)(dimensionHeight * 0.25));

            DrawX(e.Graphics, (int)(2.5 * dimensionWidth), (int)(4.5 * dimensionHeight), (int)(dimensionHeight * 0.25));
            DrawX(e.Graphics, (int)(4.5 * dimensionWidth), (int)(4.5 * dimensionHeight), (int)(dimensionHeight * 0.25));
            DrawX(e.Graphics, (int)(6.5 * dimensionWidth), (int)(4.5 * dimensionHeight), (int)(dimensionHeight * 0.25));

            DrawX(e.Graphics, (int)(2.5 * dimensionWidth), (int)(6.5 * dimensionHeight), (int)(dimensionHeight * 0.25));
            DrawX(e.Graphics, (int)(4.5 * dimensionWidth), (int)(6.5 * dimensionHeight), (int)(dimensionHeight * 0.25));
            DrawX(e.Graphics, (int)(6.5 * dimensionWidth), (int)(6.5 * dimensionHeight), (int)(dimensionHeight * 0.25));

            for (int x = 0; x < 9; x++)
            {
                for (int y = 0; y < 9; y++)
                {
                    if (board[x, 8 - y] > 0)
                    {
                        e.Graphics.FillEllipse(Brushes.Black, x * dimensionWidth, y * dimensionHeight, dimensionWidth, dimensionHeight);
                        e.Graphics.DrawString(board[x, 8 - y].ToString(), pieceFont, Brushes.White, x * dimensionWidth + 7, y * dimensionHeight + 6);
                    }
                    else if (board[x, 8 - y] < 0)
                    {
                        e.Graphics.FillEllipse(Brushes.White, x * dimensionWidth, y * dimensionHeight, dimensionWidth, dimensionHeight);
                        e.Graphics.DrawString((-board[x, 8 - y]).ToString(), pieceFont, Brushes.Black, x * dimensionWidth + 7, y * dimensionHeight + 6);
                    }
                }
            }            

            e.Graphics.FillEllipse(Brushes.Black, 1 * dimensionWidth, 10 * dimensionHeight, dimensionWidth, dimensionHeight);
            e.Graphics.DrawString(blackAi, pieceFont, Brushes.White, 1 * dimensionWidth + 7, 10 * dimensionHeight + 6);

            e.Graphics.FillEllipse(Brushes.White, 4 * dimensionWidth, 10 * dimensionHeight, dimensionWidth, dimensionHeight);
            e.Graphics.DrawString(whiteAi, pieceFont, Brushes.Black, 4 * dimensionWidth + 7, 10 * dimensionHeight + 6);
        }
        private void DrawX(Graphics g, int centerX, int centerY, int radius)
        {
            g.DrawLine(Pens.Black, centerX - radius, centerY, centerX + radius, centerY);
            g.DrawLine(Pens.Black, centerX, centerY - radius, centerX, centerY + radius);
        }

        private void winRateDisplay_Paint(object sender, PaintEventArgs e)
        {
            int x = 0, y = 0;

            foreach (string key in gamesPlayed.Keys)
            {
                e.Graphics.DrawString(key, font, Brushes.Black, x, y);

                x += 10;
                y += 10;
                if (blackWinSum.ContainsKey(key) && gamesPlayed.ContainsKey(key))
                    e.Graphics.DrawString(formatNumber(blackWinSum[key] / gamesPlayed[key], 5) + " vs " +
                        formatNumber(1 - (blackWinSum[key] / gamesPlayed[key]), 5), font, Brushes.Black, x, y);

                y += 10;
                if (blackScoreSum.ContainsKey(key) && gamesPlayed.ContainsKey(key))
                    e.Graphics.DrawString(formatNumber(blackScoreSum[key] / gamesPlayed[key], 5) + " average black score", font, Brushes.Black, x, y);

                y += 20;
                x = 0;
            }
        }
        string formatNumber(double number, int decimalPlaces)
        {
            string text = number.ToString();

            return text.Substring(0, Math.Min(text.Length, decimalPlaces));
        }

        char[] letterCoordinates = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'V', 'U', 'X', 'Z' };

        private void nineBoard_MouseClick(object sender, MouseEventArgs e)
        {
            float dimensionHeight = (float)((nineBoard.Height - 100) / 9.0);
            float dimensionWidth = (float)(nineBoard.Width / 9.0);

            int x = (int)(e.X / dimensionWidth);
            int y = 8 - (int)(e.Y / dimensionHeight);

            if (board[x, y] == 0)
            {
                board[x, y] = moveNumber++;

                PlayMove(goAis, new Ent_move(new Ent_color("b"), new Ent_vertex(x, y)), -1);                
            }

            nineBoard.Refresh();
        }

        private void getComputerMove_Click(object sender, EventArgs e)
        {
            Ent_vertex vertex = goAis[0].GetAndPlayMove("w");

            PlayMove(goAis, new Ent_move(new Ent_color("w"), vertex), 0);

            //board[vertex.xPos, vertex.yPos] = -(moveNumber++);

            nineBoard.Refresh();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            goAis[0].showboard();
        }

        private void GTPInterface_Click(object sender, EventArgs e)
        {

        }

        #endregion
    }

    //As oppposed to just a custom pure code player, which is useful for debugging and stuff
    
}