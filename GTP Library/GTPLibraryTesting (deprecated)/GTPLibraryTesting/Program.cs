using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading;
using GTPLibrary;

namespace GTPLibraryTesting
{
    class Program
    {
        static void Main(string[] args)
        {            
            
            ProcessWrapper gnuGoAI = new ProcessWrapper(
                @"C:\Users\quentin\Dropbox\Go AI\AIs\gnugo-3.8\gnugo.exe",
                "--mode gtp", new ProcessWrapper.lineReceived(lineRecieved));

            Controller.play("b", 5, 3, gnuGoAI);

            //int protocol_version = Controller.protocol_version(gnuGoAI);
            //string name = Controller.name(gnuGoAI);
            //string version = Controller.version(gnuGoAI);

            //bool knowPlay = Controller.known_command("play", gnuGoAI);
           // bool knowRandom = Controller.known_command("random", gnuGoAI);

            //string[] allCommands = Controller.list_commands(gnuGoAI);


            //Controller.boardsize(9, gnuGoAI);
            //Controller.clear_board(gnuGoAI);
            //Controller.komi(3.3f, gnuGoAI);
            Controller.play("b", 5, 3, gnuGoAI);                                                
            //Ent_vertex vertex = Controller.genmove("b", gnuGoAI);

            Controller.quit(gnuGoAI);
            
            //gnuGoAI.sendLine("boardsize 9");

            

            //GTP.CommandReader reader = new GTP.CommandReader();

            //Setup the engine
            commandRecieved = new EventHandler<CommandReader.CommandEvent>(CommandReader_commandRecieved);
            CommandReader reader = new CommandReader(ref commandRecieved);

            reader.boardsize += new CommandReader.boardsize_delegate(reader_boardsize);

            //Talk to it using the controller code
            Controller.boardsize(8, SndRec);


            Console.Read();
        }

        static void reader_boardsize(int size)
        {
            
        }

        //Engine Side
        static EventHandler<CommandReader.CommandEvent> commandRecieved;
        static void CommandReader_commandRecieved(object sender, CommandReader.CommandEvent e)
        {
            //Extra command recieved function, commands are send to this 
            //(just because I have to due to EventHandler instantiation ?)
            //And they are send to the GTP.CommandReader, which spawns them 
            //off to the appropriate events (which I can subscribe too).

            Console.WriteLine("ENGINE <-- " + e.command);
        }

        //Controller side
        static string SndRec(string input)
        {
            CommandReader.CommandEvent cmdEvent = new CommandReader.CommandEvent(input);

            Console.WriteLine("CONTROLLER --> " + input);

            commandRecieved(null, cmdEvent); //Send to engine

            Console.WriteLine("CONTROLLER <-- " + cmdEvent.response);

            return cmdEvent.response;
        }

        static void lineRecieved(string line)
        {
            Console.WriteLine(line);
        }
    }

}
