using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GTPLibrary
{
    //GTP Controller Structure
    public static class BaseController
    {
        #region SendCommand Structure
        public static void SendCommand(int id, string command_name,
        List<Entity> entities, ProcessWrapper proc)
        {
            SendCommand(id.ToString() + " " + command_name, entities, proc);
        }
        public static void SendCommand(int id, string command_name, ProcessWrapper proc)
        {
            SendCommand(id.ToString() + " " + command_name, proc);
        }
        public static void SendCommand(string command_name, List<Entity> entities, ProcessWrapper proc)
        {
            foreach (Entity entity in entities)
                command_name += " " + entity.ToString();

            SendCommand(command_name, proc);
        }

        //command_name OR, just the entire command (created from above or manually)
        public static void SendCommand(string command_name, ProcessWrapper proc)
        {
            proc.sendLine(command_name);
        }
        #endregion

        #region GetNextReponse Structure
        //One or both of id and response may be empty
        public static void GetNextReponse(out int id, out string response, ProcessWrapper proc)
        {
            //Starts with an equal sign, if it has a non \n, then thats id,
            //after that is either space then response, or \n\n.
            //After response is \n\n

            //=id response\n\n
            //=id\n\n
            //= response\n\n
            //=\n\n

            string curLine = proc.readNextLine().Trim();

            while (!curLine.Contains("=") && !curLine.Contains("?"))
                curLine = proc.readNextLine().Trim();

            while (curLine.Length == 0)
            {
                throw new Exception("Recieved empty line? This may be a problem?");
                curLine = proc.readNextLine().Trim();
            }

            if (curLine[0] == '?')
            {
                string error = "";
                error += curLine;
                while (curLine.Length != 0) //Go until we find \n\n (so an empty line)
                {
                    curLine = proc.readNextLine();
                    error += " " + curLine;
                }
                throw new Exception(error);
            }
            else if (curLine[0] != '=')
                throw new Exception("Error expected = or ? on all responses!");

            if (curLine == "=") //No id or response
            {
                curLine = proc.readNextLine(); //read next line still though as we are expecting
                //another line break

                id = -1; //Should be ignored anyway, this is NOT A SENTINEL
                response = null;

                return;
            }
            //else
            if (curLine[1] == ' ') //Just response, no id
            {
                id = -1; //Should be ignored anyway, this is NOT A SENTINEL
                curLine = curLine.Substring(1);
            }
            else
            {
                string[] delimitter = { " " };

                string[] parts = curLine.Split(delimitter, StringSplitOptions.RemoveEmptyEntries);

                id = Convert.ToInt32(parts[0].Substring(0));

                if (parts.Length <= 1) //No response
                {
                    response = null;
                }

                curLine = curLine.Substring(parts[0].Length + 1);
            }

            //There is a response at this point for sure            
            response = "";
            do
            {
                string[] delimitter = { " " };

                string[] parts = curLine.Split(delimitter, StringSplitOptions.RemoveEmptyEntries);

                if (response.Length > 0) //Sort of a hack to keep the line stucture
                    response += "\n";

                foreach (string part in parts)
                    response += part + " ";

            } while ((curLine = proc.readNextLine()) != "");
        }
        #endregion

        public static string SendAndReceiveReponse(string command_name, ProcessWrapper proc)
        {
            return SendAndReceiveReponse(command_name, new List<Entity>(), proc);
        }
        public static string SendAndReceiveReponse(string command_name, List<Entity> entities, ProcessWrapper proc)
        {
            proc.clearOutAllLines();

            //If we use an id we can insure we get the correct response,
            //if we are not getting the correct response we should add this

            if (entities.Count == 0)
                SendCommand(command_name, proc);
            else
                SendCommand(command_name, entities, proc);

            int id;
            string response;

            GetNextReponse(out id, out response, proc);

            return response;
        }

        public static string SendAndReceiveReponse(string command_name, GTPBasics.SendAndRecieve sndRec)
        {
            return SendAndReceiveReponse(command_name, new List<Entity>(), sndRec);
        }
        public static string SendAndReceiveReponse(string command_name, List<Entity> entities, GTPBasics.SendAndRecieve sndRec)
        {
            foreach (Entity entity in entities)
                command_name += " " + entity.ToString();

            return sndRec(command_name);
        }
    }

    public static class Controller
    {
        #region Required Commands
        public static int protocol_version(GTPBasics.SendAndRecieve sndRec)
        {
            string response = sndRec("protocol_version");

            return Convert.ToInt32(response);
        }
        public static string name(GTPBasics.SendAndRecieve sndRec)
        {
            return sndRec("name");
        }
        public static string version(GTPBasics.SendAndRecieve sndRec)
        {
            return sndRec("version");
        }
        public static bool known_command(string command_name, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_string(command_name)); //Super OO just because might as well
            return (new Ent_boolean(sndRec("known_command " + string.Join(" ", entities)))).data;
        }
        public static string[] list_commands(GTPBasics.SendAndRecieve sndRec)
        {
            string response = sndRec("list_commands");

            char[] delimitters = { GTPBasics.LF, GTPBasics.HT, GTPBasics.SPACE }; //I am not entirely sure, but I think just tabs (HT) will be enough
            return response.Split(delimitters, StringSplitOptions.RemoveEmptyEntries);
        }
        public static void quit(ProcessWrapper proc)
        {
            proc.sendLine("quit");
            proc.readNextLine();
            proc.stopAllReadingAndQuit();
        }
        //Unhandled error: "unacceptable size"!
        public static void boardsize(int size, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_int(size)); //Super OO just because might as well
            sndRec("boardsize " + string.Join(" ", entities));
        }
        public static void clear_board(GTPBasics.SendAndRecieve sndRec)
        {
            sndRec("clear_board");
        }
        public static void komi(float new_komi, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_float(new_komi)); //Super OO just because might as well
            sndRec("komi " + string.Join(" ", entities));
        }
        //Unhandled error: "unacceptable size"!
        //coordinates are 0 based from bottom left of board
        public static void play(string color, int xPos, int yPos, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_move(new Ent_color(color), new Ent_vertex(xPos, yPos)));
            sndRec("play " + string.Join(" ", entities));
        }
        //Returning null means resign
        public static Ent_vertex genmove(string color, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_color(color));
            string response = sndRec("genmove " + string.Join(" ", entities));

            if (response.ToLower() == "resign")
                return null;
            else
                return new Ent_vertex(response);
        }
        #endregion

        #region Extra Commands
        //Unhandled error message: "cannot undo"!
        public static void undo(GTPBasics.SendAndRecieve sndRec)
        {
            sndRec("undo");
        }
        public static string showboard(GTPBasics.SendAndRecieve sndRec)
        {
            return sndRec("showboard");
        }
        #endregion

        #region Tournament
        //Unhandled error situations; board not empty!

        public static List<Ent_vertex> fixed_handicap(int number_of_stones, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_int(number_of_stones)); //Super OO just because might as well
            string response = sndRec("fixed_handicap " + string.Join(" ", entities));

            string[] delimitter = { " " };
            string[] parts = response.Split(delimitter, StringSplitOptions.RemoveEmptyEntries);

            List<Ent_vertex> vertices = new List<Ent_vertex>();
            for (int x = 0; x < parts.Length; x++)
                vertices.Add(new Ent_vertex(parts[x]));

            return vertices;
        }
        public static List<Ent_vertex> place_free_handicap(int number_of_stones, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_int(number_of_stones)); //Super OO just because might as well
            string response = sndRec("place_free_handicap " + string.Join(" ", entities));

            string[] delimitter = { " " };
            string[] parts = response.Split(delimitter, StringSplitOptions.RemoveEmptyEntries);

            List<Ent_vertex> vertices = new List<Ent_vertex>();
            for (int x = 0; x < parts.Length; x++)
                vertices.Add(new Ent_vertex(parts[x]));

            return vertices;
        }
        public static void set_free_handicap(List<Ent_vertex> vertices, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>(vertices);
            sndRec("set_free_handicap " + string.Join(" ", entities));
        }

        //Unhandled syntax errors!
        public static void time_settings(
            int main_time, int byo_yomi_time, int byo_yomi_stones, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_int(main_time));
            entities.Add(new Ent_int(byo_yomi_time));
            entities.Add(new Ent_int(byo_yomi_stones));
            sndRec("time_settings " + string.Join(" ", entities));
        }
        public static void time_left(
           string color, int time, int stones, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_color(color));
            entities.Add(new Ent_int(time));
            entities.Add(new Ent_int(stones));
            sndRec("time_left " + string.Join(" ", entities));
        }
        //Unhandled error message: "cannot score"!
        public static string final_score(GTPBasics.SendAndRecieve sndRec)
        {
            return sndRec("final_score");
        }
        public static List<Ent_vertex> final_status_list(string status, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_string(status)); //Super OO just because might as well
            string response = sndRec("final_score " + string.Join(" ", entities));

            string[] delimitter = { " " };
            string[] parts = response.Split(delimitter, StringSplitOptions.RemoveEmptyEntries);

            List<Ent_vertex> vertices = new List<Ent_vertex>();
            for (int x = 0; x < parts.Length; x++)
                vertices.Add(new Ent_vertex(parts[x]));

            return vertices;
        }
        #endregion

        #region Regression
        public static void loadsgf(string filename, int move_number, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_string(filename));
            entities.Add(new Ent_int(move_number));
            sndRec("loadsgf " + string.Join(" ", entities));
        }
        //Same as gen_move, but does not play the move
        public static Ent_vertex reg_genmove(string color, GTPBasics.SendAndRecieve sndRec)
        {
            List<Entity> entities = new List<Entity>();
            entities.Add(new Ent_color(color));
            string response = sndRec("genmove " + string.Join(" ", entities));

            if (response.ToLower() == "resign")
                return null;
            else
                return new Ent_vertex(response);
        }
        #endregion
    }
}
