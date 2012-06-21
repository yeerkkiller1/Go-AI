using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GTPLibrary
{
    /// <summary>
    /// This provides a common interface so we can access the AI directly from code.
    /// </summary>
    public abstract class SimpleGoPlayer
    {
        /// <summary>
        /// This can be used to change the speed of a go player. Put it up to make the player play faster (but it will generally play worse too).       
        /// </summary>
        public double speedModifier = 1.0;

        public abstract Ent_vertex GetAndPlayMove(string color);
        public abstract void Play(Ent_move move);
        public abstract void ResetAndSetSize(int size);
        public abstract string final_score();
        public abstract void quit();
        public abstract string name { get; }
        public abstract string showboard();
    }

    /// <summary>
    /// Wrappers the underlying AI process with a wrapper so its easy to access.
    /// </summary>
    public class ProcessBasedPlayer : SimpleGoPlayer
    {
        public ProcessWrapper proc;
        public ProcessBasedPlayer(ProcessWrapper _proc)
        {
            proc = _proc;
        }

        public override Ent_vertex GetAndPlayMove(string color)
        {
            return Controller.genmove(color, proc);
        }

        public override void Play(Ent_move move)
        {
            Controller.play(move.color.ToString(), move.vertex.xPos, move.vertex.yPos, proc);
        }

        public override void ResetAndSetSize(int size)
        {
            Controller.clear_board(proc);
            Controller.boardsize(size, proc);
        }

        public override string final_score()
        {
            return Controller.final_score(proc);
        }

        public override void quit()
        {
            Controller.quit(proc);
        }

        public override string name
        {
            get
            {
                return proc.name;
            }
        }

        public override string showboard()
        {
            return Controller.showboard(proc);
        }
    }
}
