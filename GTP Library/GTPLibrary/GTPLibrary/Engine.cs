using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GTPLibrary
{
    //GTP Engine Structure
    /// <summary>
    /// Implement this by creating it with the event you will call when commands are recieved (simple lines)
    /// and then subscribing to the appropriate events. Required events are:
    /// int protocol_version
    /// string name
    /// string version
    /// bool known_command
    /// string[] list_commands
    /// void quit
    /// void boardsize
    /// void clear_board
    /// void komi
    /// void play
    /// Ent_vertex genmove
    /// </summary>
    public class CommandReader
    {
        public CommandReader(ref EventHandler<CommandEvent> commandRecieved)
        {
            commandRecieved += new EventHandler<CommandEvent>(CommandReader_commandRecieved);
        }

        public class CommandEvent : EventArgs
        {
            public string command;

            /// <summary>
            /// This is filled in in the called event, and should be used afterwords by the caller.
            /// </summary>
            public string response;

            public CommandEvent(string _command)
            {
                command = _command;
            }

            public override string ToString()
            {
                return command;
            }
        }
        void CommandReader_commandRecieved(object sender, CommandReader.CommandEvent e)
        {
            string[] delimitter = { " " };
            string[] parts = e.command.Split(delimitter, StringSplitOptions.None);

            if (parts.Length <= 0)
                throw new Exception("Recieved an empty string, this is not a command");

            Ent_int id = null; //Null if no id was recieved

            int tempId;
            int pos = 0;

            if (Int32.TryParse(parts[0], out tempId))
            {
                id = new Ent_int(tempId);
                pos++;
            }

            string command_name = parts[pos++];
            string arguments = e.command.Substring(e.command.IndexOf(command_name) + command_name.Length);

            command_name = command_name.ToLower();

            //Call appropriate events
            try
            {
                e.response = "=";
                if (id != null)
                    e.response += id.data.ToString();

                e.response += " ";

                switch (command_name)
                {
                    //case "protocol_version":
                    //   e.response += protocol_version();
                    //    break;

                    case "protocol_version": e.response += protocol_version(); break;
                    case "name": e.response += name(); break;
                    case "version": e.response += version(); break;
                    case "known_command": e.response += known_command(arguments); break;
                    case "list_commands": e.response += list_commands(); break;
                    case "quit": quit(); break;
                    case "boardsize": boardsize(new Ent_int(arguments).data); break;
                    case "clear_board": clear_board(); break;
                    case "komi": komi(new Ent_float(arguments).data); break;
                    case "play": Ent_move move = new Ent_move(arguments); play(move.color.ToString(), move.vertex.xPos, move.vertex.yPos); break;
                    case "genmove": e.response += genmove(new Ent_color(arguments).ToString()); break;
                    case "undo": undo(); break;
                    case "showboard": e.response += showboard(); break;

                    case "fixed_handicap": List<Ent_vertex> fixedVertices = fixed_handicap(new Ent_int(arguments).data);
                        foreach (Ent_vertex vertex in fixedVertices)
                            e.response += " " + version.ToString();
                        break;

                    case "place_free_handicap": List<Ent_vertex> placeFreeVertices = place_free_handicap(new Ent_int(arguments).data);
                        foreach (Ent_vertex vertex in placeFreeVertices)
                            e.response += " " + version.ToString();
                        break;

                    case "set_free_handicap":
                        List<Ent_vertex> setFreeVertices = new List<Ent_vertex>();
                        while (pos < parts.Length)
                            setFreeVertices.Add(new Ent_vertex(parts[pos++]));
                        set_free_handicap(setFreeVertices);
                        break;

                    case "time_settings": time_settings(new Ent_int(parts[pos]).data, new Ent_int(parts[pos + 1]).data, new Ent_int(parts[pos + 2]).data); break;
                    case "time_left": time_left(new Ent_color(parts[pos]).ToString(), new Ent_int(parts[pos + 1]).data, new Ent_int(parts[pos + 2]).data); break;
                    case "final_score": e.response += final_score(); break;

                    case "final_status_list": List<Ent_vertex> finalStatusVertices = final_status_list(parts[pos]);
                        foreach (Ent_vertex vertex in finalStatusVertices)
                            e.response += " " + version.ToString(); break;
                }

                if (e.response.EndsWith(" "))
                    e.response = e.response.Substring(0, e.response.Length - 1);

                e.response += "\n\n";
            }
            catch (Exception error)
            {
                e.response = "? ";
                if (id != null)
                    e.response += id.data.ToString() + " ";

                //Sort of hackish, but it saves me time in adding a check to every case
                if (error.Message == "Object reference not set to an instance of an object.")
                    e.response += command_name + " command known, but not handled.";
                else
                    e.response += error.Message;

                e.response += "\n\n";
            }

            //fill in e.response
            //return [id] [response] \n\n
        }

        //Commands can throw errors, they should throw them according to:
        //http://www.lysator.liu.se/~gunnar/gtp/gtp2-spec-draft2/gtp2-spec.html#SECTION00070000000000000000

        #region Required Commands
        public delegate int protocol_version_delegate();
        public delegate string name_delegate();
        public delegate string version_delegate();
        public delegate bool known_command_delegate(string command_name);
        public delegate string[] list_commands_delegate();
        public delegate void quit_delegate();
        public delegate void boardsize_delegate(int size);
        public delegate void clear_board_delegate();
        public delegate void komi_delegate(float new_komi);
        public delegate void play_delegate(string color, int xPos, int yPos);
        public delegate Ent_vertex genmove_delegate(string color);

        public event protocol_version_delegate protocol_version;
        public event name_delegate name;
        public event version_delegate version;
        public event known_command_delegate known_command;
        public event list_commands_delegate list_commands;
        public event quit_delegate quit;
        public event boardsize_delegate boardsize;
        public event clear_board_delegate clear_board;
        public event komi_delegate komi;
        public event play_delegate play;
        public event genmove_delegate genmove;

        #endregion

        #region Extra Commands
        public delegate void undo_delegate();
        public delegate string showboard_delegate();

        public event undo_delegate undo;
        public event showboard_delegate showboard;

        #endregion

        #region Tournament
        public delegate List<Ent_vertex> fixed_handicap_delegate(int number_of_stones);
        public delegate List<Ent_vertex> place_free_handicap_delegate(int number_of_stones);
        public delegate void set_free_handicap_delegate(List<Ent_vertex> vertices);
        public delegate void time_settings_delegate(int main_time, int byo_yomi_time, int byo_yomi_stones);
        public delegate void time_left_delegate(string color, int time, int stones);
        public delegate string final_score_delegate();
        public delegate List<Ent_vertex> final_status_list_delegate(string status);

        public event fixed_handicap_delegate fixed_handicap;
        public event place_free_handicap_delegate place_free_handicap;
        public event set_free_handicap_delegate set_free_handicap;
        public event time_settings_delegate time_settings;
        public event time_left_delegate time_left;
        public event final_score_delegate final_score;
        public event final_status_list_delegate final_status_list;

        #endregion
    }
}
