using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GTPLibrary
{

    //Most entities as defined in the GTP specification (http://www.lysator.liu.se/~gunnar/gtp/gtp2-spec-draft2/gtp2-spec.html) are defined here    
    public abstract class Entity
    {
        public abstract void readFromString(string textBase);

        public abstract override string ToString();
    }

    #region Entities
    public class Ent_int : Entity
    {
        public int data;
        public Ent_int(string textBase)
        {
            readFromString(textBase.Trim());
        }
        public Ent_int(int _data)
        {
            data = _data;
        }
        public override void readFromString(string textBase)
        {
            data = Convert.ToInt32(textBase);
        }
        public override string ToString()
        {
            return data.ToString();
        }
    }

    public class Ent_float : Entity
    {
        public float data;
        public Ent_float(string textBase)
        {
            readFromString(textBase.Trim());
        }
        public Ent_float(float _data)
        {
            data = _data;
        }
        public override void readFromString(string textBase)
        {
            //There is not conversion to float?
            data = (float)Convert.ToDouble(textBase);
        }
        public override string ToString()
        {
            return data.ToString();
        }
    }

    public class Ent_string : Entity
    {
        public string data;
        public Ent_string(string textBase)
        {
            readFromString(textBase.Trim());
        }
        public override void readFromString(string textBase)
        {
            data = textBase;
        }
        public override string ToString()
        {
            return data;
        }
    }

    //Inputs to this are 1 based, but outputs are 0 based
    public class Ent_vertex : Entity
    {
        public int xPos, yPos; //0 based from bottom left (grid system)
        public bool pass = false; //If its pass, it overrides coordinates

        char[] letterCoordinates = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'V', 'U', 'X', 'Z' };

        public Ent_vertex(string textBase)
        {
            readFromString(textBase.Trim());
        }
        public Ent_vertex(int _xPos, int _yPos)
        {
            xPos = _xPos;
            yPos = _yPos;
        }
        public override void readFromString(string textBase)
        {
            textBase = textBase.ToUpper();

            if (textBase == "PASS")
            {
                pass = true;
                return;
            }

            xPos = -1;
            char xLetter = textBase[0];
            for (int x = 0; x < letterCoordinates.Length; x++)
                if (xLetter == letterCoordinates[x])
                {
                    xPos = x;
                    break;
                }
            if (xPos == -1)
                throw new Exception("Cannot recognized '" + textBase + "' as a vertex (coordinates, like A11)");

            yPos = Convert.ToInt32(textBase.Substring(1)) - 1;
        }
        public override string ToString()
        {
            return pass ? "PASS" :
                letterCoordinates[xPos].ToString().ToLower() + (yPos+1).ToString();
        }
    }

    public class Ent_color : Entity
    {
        public int color; //1 is black, 2 is white
        public Ent_color(string textBase)
        {
            readFromString(textBase.Trim());
        }
        public override void readFromString(string textBase)
        {
            char colorLetter = textBase.ToLower()[0];

            if (colorLetter == 'b')
                color = 1;
            else if (colorLetter == 'w')
                color = 2;
            else
                throw new Exception("Unknown color " + textBase);
        }
        public override string ToString()
        {
            if (color == 1)
                return "b";
            else if (color == 2)
                return "w";
            else
                throw new Exception("Unknown color " + color.ToString());
        }
    }

    public class Ent_move : Entity
    {
        public Ent_color color;
        public Ent_vertex vertex;

        public Ent_move(string textBase)
        {
            readFromString(textBase.Trim());
        }

        public Ent_move(Ent_color _color, Ent_vertex _vertex)
        {
            color = _color;
            vertex = _vertex;
        }

        public override void readFromString(string textBase)
        {
            string[] delimitter = { " " };
            string[] parts = textBase.Split(delimitter, StringSplitOptions.RemoveEmptyEntries);

            color = new Ent_color(parts[0]);

            vertex = new Ent_vertex(parts[1]);
        }
        public override string ToString()
        {
            return color.ToString() + " " + vertex.ToString();
        }
    }

    public class Ent_boolean : Entity
    {
        public bool data;

        public Ent_boolean(string textBase)
        {
            readFromString(textBase.Trim());
        }
        public override void readFromString(string textBase)
        {
            if (textBase == "true")
                data = true;
            else if (textBase == "false")
                data = false;
            else
                throw new Exception("Unknown boolean " + textBase);
        }
        public override string ToString()
        {
            return data ? "true" : "false";
        }
    }
    #endregion

}
