using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading;

namespace GTPLibrary
{
    //Implements the GTP protocol, using a ProcessWrapper to help it
    public static class GTPBasics
    {
        public static char HT = (char)9;
        //carriage return
        public static char CR = (char)13;
        //line feed (\n)
        public static char LF = (char)10;

        public static char SPACE = (char)32; //Pretty sure just ' '     

        public delegate string SendAndRecieve(string input);
    }
}
